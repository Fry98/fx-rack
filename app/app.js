const { ipcRenderer } = require('electron');
const play = () => ipcRenderer.send('play');
const stop = () => ipcRenderer.send('stop');
