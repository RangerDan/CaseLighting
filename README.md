CaseLighting
============

Ardunio Project to provide backlights for a presentation case using addressable RGB LED Strips from RadioShack

Hardware Requirements
- SD Card Shield (SeeedStudio model works out of the box)
- RadioShack Tricolor LED Strip (1 meter, currently.  Upgradeable to 2 meters)
- 12V Power Supply with 5.5mm OD, 2.1mm ID barrel connector
- Arduino Uno R3
- USB A to B cable (for serial communication with Arduino)

Software Requirements
- Arduino IDE 1.0.6
- Modified Pololu drivers for properly communicating with the Radio Shack Tricolor RGB LED Strip: https://github.com/RangerDan/RadioShackTricolorLEDStrip

File Syntax
- Color Definition File Syntax
- Each line in the file that is used to define the color patterns uses the following syntax:
<RED, 0-255>|<GREEN, 0-255>|<BLUE, 0-255>&[T<RED, 0-255>|<GREEN, 0-255>|<BLUE, 0-255>$]
- Up to MAX_LED_COUNT worth of lines will be sent to the LED Strip even if they are not hooked up
- Samples will be provided

TODO
- Instructions for preparing the strips for use in carriages
- Design Carriages for strips and validate design with 3D Printing, assembly
- Validate file format, provide message to serial on error
- Expand to support up to 2 meters and test with extra strip
- Provide pictures and other documentation of assembly and operation.
