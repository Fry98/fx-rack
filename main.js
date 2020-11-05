const dev = process.env.NODE_ENV === 'development';
const { app, BrowserWindow, ipcMain, dialog } = require('electron');
const fxRack = require(`./build/${dev ? 'Debug' : 'Release'}/fx-rack`);
const path = require('path');

if (dev) {
  require('electron-reload')(
    path.join(__dirname, 'public'), {
      electron: path.join(__dirname, 'node_modules', '.bin', 'electron.cmd')
    }
  );
}

function createWindow () {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    resizable: false,
    webPreferences: {
      nodeIntegration: true,
      enableRemoteModule: true
    }
  });

  win.setMenu(null);
  win.loadFile('public/index.html');
  // win.webContents.openDevTools();
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

ipcMain.on('load', (_, path) => {
  try {
    fxRack.load(path);
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

fxRack.onCursorMove(cursor => {
  console.log(cursor);
});
