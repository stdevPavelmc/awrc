# Amateur Wireless Rotor Controller

An arduino based rotor controller connected via WiFi using the ESP8266 MCU

## Rationale

Why use cables when you have wifi around?

The hamlib project has a simple TCP daemon for abstract the rotor control, so we implemented the basic commands for that protocol and simulate a `rotctld` instance right on the rotor hardware; as today most of the softwares I use supports rotctld directly.

A web interface with position and simple commands is built on, I will improve it with time but this is low priority.

## How to compile

We use platformio, not the Arduino IDE, and main development is on linux (It must work on Windows/Mac), so you need Vscode/Codium and the Platformio addon.

By now I'm managing the dependencies as git submodules, so to install them you must do on the console this:

```sh
git clone https://github.com/stdevPavelmc/awrc.git
cd awrc
git submodule update
```

I will port them to platformio.ini deps in the future. Now just open the Platformio IDE and open the project and run. If you have internet plarformio will install the frameworks and tools for you.

## Hardware

There is a folder named `awrc-kicad` that has the project for board and schematics.

## This is a work in progress

Until labeled otherwise this project is an `alpha` code.
