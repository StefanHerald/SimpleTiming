/*
A simple program demonstrating all the different functions of SimpleTiming.
*/

#include <SimpleTiming.h>

//on board LED
const int boardLed = LED_BUILTIN;
bool state = LOW;

//LED's connected to PWM. Remember that the bytes will overflow,
const int yellowLed = 5; 
const int redLed    = 6; 
const int greenLed  = 3; 
byte yellow;
byte red;
byte green;

//button to control onBoardLed
const int buttonPin = 8;
bool buttonState = HIGH;
bool casingButton = false;

//id of red Timer.
byte id = 0;

//create an instance of the SimpleTiming class called allTimers
SimpleTiming allTimers;

void setup() 
{
  //set pin modes
  pinMode(boardLed  ,OUTPUT);
  pinMode(yellowLed ,OUTPUT);
  pinMode(redLed    ,OUTPUT);
  pinMode(greenLed  ,OUTPUT);
  pinMode(buttonPin ,INPUT);  

  Serial.begin(9600);
  while(!Serial){} //wait for serial

  reset();
}

void loop() 
{
  allTimers.tick();
  analogWrite(redLed    ,red);
  analogWrite(yellowLed ,yellow);
  analogWrite(greenLed  ,green);
  if(!casingButton){
    checkButton();
  }
}

void useless(){
  return;
}

void increaseLumRed()
{
  red += 5;
}

void increaseLumYellow()
{
  yellow += 5;
}

void increaseLumGreen()
{
  green += 5;
}

void removeRed(){
  allTimers.removeTimer(id);
  red = 0;
}

void reset(){
  //add timers and store their respective ID's
  red    = 0;
  yellow = 0;
  green  = 0;
  id = allTimers.addTimer(1000, 255,increaseLumRed, blink); //will run indefinitely
  allTimers.addTimer(1000, 15, increaseLumYellow, blink); //will run for 15 secondes
  allTimers.addTimer(1534, 15, increaseLumGreen, blink); //will run for 23.01 seconds
  allTimers.addTimer(10000, 1, removeRed, useless); //remove first timer after 10 seconds
  allTimers.addTimer(30000, 1, reset, useless); //reset everything after 30 seconds
  Serial.println("reset!");
}

void checkButton(){
  bool newButtonState = digitalRead(buttonPin);
  if (buttonState != newButtonState){
    casingButton = true;
    allTimers.addTimer(6, 1, caseButton, useless);
  }
}

void caseButton(){
  bool newButtonState = digitalRead(buttonPin);
  if (buttonState != newButtonState){
    blink();
    buttonState = newButtonState;
  }
  casingButton = false;
}

void blink()
{
  state = !state;
  digitalWrite(boardLed,state);
}
