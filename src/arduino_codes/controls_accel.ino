#include <AccelStepper.h>


// Define the pins for the stepper motor
#define STEP_PIN 25
#define DIR_PIN 29

// Create an AccelStepper object
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  // Set the maximum speed and acceleration
  stepper.setMaxSpeed(65.0);
  stepper.setAcceleration(30.0);

  // Set the initial position to 0
  stepper.setCurrentPosition(200);

  // Set the direction pin as OUTPUT
  pinMode(DIR_PIN, OUTPUT);

  // moveStepper(-200);
}

void loop() {
  // Move forward 500 steps
  

  // delay(1000); // Delay for 1 second

  // Move backward 500 steps
  moveStepper(400);

  delay(1000); // Delay for 1 second

  moveStepper(-400);

  delay(1000); // Delay for 1 second

}

void moveStepper(long steps) {
  // Set the direction based on the sign of steps
  if (steps > 0) {
    digitalWrite(DIR_PIN, HIGH); // Set direction forward
  } else {
    digitalWrite(DIR_PIN, LOW); // Set direction backward
  }

  // Move the stepper motor the specified number of steps
  stepper.move(steps);

  // Run the stepper motor until it reaches the target position
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }
}