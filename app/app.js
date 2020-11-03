const { ipcRenderer } = require('electron');
const { dialog } = require('electron').remote;
const play = () => ipcRenderer.send('play');
const stop = () => ipcRenderer.send('stop');

const load = () => {
  dialog.showOpenDialog({
    properties: ['openFile'],
    filters: [
      { name: 'Wave File', extensions: ['wav'] }
    ]
  }).then(files => {
    ipcRenderer.send('load', files.filePaths[0]);
  });
};
