const fxRack = require('./build/Release/fx-rack');

fxRack.play();
setTimeout(() => {
  fxRack.stop();
}, 1000);
