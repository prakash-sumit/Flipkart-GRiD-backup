//task1 : x and z motors running together -> need to setup threshhold for z min when coming up in task 4

#include <ros.h>
#include <geometry_msgs/Point.h>
#include <Servo.h>
#include <std_msgs/Int64.h>
#include <math.h>
#include <AccelStepper.h>

ros::NodeHandle nh;

#define stepPinM1 37
#define dirPinM1 43
#define stepPinM2 23
#define dirPinM2 31
#define stepPinM3 5
#define dirPinM3 7

#define stepPinM4 48
#define dirPinM4 39

// Create an AccelStepper object
AccelStepper motor1(AccelStepper::DRIVER, stepPinM1, dirPinM1);
AccelStepper motor2(AccelStepper::DRIVER, stepPinM2, dirPinM2);
AccelStepper motor3(AccelStepper::DRIVER, stepPinM3, dirPinM3);
AccelStepper motor4(AccelStepper::DRIVER, stepPinM4, dirPinM4);

Servo myservo1;
Servo myservo2;

std_msgs::Int64 confirmation_msg;
std_msgs::Int64 msg_for_yaw;

int counter_x = 1;
int counter_y = 1;
int counter_z = 100;
int counter_servo = 100;

int tray_r = 0;
int tray_theta= 155;
int tray_z = 0 ;    //for debugging
int yaw_r = -160;

int steps_x;
int steps_y;
int steps_z,task;
int yaw_msg = 100;

ros::Publisher confirmation ("/info_from_arduino", &confirmation_msg); //publish on completion of task
ros::Publisher yaw ("/yaw_task", &msg_for_yaw); 

void angles_callback(const std_msgs::Int64& msg) {
nh.loginfo("task aaya");

int64_t combined_msg = msg.data;

  steps_z = combined_msg % 100000;
  combined_msg /= 100000;
 
  if (steps_z /10000 ==1) {
    steps_z = -steps_z % 10000;
  }

  else if (steps_z /10000 == 0) {
    steps_z = steps_z % 10000;
  }

  steps_y = combined_msg % 100000;
  combined_msg /= 100000;
 
  if (steps_y /10000 ==1) {
    steps_y = -steps_y % 10000;
  }

  else if (steps_y /10000 == 0) {
    steps_y = steps_y % 10000;
  }

    steps_x = combined_msg % 100000;
  combined_msg /= 100000;
 
  if (steps_x /10000 ==1) {
    steps_x = -steps_x % 10000;
  }

  else if (steps_x /10000 == 0) {
    steps_x = steps_x % 10000;
  }
  
 task = combined_msg;

 if(task == 4){
    steps_x = tray_r - steps_x;
    steps_y = tray_theta - steps_y;
    steps_z = -(steps_z);
 }
 if(task==3){
  steps_x = yaw_r - steps_x;
  steps_y = tray_theta - steps_y;
  steps_z = -(steps_z);
 }

 if (steps_x > 0) {
    digitalWrite(dirPinM1, HIGH); // Set direction forward
  } else {
    digitalWrite(dirPinM1, LOW); // Set direction backward
  }
  
  if (steps_y > 0) {
    digitalWrite(dirPinM2, HIGH); // Set direction forward
  } else {
    digitalWrite(dirPinM2, LOW); // Set direction backward
  }

  if (steps_z > 0) {
    digitalWrite(dirPinM3, HIGH); // Set direction forward
  } else {
    digitalWrite(dirPinM3, LOW); // Set direction backward
  }

  motor1.move(steps_x);
  motor2.move(steps_y);
  motor3.move(steps_z);
  nh.loginfo("callback ke end me");    
}

void yaw_callback(const std_msgs::Int64& msg) {
yaw_msg = msg.data;
nh.loginfo("yaw callback run hua");
}

ros::Subscriber<std_msgs::Int64> angles("/motor/target_angles", &angles_callback);  //to take info
ros::Subscriber<std_msgs::Int64> confirmation_from_yaw("/yaw_confirm", &yaw_callback);  //to take info

void setup() {
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);

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
    
//sucktion 2 mechanim
    myservo1.attach(11);
    myservo1.write(0);                 //degree 110 is the downward position as per night of dec19th 
    nh.initNode();
    nh.subscribe(angles);
    nh.subscribe(confirmation_from_yaw);
    nh.advertise(yaw);
    nh.advertise(confirmation);
    Serial.begin(57600);
}



void loop() {

  delay(10000;)
  digitalWrite(10, HIGH);
}
  

     