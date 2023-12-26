#include <math.h>
#include <AccelStepper.h>

#define stepPinM1 37
#define dirPinM1 43
#define stepPinM2 23
#define dirPinM2 31
#define stepPinM3 5
#define dirPinM3 7


// Create an AccelStepper object
AccelStepper motor1(AccelStepper::DRIVER, stepPinM1, dirPinM1);
AccelStepper motor2(AccelStepper::DRIVER, stepPinM2, dirPinM2);
AccelStepper motor3(AccelStepper::DRIVER, stepPinM3, dirPinM3);

void setup() {

  motor1.setMaxSpeed(2800.0);
  motor1.setAcceleration(2000.0);

  // Set the initial position to 0
  motor1.setCurrentPosition(0);

  motor2.setMaxSpeed(65.0);
  motor2.setAcceleration(30.0);

  // Set the initial position to 0
  motor2.setCurrentPosition(0);

  motor3.setMaxSpeed(4000.0);
  motor3.setAcceleration(3000.0);

  // Set the initial position to 0
  motor3.setCurrentPosition(0);

  motor4.setMaxSpeed(5000.0);
  motor4.setAcceleration(2500.0);

  // Set the initial position to 0
  motor4.setCurrentPosition(0);

  pinMode(dirPinM1, OUTPUT);
  pinMode(dirPinM2, OUTPUT);
  pinMode(dirPinM3, OUTPUT);
    

    Serial.begin(57600);
}



void loop() {
