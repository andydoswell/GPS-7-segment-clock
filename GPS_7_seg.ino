/*
 7-segment GPS clock 
 Version 0.9
 Written by Andy Doswell 2015
 License: The MIT License (See full license at the bottom of this file)
   
 Schematic can be found at www.andydoz.blogspot.com/
 
 You will need the LedControl library from http://playground.arduino.cc/Main/LedControl
 and the very excellent TinyGPS++ library from http://playground.arduino.cc/Main/LedControl
 
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to CS
 pin 0 (RX)  is connected to the GPS TX pin

 */
 
#include "LedControl.h"
#include <TinyGPS++.h>

LedControl lc=LedControl(12,11,10,1); //Tells LedControl where our hardware is connected.
TinyGPSPlus gps; //TinyGPS++ class
static const uint32_t GPSBaud = 57600; //Our GPS baudrate. 
int hourTen; //tens of hours
int hourUnit; //units of hours
int minTen; // you get the idea..
int minUnit;
int secTen;
int secUnit;
int centTen; //centiseconds.
int centUnit;
unsigned long timer =0;

void setup() {

  lc.shutdown(0,false); // Wake up the MAX 72xx controller 
  lc.setIntensity(0,8); // Set the display brightness
  lc.clearDisplay(0); //Clear the display
  
  Serial.begin(GPSBaud); // start the comms with the GPS Rx
}

// This contains the bit patterns for the "no GPS" display.

void displayNoGPS() { // displays "noGPS" if the GPS lock isn't valid
  lc.setRow(0,0,B00000000);
  lc.setRow(0,1,B00000000);
  lc.setRow(0,2,B01011011);
  lc.setRow(0,3,B01100111);
  lc.setRow(0,4,B01011110);
  lc.setRow(0,5,B00000000);
  lc.setRow(0,6,B00011101);
  lc.setRow(0,7,B00010101);
}

void displayNoSerial() { // Displays "noSeriAL" in the event of serial comms fail.

  lc.setRow(0,7,B00010101);
  lc.setRow(0,6,B00011101);
  lc.setRow(0,5,B01011011);
  lc.setRow(0,4,B01001111);
  lc.setRow(0,3,B00000101);
  lc.setRow(0,2,B00010000);
  lc.setChar(0,1,'a',false);
  lc.setRow(0,0,B00001110);
}
  
// Displays the time on our LEDs
void displayTime() { 
  if (gps.time.isValid()) {
    timer = millis(); // reset the serial comms timer
    hourUnit = (gps.time.hour()%10);
    hourTen = ((gps.time.hour()/10)%10);
    minUnit = (gps.time.minute()%10);
    minTen = ((gps.time.minute()/10)%10);
    secUnit = (gps.time.second()%10);
    secTen = ((gps.time.second()/10)%10);
    secUnit = (gps.time.second()%10);
    secTen = ((gps.time.second()/10)%10);
    centUnit = (gps.time.centisecond()%10);
    centTen = ((gps.time.centisecond()/10)%10);
    lc.setDigit (0,7,hourTen,false);
    lc.setDigit (0,6,hourUnit,false);
    lc.setDigit (0,5,minTen,false);
    lc.setDigit (0,4,minUnit,false);
    lc.setDigit (0,3,secTen,false);
    lc.setDigit (0,2,secUnit,false);
    lc.setDigit (0,1,centTen,false);
    lc.setDigit (0,0,centUnit,false);
  }
  else
  {
    displayNoGPS();
    delay (2000);
  }
  
}

void loop()
{
  // If the GPS data is OK, then display it. If not display "no GPS"
  while (Serial.available() > 0)
    if (gps.encode(Serial.read())){
      displayTime();
    }
  if (millis() > timer+1000 ) // detects if the serial comms has failed.
  {
    displayNoSerial();
  }
  


  
}

/*
 * Copyright (c) 2015 Andrew Doswell
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHOR(S) OR COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
