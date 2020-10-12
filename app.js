const path = require('path');
const fxRack = require('./build/Release/fx-rack');

try {
  fxRack.play(path.join(__dirname, './sample.wav'));
} catch (err) {
  console.log(err);
}
