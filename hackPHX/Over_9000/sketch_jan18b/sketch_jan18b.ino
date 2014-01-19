/*#########################################
 #                Team 4                  #
 #            "The GG-Killer"             #
 ##########################################
 #  Created by:                           #
 #    Todd Harrison,  "Arduino-King"      #
 #    Chad Stearns,   "Craft-Master"      #
 #    Joe Fleming,    "Jack-of-All-Trades"#
 #    Doug Sheridan,  "The Hacker"        #
 ##########################################*/
#include <Wire.h>
#include "FastLED.h"
#include <ADXL345.h>
#include <SeeedOLED.h>
#include <math.h>
#include <string>
#include <cstdlib.h>

#define DEBUG 0

// Number of LEDs
#define NUM_LEDS 9
#define DATA_PIN 15
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

ADXL345 adxl; //variable adxl is an instance of the ADXL345 library

// Global variables
int powerLevel;
int count;

// Ping variables
int pingPin = 5;  // pin for ping sensor (should use A5 on xadow)
int ledPin = 1;   // pin turn on LED (should use 1 on xadow)
long duration;    // duration for ping return with evaluates to distance
long inches;      // inches to object
long feet;        // feet to object

void setup() {
  initSerial();
  initDisplay();
  pingSetup();
  FastLED.addLeds<NEOPIXEL, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
  // Get accelerometer data
  int x,y,z;

  //read the accelerometer values and store them in variables  x,y,z
  adxl.readXYZ(&x, &y, &z);

  // Perform all Ping ops
  pingStart();
  
  // Decrease power level periodically
  dec_powerLevel();

  // Calculate the new power level
  powerLevel += get_powerLevel();
  
  // Print the power level
  print_powerLevel();

  // Draw the power meter
  clear_5thRow();
  draw_powerMeter(powerLevel);

  // Print the ping data
  print_pingData();

  // Update the LED strip to represent the power level
  updateLED(powerLevel);

  count++;
  delay(125);
}

void updateLED(int x) {
    // Input x : Number of LEDs to turn on
    int i, j;
    
    x = x/1000;
    
    if (x <= 9) {
      // Turn on correct number of LEDs
      for (i=0; i<x; i++) {
         leds[i] = CRGB::Red;
      }
      
      // Turn off all remaining LEDs
      for (j=NUM_LEDS-1; j>=x; j--) {
         leds[j] = CRGB::Black;
      }
      
      // Update the light setting
      FastLED.show();
    } else {
        blinkAll();
    }
}

void blinkAll(){
  // Set all LEDs to Black
  for (int i=0; i<NUM_LEDS; i++) {
       leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(50);
  // Set all LEDs to Red (aka. Green)
  for (int i=0; i<NUM_LEDS; i++) {
       leds[i] = CRGB::Green;
  }
  FastLED.show();
}

void clearPL() {
  // Clear powerLevel area
  SeeedOled.setTextXY(3,5);
  SeeedOled.putString("          ");
}

void clear_5thRow() {
  // Clear the 5th row
  SeeedOled.setTextXY(5,2);
  SeeedOled.putString("              ");
}

void draw_powerMeter(int x) {
    int i, j;
    char* meterString;
    x = x/1000;

    SeeedOled.setTextXY(5,1);
    SeeedOled.putString("[");

    switch (x) {
      case 0:
        meterString = "";
        break;
      case 1:
        meterString = "#";
        break;
      case 2:
        meterString = "##";
        break;
      case 3:
        meterString = "###";
        break;
      case 4:
        meterString = "####";
        break;
      case 5:
        meterString = "#####";
        break;
      case 6:
        meterString = "######";
        break;
      case 7:
        meterString = "#######";
        break;
      case 8:
        meterString = "########";
        break;
      default:
        meterString = "OVER 9000!!";
        break;
    }

    SeeedOled.setTextXY(5,2);
    SeeedOled.putString(meterString);
    SeeedOled.setTextXY(5,13);
    SeeedOled.putString("]");
}

void print_pingData() {
    long fixedInches;

    if (inches >= 12) {
      fixedInches = inches % 12;
    } else {
      fixedInches = inches;
    }

    SeeedOled.setTextXY(6,0);
    SeeedOled.putString("Distance :");
    SeeedOled.setTextXY(6,10);
    SeeedOled.putNumber(feet);
    SeeedOled.setTextXY(6,12);
    SeeedOled.putString("\'");
    SeeedOled.setTextXY(6,13);
    SeeedOled.putNumber(fixedInches);
    SeeedOled.setTextXY(6,15);
    SeeedOled.putString("\"");
}

void print_powerLevel() {
    SeeedOled.setTextXY(1,0);
    SeeedOled.putString("  Power Level  ");
    SeeedOled.setTextXY(2,0);
    SeeedOled.putString(" --------------");
    SeeedOled.setTextXY(3,0);
    SeeedOled.putString("|  > ");
    SeeedOled.setTextXY(3,15);
    SeeedOled.putString("|");
    SeeedOled.setTextXY(3,5);
    SeeedOled.putNumber(powerLevel);
    SeeedOled.setTextXY(4,0);
    SeeedOled.putString(" --------------");
}

void negSafety() {
  if (powerLevel < 0) {
      powerLevel = 0;
  }
  clearPL();
  print_powerLevel();
}

void dec_powerLevel() {
  // Increment count, handles decrementing the powerLevel
  if (++count = 4) {
    // Clear the readout
    SeeedOled.setTextXY(3,5);
    SeeedOled.putString("     ");
    
    // Decrement powerLevel on idle
    if (powerLevel > 0) {
      powerLevel -= rand() % 700;
    }

    // Protect powerLevel from going into the negatives
    negSafety();

    // Reset the count
    count = 0;
  }
}

int get_powerLevel() {
  int a,b,c; // starting accel
  int x,y,z; // final accel
  double diff_ax, diff_by, diff_cz; // difference variables
  
  adxl.readXYZ(&x, &y, &z);
  delay(250);
  adxl.readXYZ(&a, &b, &c);
  
  // Algorithm to calculate a pseudo-powerlevel
  diff_ax = abs(x) - abs(a);
  diff_by = abs(y) - abs(b);
  diff_cz = abs(z) - abs(c);
  if ((diff_ax > 15) || (diff_by > 15) || (diff_cz > 15)) {
      return rand() % 500 + 1000;
  } else
      return 0;
}

void pingStart() {
  ping();
  
  //turn on led if objects are under 3 feet
  if (feet < 3) {
     digitalWrite(ledPin, HIGH);
  }
  else {
     digitalWrite(ledPin, LOW);
  }
  
  if (inches < 24) {
    Serial.print("inches=");
    Serial.print(inches);  
  }
  else {
    Serial.print("feet=");
    Serial.print(feet);
  }
  
  Serial.println();
}

void pingSetup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  digitalWrite(ledPin, LOW);    //make sure this is off
}

