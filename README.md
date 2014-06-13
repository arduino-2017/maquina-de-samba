# Máquina de Samba

A robotic drum player by [Crear - Arte y Tecnología](http://elarteylatecnologia.com.ar)

## Hardware requisites:

  - An arduino UNO.
  - A motor shield.
  - Some electromechanical actuators, like the ones used for car locks.
  - An external power source.

### Building remarks:
  - Be sure to remove the power jumper on the motor shield before connecting the external source.


## Building the firmware:

We use [Ino](http://inotool.org "Ino is a command line toolkit for working with Arduino hardware"), so running
```
ino build
ino upload
```
should work.

If you want to build using the Arduino IDE copy the sketch found under /src and add the libraries on /lib.

### Third party libraries included:

  - [Adafruit Motor Shield](https://github.com/adafruit/Adafruit-Motor-Shield-library/zipball/master)
  - [Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library) by Francois Best.


## Midi controls

This devices listens on all channels and maps (for now) notes 57 59 60 and 62
to the four actuators.


## TODO:
  - Proper handling of repeated notes and velocity.
  - Dynamic map of notes to channels.
