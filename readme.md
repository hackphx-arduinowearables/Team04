# HackPHX 2014 - Team 04

Big thanks to everyone that made this event a possibility. It was awesome!

# Over 9000

![Head Unit](http://i.imgur.com/kUpPTDW.jpg)
![Power Display](http://i.imgur.com/6uM2GPN.jpg)

Our project, also appropriately referred to as *The Google Glass Killer*, uses the following components components:

- Xadow
- 3-Axis Digital Accelerometer ADXL345
- OLED Display (128x64)
- Addressable RGB LED strip
- Ping sensor
- Single LED
- Wire
- Jumper headers
- Solder

## General Design process.

The edge of the head unit, that meets with the users head, was shaped by photo mapping real human heads (resulting imagery in the resources folder). From there, the 'box' of the head unit was designed around that edge. The box was then physically sewn and zip tied to raquet ball eye proection (eye protection of the higher caliber). Protective blue foam (chosen for its comfort) was added to the edge that meets the users head. 3 Labeled audio jacks were added to the back side of the head unit. A human face was spray painted on to the front side.

A number of apps were coded to run on the head unit hardware. Including highly accurate power level meters, distance sensors, and a software friend to give you words of encouragement.

Images of the project being built can be found in the `pictures` folder.

Notes that lead to the finished product can be found in `notes`.

Resources used to build the process can be found in `resources`.

## Power Level

The user's power level, measured by accelerometers in the head unit, is displayed on the internal screen. The LED strip on the outside of the head unit indicates the power level, 1 LED per 1000 power units, and blinking red when *over 9000*.

## Reflective technology

The head unit uses reflective technology for the display, solving the human eye focal limitations and keeping the unit from extending too far in front of the face. While the front side of the head unit is only a few inches in front of the user's eye, a two mirrors reflect the display from the back of the head unit, creating a total visual distance of ~1 foot. When worn, the screen becomes the sole light source from inside the head unit, giving it an errie and constant presence in front of the user.

## Human Face cammoflouge

A stenciled image of a human face was applied to the front of the head unit to make the head unit indistinguishable from a normal human face. The LED in the corner, was added so as not to distress by-standers

## External Device ports

Many of the Xadow pins are connect to quarter inch jacks, a very standard cable format. External devices can be connected to the head unit with corresponding quarter inch jacks, offering unprecidented expandibility.

## Distance sensor

Extending from the head unit, by wire, is connected a glove supplemented by a ultra sonic distance sensor. The distance read by the sensor is displayed on the internal screen. Distances under 2 feet are measured in inches, over 2 feet is measured in feet.

## Budget

We managed to stick to parts found in the lab, in addition to the parts provided to us. Our out-of-pocket cost was $0.00 USD.
