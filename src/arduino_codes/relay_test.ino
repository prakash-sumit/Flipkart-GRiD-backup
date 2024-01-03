#include <AccelStepper.h>
#include <ros.h>
#include <std_msgs/Int64.h>

ros::NodeHandle nh;
int yaw_message = 99;

const int stepPin = 2;   // Adjust these pin numbers based on your setup
const int dirPin = 4;

std_msgs::Int64 bottom_barcode_msg;
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

ros::Publisher bottom ("/bottom_barcode_confirm", &bottom_barcode_msg); 

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
  nh.advertise(bottom);
  pinMode(dirPin, OUTPUT);
  digitalWrite(dirPin, HIGH);
//   stepper.setCurrentPosition(0);
  Serial.begin(57600);
}

void loop() {
  nh.spinOnce();

  if (yaw_message == 1503 ) {
    digitalWrite(10, HIGH);
    delay(1000);
    digitalWrite(10, LOW);
    delay(15);
    digitalWrite(10, HIGH);
    delay(1000);
    digitalWrite(10, LOW);
  }
digitalWrite (10, LOW)
//publish an arbitary value for aurdino-ros comunication
bottom_barcode_msg.data = 111;
bottom.publish(&bottom_barcode_msg);

}
