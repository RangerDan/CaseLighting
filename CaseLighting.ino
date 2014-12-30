// Serial Prep
// Used for logging behavior to the PC
const long SERIAL_BAUD_RATE = 115200L;

/* SD card file dump
 This example shows how to read a file from the SD card using the
 SD library and send it over the serial port.
 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 
 created  22 December 2010
 by Limor Fried
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain. */
#include <SD.h>
// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int DEFAULT_CHIP_SELECT_PIN = 10;
const int CHIP_SELECT = 4;
// Color Definition File Syntax
//  Each line in the file that is used to define the color patterns uses the following syntax:
// <RED, 0-255>|<GREEN, 0-255>|<BLUE, 0-255>&[T<RED, 0-255>|<GREEN, 0-255>|<BLUE, 0-255>$]
// Up to MAX_LED_COUNT worth of lines will be sent to the LED Strip even if they are not hooked up
// Numerical input outside 0-255 is constrained to inside the bounds.
// If parsing fails at any time, all parsing will cease and an error will be printed to Serial

// Delimiters used for file structure
const char COLOR_DELIMITER = '|';
const char SET_DELIMITER = '&';
const char TRANSITION_COLOR_DELIMITER = 'T';
const char TRANSITION_SET_DELIMITER = '$';

// LED Strip Prep
// Structure in this sketch adapted from Pololu LED Strip example LedStripColorTester
#include <RadioShackRgbLedStrip.h>                // LED Strip library
bool PololuLedStripBase::interruptFriendly = false;
const int LED_STRIP_PIN = 9;               // Strip input is on Pin 9
PololuLedStrip<LED_STRIP_PIN> ledStrip;    // Create an ledStrip object on pin X.
const int MAX_LED_COUNT = 10;                  // Project requires MAX_LED_COUNT LEDs
rgb_color colors[MAX_LED_COUNT];
const rgb_color C_OFF(0,0,0);

/* ------------------------------------------------------------------------------- */
/// void setup
///   
/* ------------------------------------------------------------------------------- */
void setup()
{  
  // Initialize Serial
  Serial.begin(SERIAL_BAUD_RATE);
  
  // Initialize the file buffer and position
  char fileName[] = "colordef.txt";
  String fileBuffer = "";
        
  if (GetFileFromSD(fileName, fileBuffer))
  {
    // Parse File contents
    // TODO: Add more file validation
    // Initialize the array of colors
    for(int x=0;x<MAX_LED_COUNT;x++)
    {
      colors[x]  = C_OFF;
    }
    int ledCounter = 0;          // Counts LEDs, ensures no more than MAX_LED_COUNT are read from the file
    int nextSetLocation = 0;     // Tracks where the last set delimiter was encountered
    int nextColorLocation = 0;   // Tracks where the last color delimiter was encountered in the 'current set'
    String currentSet = "";      // String to track the current set of LEDs being read from the file
  
    do
    {
      currentSet = fileBuffer.substring(nextSetLocation,fileBuffer.indexOf(SET_DELIMITER,nextSetLocation));
      nextSetLocation = fileBuffer.indexOf(SET_DELIMITER,nextSetLocation)+1;
      Serial.println(currentSet);
      
      colors[ledCounter].red = currentSet.substring(0,currentSet.indexOf(COLOR_DELIMITER)).toInt();
      nextColorLocation = currentSet.indexOf(COLOR_DELIMITER)+1;
      colors[ledCounter].green = currentSet.substring(nextColorLocation, currentSet.indexOf(COLOR_DELIMITER, nextColorLocation)).toInt();
      nextColorLocation = currentSet.indexOf(COLOR_DELIMITER,nextColorLocation)+1;
      colors[ledCounter].blue = currentSet.substring(nextColorLocation).toInt();    
      
      ledCounter++;
    } while ((nextSetLocation != fileBuffer.lastIndexOf(SET_DELIMITER)) && (ledCounter < MAX_LED_COUNT));
  }
  
  // Write to the LED Strip  
  Serial.println(F("Starting to write to the strip."));
  ledStrip.write(colors, MAX_LED_COUNT);  
  Serial.println(F("Done writing to strip."));
  
  // Preparing to enter diagnostic mode
  Serial.println(F("Listening for colors on serial of the form \"set|red|green|blue\""));
  Serial.println("Set range is 1 to " + MAX_LED_COUNT);
  Serial.println(F("Color range is 0-255"));
}

/* ------------------------------------------------------------------------------- */
/// void loop
/// Author: RangerDan
/// Set LED colors interactively through Serial: "set|red|green|blue"
/* ------------------------------------------------------------------------------- */
void loop()
{
  // TODO: Add the ability to pull different file names off the card through Serial
  // TODO: Test if GetFileFromSD works with multiple files per session
  while (Serial.available() > 0)
  {  
    int set = Serial.parseInt();
    int red = Serial.parseInt(); 
    int green = Serial.parseInt(); 
    int blue = Serial.parseInt(); 
    
   Serial.println(set);
   Serial.println(red);
   Serial.println(green);
   Serial.println(blue);

    // look for the newline. That's the end of your
    // sentence:
    if (Serial.read() == '\n')
    {
      set = constrain(set, 1, MAX_LED_COUNT)-1;      // Constrained, mapped to zero-indexed array
      colors[set].red = constrain(red, 0, 255);      // Constrained to range of colors on LEDs
      colors[set].green = constrain(green, 0, 255);
      colors[set].blue = constrain(blue, 0, 255);
      
      Serial.println(F("Starting to write to the strip."));
      ledStrip.write(colors, MAX_LED_COUNT);  
      Serial.println(F("Done writing to strip."));
    }
  }  
}

boolean GetFileFromSD(char* fileName, String& fileBuffer)
{
  Serial.println(F("Initializing SD card..."));
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(DEFAULT_CHIP_SELECT_PIN, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(CHIP_SELECT))
  {
    Serial.println(F("Card failed, or not present"));
    return false;
  }
  else
  {
    Serial.println(F("Card initialized"));
    
    // Attempt to open the file
    File dataFile = SD.open(fileName);

    // if the file is available, read from it:
    if (!dataFile)
    {
      Serial.println(F("Error opening file"));
      return false;
    }
    else
    {
      while (dataFile.available())
      {
        fileBuffer += char(dataFile.read());
      }
      dataFile.close(); 
    }
  }
  // DEBUG STATEMENT
  Serial.println(fileBuffer);
  return true;
}
