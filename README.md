# FX Rack
This is an audio effect rack inspired by Reason and it was created as a semestral project for CTU. Included are 3 effect modules:
- High-pass / Low-pass Filter
- Reverb
- Distortion (hard-clip)

The only currently supported audio format is uncompressed WAV.

## Technology
This project is separated into the back-end, which handles the audio processing, and the front-end, which provides user interface for creating custom FX chains.

The front-end is using Svelte with TypeScript as its UI framework and the whole app is wrapped in Electron. The back-end is written in C++ and uses the library **iimavlib** *(https://github.com/iimcz/iimavlib)*, which was a mandatory dependecy for this university course. The C++ code is compiled as a native module for Node and imported directly into Electron.

## Build
In order to build this project, you've got to have CMake and a C++ compiler installed and included in your PATH. After installing all the external dependencies with `npm i`, you can start the build process by running `npm run build`. The output executable will be located in the `/dist` folder.

## Credits
- Filip Toman
- Vít Říha
