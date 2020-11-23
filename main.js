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
    width: 400,
    height: 600,
    resizable: false,
    webPreferences: {
      nodeIntegration: true,
      enableRemoteModule: true
    }
  });

  win.setMenu(null);
  win.loadFile('public/index.html');
  win.webContents.openDevTools();

  ipcMain.on('load', (_, path) => {
    try {
      const meta = fxRack.load(path);
      win.webContents.send('meta', meta);
    } catch (err) {
      dialog.showErrorBox('Load Error', err.message);
    }
  });

  ipcMain.on('play', (_, params) => {
    try {
      fxRack.play(params);
    } catch (err) {
      dialog.showErrorBox('Play Error', err.message);
    }
  });

  ipcMain.on('stop', () => fxRack.stop());
  ipcMain.on('reset', () => fxRack.reset());
  ipcMain.on('skip', (_, value) => fxRack.skip(value));

  fxRack.onCursorMove(cursor => {
    win.webContents.send('cursorMove', cursor);
  });
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
