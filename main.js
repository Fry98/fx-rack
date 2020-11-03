const { app, BrowserWindow, ipcMain, dialog } = require('electron');
const path = require('path');
const fxRack = require('./build/Debug/fx-rack');

function createWindow () {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      nodeIntegration: true,
      enableRemoteModule: true
    }
  });

  win.setMenu(null);
  win.setResizable(false);
  win.loadFile('app/index.html');
  win.webContents.openDevTools();
}

app.allowRendererProcessReuse = true;
app.whenReady().then(createWindow);

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow();
  }
});

ipcMain.on('load', () => {
  try {
    const x = fxRack.load(path.join(__dirname, './sample.wav'));
  } catch (err) {
    dialog.showErrorBox('Load Error', err.message);
  }
});

ipcMain.on('play', () => {
  try {
    fxRack.play();
  } catch (err) {
    dialog.showErrorBox('Play Error', err.message);
  }
});

ipcMain.on('stop', () => {
  fxRack.stop();
});
