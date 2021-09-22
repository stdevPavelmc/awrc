# Amateur Wireless Rotor Controller

An arduino based rotor controller connected via WiFi using the ESP8266 mcu

## Explain...

Why to use cables whey you have wifi around?

The hamlib project has a simple TCP daemon for abstracting the rotor control, so we implemented the basic commands for that protocol and simulate a `rotctld` instance right on the rotor hardware.

## How to compile & test

We use platformio, not the Arduino IDE, and main development is on linux.

```sh
git clone https://github.com/stdevPavelmc/awrc.git
cd awrc
git submodule update
```

Now just open the Platformio IDE and open the project and run.

## This is a work in progress

Until labeled otherwise this project is an `alpha` code.
