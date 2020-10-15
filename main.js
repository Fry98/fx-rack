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
  win.loadFile('index.html');
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

ipcMain.on('play', () => fxRack.play(path.join(__dirname, './sample.wav')));
ipcMain.on('stop', () => fxRack.stop());
