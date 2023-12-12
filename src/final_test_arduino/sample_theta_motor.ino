#include <ros.h>
#include <geometry_msgs/Point.h>
// #include <std_msgs/String.h>
#include <Servo.h>
#include <std_msgs/Int64.h>
// #include <std_msgs/Int64MultiArray.h>
#include <math.h>

ros::NodeHandle nh;

#define stepPinM1 47
#define dirPinM1 53
#define stepPinM2 25
#define dirPinM2 29
#define stepPinM3 4
#define dirPinM3 3

#define stepPinM4 41
#define dirPinM4 39

Servo myservo1;
Servo myservo2;

// Servo suckservo1;
// Servo suckservo2;
// Servo suckservo3;
// Servo suckservo4;

std_msgs::Int64 confirmation_msg;
// std_msgs::Int64MultiArray random_debug;
// std_msgs::Int64 random_steps;
int motor_status [3] = {0,0,0} ;

int current_steps_x = 0;
int current_steps_y = 0;
int current_steps_z = 0;

int counter_x = 100;
int counter_y = 1;
int counter_z = 100;
int counter_servo = 100;
// int counter = 68;

int tray_r = 980;
int tray_theta= 1600;
int tray_z = 1537 ;

int start_r = 2470;

int steps_x;
int steps_y;
int steps_z ,task;

ros::Publisher confirmation ("/info_from_arduino", &confirmation_msg); //publish on completion of task

// ros::Publisher randomness ("/okbro", &random_debug);
// ros::Publisher debugger ("/debugging", &random_steps);
void angles_callback(const std_msgs::Int64& msg) {

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
    
  // Implement your motor control logic for steps_x here
}

ros::Subscriber<std_msgs::Int64> angles("/motor/target_angles", &angles_callback);  //to take info

void setup() {
    // Initialize your stepper motors and servo here

  pinMode(stepPinM1, OUTPUT);
  pinMode(dirPinM1, OUTPUT);

  pinMode(stepPinM2, OUTPUT);
  pinMode(dirPinM2, OUTPUT);

  pinMode(stepPinM3, OUTPUT);
  pinMode(dirPinM3, OUTPUT);
  
// //sucktion 1 mechanism 
//    suckservo1.attach(52);
//    suckservo1.write(0);
//     suckservo2.attach(38);
//    suckservo2.write(0);

//     suckservo3.attach(0);
//    suckservo3.write(0);
//     suckservo4.attach(46);
//    suckservo4.write(0);
    
//sucktion 2 mechanim
    myservo1.attach(7);
    myservo1.write(90);
    nh.initNode();
    nh.subscribe(angles);
    nh.advertise(confirmation);
    // nh.advertise(randomness);
    // nh.advertise(debugger);
    Serial.begin(57600);
}


void move_anticlockwise (int stepPin , int dirPin) {
 digitalWrite(dirPin, HIGH);
    
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
}

void move_clockwise(int stepPin , int dirPin) {

    digitalWrite(dirPin, LOW);
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
    
}

void loop() {

for (int i=0; i<10; i++) {
  while (current_steps_y <= 400) {
  move_clockwise(stepPinM2,dirPinM2);
  current_steps_y++ ;
    delay(20);
  }
  delay(500);
  while (current_steps_y >= -400) {
  move_anticlockwise(stepPinM2,dirPinM2);
  current_steps_y-- ;
    delay(20);
  }
}
} 
