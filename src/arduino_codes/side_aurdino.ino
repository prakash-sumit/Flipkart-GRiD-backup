#include <AccelStepper.h>
#include <ros.h>
#include <std_msgs/Int64.h>

ros::NodeHandle nh;

const int stepPin = 2;   //please change this
const int dirPin = 3;

const int stepsPerRevolution = 200;  // Number of steps per revolution (step size)
int yaw_message = 99 ; 

AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

void yaw_callback(const std_msgs::Int64& msg) {
yaw_message = msg.data;
}
ros::Subscriber<std_msgs::Int64> yaw("/yaw_task", &yaw_callback);  //to take info

void setup() {
    stepper.setMaxSpeed(stepsPerRevolution * 360 / 50);  // Set maximum speed to achieve 50 degrees in 1 second
    stepper.setAcceleration(stepsPerRevolution * 360 / 1);  // Set acceleration to achieve desired speed
    nh.initNode();
    nh.subscribe(yaw);
    Serial.begin(57600);
}

void loop() {
     nh.spinOnce();
    if (yaw_message != 99) {
    // Move to an absolute target position to achieve 50 degrees
    stepper.moveTo(stepsPerRevolution * 50 / 360);    
    // Move the motor to the target position
    stepper.run();
    }
// Your additional code here
}
