
/*
  Stepper Motor Demonstration 1
  Stepper-Demo1.ino
  Demonstrates 28BYJ-48 Unipolar Stepper with ULN2003 Driver
  Uses Arduino Stepper Library

  DroneBot Workshop 2018
  https://dronebotworkshop.com
*/

//Include the Arduino Stepper Library
#include <Stepper.h>

// Define Constants

// Number of steps per internal motor revolution 
const float STEPS_PER_REV = 32; 

//  Amount of Gear Reduction
const float GEAR_RED = 64;

// Number of steps per geared output rotation
const float STEPS_PER_OUT_REV = STEPS_PER_REV * GEAR_RED;

// Define Variables

// Number of Steps Required
int StepsRequired;

// Number of steps to wind warp
int WIND_WARP_STEPS = 160;

// Number of steps to unwind weave
int UNWIND_WEAVE_STEPS = 160;

// Create Instance of Stepper Class
// Specify Pins used for motor coils
// The pins used are 8,9,10,11 
// 6 7 8 9
// Connected to ULN2003 Motor Driver In1, In2, In3, In4 
// Pins entered in sequence 1-3-2-4 for proper step sequencing

Stepper warp(STEPS_PER_REV, 9, 11, 10, 12);
Stepper weave(STEPS_PER_REV, 5, 7, 6, 8);
#define DIR_PIN 3
#define HEDDLE_STEP_PIN 4
#define SHUTTLE_STEP_PIN 2
int x; 
#define BAUD (9600)
#define HEDDLE_FRONT_STOP_PIN A0
#define HEDDLE_BACK_STOP_PIN A0
#define SHUTTLE_STOP_PIN A1
#define STEP_DLY 1
#define SHUTTLE_STEP_DLY 1
#define WARP_SPEED 800


int speed = 500;
void setup()
{
// Nothing  (Stepper Library sets pins as outputs)
  Serial.begin(BAUD);
  pinMode(HEDDLE_STEP_PIN,OUTPUT); // Heddlie Step
  pinMode(SHUTTLE_STEP_PIN,OUTPUT); // Shuttle Step
  pinMode(DIR_PIN,OUTPUT); // Dir
  
 //moveHeddleToFrontStop();
 //moveShuttleToLeftStop();
}

void loop()
{  
  moveHeddleToFrontStop();
  moveShuttleToLeftStop();
  stepShuttleRight(9500); // cw is to the left, ccw is to the right
  beatWeft();
  tighten();
  
  moveHeddleToBackStop();
  moveShuttleToLeftStop();
  tighten();
  beatWeft();
  tighten();
}


void stepHeddleBwd(int steps) {
  digitalWrite(DIR_PIN,HIGH); // ccw direction
  for (int i = 0; i < steps; i++) {
    digitalWrite(HEDDLE_STEP_PIN,HIGH); // Output high
    //delay(10); // Wait
    digitalWrite(HEDDLE_STEP_PIN,LOW); // Output low
    delay(STEP_DLY);
  }
}

void stepShuttleRight(int steps) {
  digitalWrite(DIR_PIN,HIGH); // ccw direction
  for (int i = 0; i < steps; i++) {
    digitalWrite(SHUTTLE_STEP_PIN,HIGH); // Output high
    //delay(10); // Wait
    digitalWrite(SHUTTLE_STEP_PIN,LOW); // Output low
    delay(SHUTTLE_STEP_DLY);
  }
}


void stepHeddleFwd(int steps) {
  digitalWrite(DIR_PIN,LOW); // cw direction
  for (int i = 0; i < steps; i++) {
    digitalWrite(HEDDLE_STEP_PIN,HIGH); // Output high
    //delay(10); // Wait
    digitalWrite(HEDDLE_STEP_PIN,LOW); // Output low
    delay(STEP_DLY);
  }
}

void stepShuttleLeft(int steps) {
  digitalWrite(DIR_PIN,LOW); // cw direction
  for (int i = 0; i < steps; i++) {
    digitalWrite(SHUTTLE_STEP_PIN,HIGH); // Output high
    //delay(10); // Wait
    digitalWrite(SHUTTLE_STEP_PIN,LOW); // Output low
    delay(SHUTTLE_STEP_DLY);
  }
}

void windWarp(int steps) {
  warp.setSpeed(WARP_SPEED);     
  warp.step(steps);
}

void unwindWarp(int steps) {
 
  warp.setSpeed(WARP_SPEED);    
  warp.step(-steps);
}

void unwindWeave(int steps) {
  
  weave.setSpeed(WARP_SPEED);     
  weave.step(steps);
}

void windWeave(int steps) {
  weave.setSpeed(WARP_SPEED);    
  weave.step(-steps);
}

void moveHeddleToFrontStop(void) {
  int limit_switch_value = analogRead(HEDDLE_FRONT_STOP_PIN);
   
  while (limit_switch_value > 100) {
    stepHeddleFwd(1);
    limit_switch_value = analogRead(HEDDLE_FRONT_STOP_PIN);
  }
  stepHeddleFwd(70);
}

void moveHeddleToBackStop(void) {
  moveHeddleToFrontStop();
  stepHeddleBwd(700);
}

void moveShuttleToLeftStop(void) {
  int stop_pin = analogRead(SHUTTLE_STOP_PIN);
   
  while (stop_pin > 100) {
    stepShuttleLeft(1);
    stop_pin = analogRead(SHUTTLE_STOP_PIN); 
  }
  //step_shuttle_cw(70);
}


void move_to_bwd_stop(void) {
  int ccw_value = analogRead(HEDDLE_BACK_STOP_PIN);
  
  while (ccw_value > 100) {
    stepHeddleBwd(1);
    ccw_value = analogRead(HEDDLE_BACK_STOP_PIN); 
  }
}

void moveHeddleToMiddle(void) {
  moveHeddleToFrontStop();
  stepHeddleBwd(240);
}

void beatWeft(void) {
  moveHeddleToMiddle();
  for (int i = 0; i < 20; i++) {
  windWarp(WIND_WARP_STEPS);
  unwindWeave(UNWIND_WEAVE_STEPS); 
  }
  for (int i = 0; i < 20; i++) {
  unwindWarp(WIND_WARP_STEPS);
  windWeave(UNWIND_WEAVE_STEPS); 
  }
}

void tighten(void) {
  for (int i=0; i < 50; i++) {
    windWarp(8);
    windWeave(8);
  }
}