void ping() {
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // We give a short LOW pulse beforehand to ensure a clean HIGH pulse.
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  // convert the time into a distance
  inches = microsecondsToInches(duration); 
  feet = inches / 12;
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

void refreshDataDisplay() {
  // Prints the values of the 3-axis
  SeeedOled.setTextXY(1,2);           //Set the cursor to 0th Page, 0th Column
  SeeedOled.putString("X=     ");           //Print number
  SeeedOled.setTextXY(2,2);          //Set the cursor to 0th Page, 0th Column
  SeeedOled.putString("Y=     ");           //Print number
  SeeedOled.setTextXY(3,2);           //Set the cursor to 0th Page, 0th Column
  SeeedOled.putString("Z=     ");           //Print number
}

void initDisplay() {
  Wire.begin();
  SeeedOled.init();  //initialze SEEED OLED display
  DDRB|=0x21;
  PORTB |= 0x21;

  SeeedOled.clearDisplay();           //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();       //Set display to Normal mode
  SeeedOled.setPageMode();            //Set addressing mode to Page Mode
}

void initSerial() {
  Serial.begin(9600);
  adxl.powerOn();

  //set activity/ inactivity thresholds (0-255)
  adxl.setActivityThreshold(75); //62.5mg per increment
  adxl.setInactivityThreshold(75); //62.5mg per increment
  adxl.setTimeInactivity(10); // how many seconds of no activity is inactive?

  //look of activity movement on this axes - 1 == on; 0 == off
  adxl.setActivityX(1);
  adxl.setActivityY(1);
  adxl.setActivityZ(1);

  //look of inactivity movement on this axes - 1 == on; 0 == off
  adxl.setInactivityX(1);
  adxl.setInactivityY(1);
  adxl.setInactivityZ(1);

  //look of tap movement on this axes - 1 == on; 0 == off
  adxl.setTapDetectionOnX(0);
  adxl.setTapDetectionOnY(0);
  adxl.setTapDetectionOnZ(1);

  //set values for what is a tap, and what is a double tap (0-255)
  adxl.setTapThreshold(50); //62.5mg per increment
  adxl.setTapDuration(15); //625us per increment
  adxl.setDoubleTapLatency(80); //1.25ms per increment
  adxl.setDoubleTapWindow(200); //1.25ms per increment

  //set values for what is considered freefall (0-255)
  adxl.setFreeFallThreshold(7); //(5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(45); //(20 - 70) recommended - 5ms per increment

  //setting all interrupts to take place on int pin 1
  //I had issues with int pin 2, was unable to reset it
  adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_DOUBLE_TAP_BIT,   ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,    ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_ACTIVITY_BIT,     ADXL345_INT1_PIN );
  adxl.setInterruptMapping( ADXL345_INT_INACTIVITY_BIT,   ADXL345_INT1_PIN );

  //register interrupt actions - 1 == on; 0 == off
  adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
  adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT,  1);
  adxl.setInterrupt( ADXL345_INT_ACTIVITY_BIT,   1);
  adxl.setInterrupt( ADXL345_INT_INACTIVITY_BIT, 1);
}
