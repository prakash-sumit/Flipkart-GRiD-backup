#include <AccelStepper.h>
#include <ros.h>
#include <std_msgs/Int64.h>

ros::NodeHandle nh;
int yaw_message = 99;

const int stepPin = 2;   // Adjust these pin numbers based on your setup
const int dirPin = 4;

AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

void yaw_callback(const std_msgs::Int64& msg) {
  yaw_message = msg.data;
  stepper.moveTo(200);
}

ros::Subscriber<std_msgs::Int64> yaw("/yaw_confirm", &yaw_callback);

void setup() {
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);

  stepper.setMaxSpeed(33.33);  // Set maximum speed to achieve 50 degrees in 1 second
  stepper.setAcceleration(100000);  // Set acceleration to achieve desired speed
  nh.initNode();
  nh.subscribe(yaw);
  Serial.begin(57600);
}

void loop() {
  nh.spinOnce();

  // Move the stepper only when the specific condition is met
  if (yaw_message == 100) {
    stepper.run();
    // Your additional code here
  }

  // Check if the yaw_message is 1
//   if (yaw_message == 1) {
//     // Get the current position of the stepper motor
//     int currentPosition = stepper.currentPosition();

//     // Calculate the new target position 20 degrees backward
//     int targetPosition = currentPosition - 11;

//     // Set the new target position
//     stepper.moveTo(targetPosition);

//     while(stepper.distanceToGo() != 0) {
//         stepper.run() ;
//     }
//     // Reset the yaw_message to avoid continuous movement
//     yaw_message = 0;
//   }


  if (yaw_message == 10){
  stepper.moveTo(0);
  while (stepper.distanceToGo() != 0) {
      stepper.run();
    }
  yaw_message = 0 ;
  }

  if (yaw_message == 1503 ) {
    digitalWrite(10, HIGH);
    delay(1000);
    digitalWrite(10, LOW);
  }
}
