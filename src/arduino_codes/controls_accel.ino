#include <AccelStepper.h>

// Define the pins for the stepper motor
#define STEP_PIN 2
#define DIR_PIN 4

// Create an AccelStepper object
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(300.0);
  stepper.setAcceleration(200.0);

  // Set the initial position to 0
  stepper.setCurrentPosition(0);

  // Set the direction pin as OUTPUT
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, HIGH);

  stepper.move(1000);

  // moveStepper(-200);
}

void loop() {
  if (stepper.distanceToGo() != 0) {
    stepper.run();
  }
}