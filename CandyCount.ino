/*
  Since it is getting closer to Halloween...
  
  A sketch to report the candy in a candy bowl using a load cell, INA125P instrumentation
  amplifier, MSP430F5529 LaunchPad, and 430BOOST-SHARP96 BoosterPack.  
  
  The load cell was taken from a defunct 1 Kg "postal" scale.
  
  To use it, start with an empty bowl on the load cell.  The raw analog value will be
  shown along with some directions. Push button "PUSH1" when the raw value has settled.
  Then add 10 pieces of candy to the bowl (or modify below with the variable candyCount2
  to your liking) and push button "PUSH2" to finish calibration when things have settled
  again.  The calibration data is then used with the map function to estimate how many
  pieces of candy there are. A rounding variable is used to address the "jitter" in the
  analog readings and the rounding that results from integer math.
  
  Frank Milburn October 2015
  Released into the public domain
*/

// Libraries
#include "SPI.h"
#include "OneMsTaskTimer.h"
#include "LCD_SharpBoosterPack_SPI.h"

// Pin declarations
const int inputPin = 23;        // analog input from INA125P instrument amplifier
const int sleepPin = 24;        // digital output pin used to put INA125P to sleep

// Global variables
int candyCount1 = 0;            // empty bowl
int analogCandyCount1;          // analog reading of an empty bowl obtained in calibrate()
int candyCount2 = 10;           // number of candies used to calibrate
int analogCandyCount2 =  0;     // analog reading of bowl with candyCount2 candies obtained in calibrate()
int rounding = 0;               // accounts for rounding - calculated in calibrate()

// Instantiation
LCD_SharpBoosterPack_SPI myScreen;

//---------------------------------------------------- setup() ----------------------------------------------
void setup()
{
  // SHARP96 BoosterPack
  myScreen.begin();   
  
  // INA125P
  pinMode(sleepPin, OUTPUT);                  // INA125P wake / sleep control
  digitalWrite(sleepPin, HIGH);               // make sure the INA125P is awake
  
  // buttons
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);
  
  // calibration of scale
  calibrate();
  
  // splash screen
  myScreen.setFont(1);
  myScreen.clear();
  myScreen.text(5, 10, "Candy");        
  myScreen.text(20, 30, "Count");
  myScreen.flush();  
}

//---------------------------------------------------- loop() ------------------------------------------------
void loop()  
{
  digitalWrite(sleepPin, HIGH);               // wake the INA125P up
  sleep(10);                                  // the INA125P needs some coffee, it is slow to get moving
                                              // but the microcontroller can snooze
  int analogValue = analogRead(inputPin);     // now that everybody is awake, read a value
  digitalWrite(sleepPin, LOW);                // put the INA125P  back to sleep
  
  // Calculate the candy in the bowl using the calibration data and linear interpolation / extrapolation
  int candyCount = map(analogValue + rounding, analogCandyCount1, analogCandyCount2, candyCount1, candyCount2);
  if (candyCount < 0)
  {
    candyCount = 0;
  }
  
  // Report out
  myScreen.setFont(1);
  myScreen.setCharXY(20, 55);
  myScreen.print("     ");                    // Clear the old reading
  myScreen.setCharXY(20, 55);
  myScreen.print(candyCount);                 // This is how much!
  myScreen.flush();
  
  sleep(500);                                 // done for a while, let the microcontroller sleep
}

//---------------------------------------------------- calibrate() ---------------------------------------------
void calibrate()
{    
  // Empty bowl for first data point in calibration
  myScreen.setFont(0);
  myScreen.text(7,  5, " Place empty");
  myScreen.text(7, 20, "bowl on scale");
  myScreen.text(7, 35, "  and push");
  myScreen.text(7, 50, "  button 1");
  myScreen.flush();
  int buttonState = HIGH;
  while(buttonState == HIGH)  // Wait for button 1 to be pushed
  {
    buttonState = digitalRead(PUSH1);
    analogCandyCount1 = analogRead(inputPin); // empty bowl reading
    myScreen.setCharXY(30, 65);
    myScreen.print(analogCandyCount1);
    myScreen.flush();
    delay(200);
  }                 
  
  // Use candyCount2 for second data point in calibration
  myScreen.setFont(0);
  myScreen.clear();
  myScreen.text(7,  5, " Place ");
  myScreen.print(candyCount2);
  myScreen.text(7, 20, "candies in bowl");
  myScreen.text(7, 35, "  and push");
  myScreen.text(7, 50, "  button 2");
  myScreen.flush();
  buttonState = HIGH;
  while(buttonState == HIGH)  // Wait for button 2 to be pushed
  {
    buttonState = digitalRead(PUSH2);
    analogCandyCount2 = analogRead(inputPin); // Second reading
    myScreen.setCharXY(30, 65);
    myScreen.print(analogCandyCount2);
    myScreen.flush();
    delay(200);
  } 
  
  // Variable to account for jitter and rounding
  rounding = (analogCandyCount2 - analogCandyCount1)/(candyCount2 - candyCount1)/2;
}
