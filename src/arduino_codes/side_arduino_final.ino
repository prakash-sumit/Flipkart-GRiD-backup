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
  stepper.move(200);
}

ros::Subscriber<std_msgs::Int64> yaw("/yaw_confirm", &yaw_callback);

void setup() {
  pinMode(10, OUTPUT);        // pin10 relay for suction
  digitalWrite(10, LOW);
  stepper.setMaxSpeed(33.33);  // Set maximum speed to achieve 60 degrees in 1 second
  stepper.setAcceleration(10000);  // Set acceleration to achieve desired speed
  nh.initNode();
  nh.subscribe(yaw);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, HIGH);
//   stepper.setCurrentPosition(0);
  Serial.begin(57600);
}

void loop() {
  nh.spinOnce();

  // Move the stepper only when the specific condition is met
  if (yaw_message == 100) {
    // digitalWrite(dirPin, HIGH);
    stepper.run();
  }

  // Check if the yaw_message is 1
   if (yaw_message == 1) {
// digitalWrite(dirPin, LOW);
    stepper.move(-12);
    stepper.runToPosition();
     // Reset the yaw_message to avoid continuous movement
     yaw_message = 0;
   }

  if (yaw_message == 10){
// digitalWrite(dirPin, LOW);
 int curr_pos = stepper.currentPosition()
  stepper.move(-curr_pos);
  stepper.runToPosition();
  yaw_message = 0 ;
  }

  if (yaw_message == 1503 ) {
    digitalWrite(10, HIGH);
    delay(1000);
    digitalWrite(10, LOW);
  }
}
