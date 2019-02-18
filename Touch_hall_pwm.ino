
/*This program is to demonstrate touch and hall sensor of ESP32.
  It uses built-in LED of ESP32 connected to PIN-2(this may arry from board to board) to show values using PWM signals.
  By touching pin D4 or bring a magnet close to ESP32 chip you will see brightness of the built-in LED will change.
  
   link to libraries used
   https://github.com/Megunolink/MLP
   
   created by-Rohan Kumar
   date-18/2/2019

*/

const int ledPin = 2; //defining LED pin
const int freq = 5000;//defining FREQUENCY for PWM
const int ledChannel = 0;//defining CHANNEL for PWM
const int resolution = 8;//defining RESOLUTION in bits (0-15) for PWM

int s = 0; //defining variables for calculation
int s1 = 0; //defining variables for calculation
int touchval = 0; //defining variables for calculation
int hallval = 0; //defining variables for calculation

#include "MegunoLink.h"//including filtering libraries
#include "Filter.h" //including filtering libraries

ExponentialFilter<long> ADCFilter0(2, 0);// Create a new exponential filter with a weight of X and an initial value of Y.
ExponentialFilter<long> ADCFilter1(70, 0);// Create a new exponential filter with a weight of X and an initial value of Y.
ExponentialFilter<long> ADCFilter2(10, 0);// Create a new exponential filter with a weight of X and an initial value of Y.

void setup()
{
  Serial.begin(115200);//initialising serial monitor at 115200 baud

  ledcSetup(ledChannel, freq, resolution);//setting up (ledChannel, freq, resolution) for PWM
  ledcAttachPin(ledPin, ledChannel);// led pin and led
}

void loop()
{
  calculate(touchRead(T0), hallRead()); //giving sensors reading to calculate function

  ledcWrite(ledChannel, s1);//writing values to LED
}

void calculate(int touch, int hall) //procesing sensor reading to reduce noise and give final data
{
  ADCFilter1.Filter(touch);
  ADCFilter0.Filter(hall);
  touchval = 91 - ADCFilter1.Current();
  hallval = ADCFilter0.Current() - 28;
  s = 20 + touchval * 2 + hallval * 2;
  ADCFilter2.Filter(s);
  s1 = ADCFilter2.Current();
}

