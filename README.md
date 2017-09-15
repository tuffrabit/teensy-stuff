# teensy-stuff
Teensyduino sketches

Joystick sketches for Teensyduino.

You may have to update the STICK_X and STICK_Y #defines, those point to the analog pins your joystick potentiometers are wired to. Depending on your desired stick orientation, you may have to also update the subtraction logic on lines 35 and 36.

The deadzone calibration will have to be done before you get any input out of it after your initial flash. Just hold down the center stick button for 8 seconds, the LED will turn on, and then let the stick center itself for a few seconds. Then cycle power on the teensy.
