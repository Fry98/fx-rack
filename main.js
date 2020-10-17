const { app, BrowserWindow, ipcMain } = require('electron');
const path = require('path');
const fxRack = require('./build/Debug/fx-rack');

function createWindow () {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      nodeIntegration: true
    }
  });

  win.setMenu(null);
  win.setResizable(false);
  win.loadFile('app/index.html');
}

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

ipcMain.on('play', () => {
  const x = fxRack.load(path.join(__dirname, './sample.wav'));
  console.log(x);
});
// ipcMain.on('stop', () => fxRack.stop());
