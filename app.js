const fxRack = require('./build/Release/fx-rack');
const path = require('path');

fxRack.play(path.join(__dirname, './sample.wav'));

setTimeout(() => {
  fxRack.stop();
}, 13000);
