#include <ros.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Int64.h>
 //note if it works or not 

ros::NodeHandle nh;

#define stepsPerRevolution 200

#define dirPinM1 2
#define stepPinM1 5
#define enPinM1 8

#define dirPinM2 3
#define stepPinM2 6
#define enPinM2 9

#define dirPinM3 4
#define stepPinM3 7
#define enPinM3 10
  
std_msgs::Int64 str_msg; 
ros::Publisher task_status ("/dm/info_from_controls", &str_msg); //publish to dm on completion of task


int motor_status [3] = {0,0,0} ;
int steps_x ;
int steps_y ;
int steps_z ;

int current_steps_x=0;
int current_steps_y=0;
int current_steps_z=0;

int task_given = 15 ;

void motorAnglesCallback(const geometry_msgs::Point& motor_angles) {  //motor_angles.x should give the angle in degree !!
  steps_x = motor_angles.x;
  steps_y = motor_angles.y;
  steps_z = motor_angles.z;
}


void tasksCallback(const std_msgs::Int64& msg) {

if (msg.data == 1) {
   task_given = 1;
  } 
  else if (msg.data == 2) {
  task_given = 2;
    }
  else if (msg.data == 4) {
  task_given = 4;
  }
 else if (msg.data == 0) {
  task_given = 0;
}
}
ros::Subscriber<std_msgs::Int64> tasklib("/task", tasksCallback);  //to take info from dm about which task to perform
ros::Subscriber<geometry_msgs::Point> motorAnglesSub("/motor/angle_rotations", &motorAnglesCallback); //take angles from controlls code



void setup() {
  // Declare pins as output:
  pinMode(stepPinM1, OUTPUT);
  pinMode(dirPinM1, OUTPUT);

  pinMode(stepPinM2, OUTPUT);
  pinMode(dirPinM2, OUTPUT);

  pinMode(stepPinM3, OUTPUT);
  pinMode(dirPinM3, OUTPUT);

  nh.initNode();
  nh.subscribe(motorAnglesSub);
  nh.subscribe(tasklib);
  nh.advertise(task_status);

}

void loop() {
nh.spinOnce();
//
if (task_given == 0){
  str_msg.data =0;
  task_status.publish(&str_msg);
}


if (task_given == 1) {

 if(motor_status[0] == 1 && motor_status[1] == 1 && motor_status[2] == 1 ) {
  str_msg.data =1;
  task_status.publish(&str_msg);

 }

//motor1 code
{
  if(current_steps_x < current_steps_x + steps_x) {
   
    digitalWrite(dirPinM1, HIGH);
    
    // These four lines result in 1 step:
    digitalWrite(stepPinM1, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPinM1, LOW);
    delayMicroseconds(500);

    current_steps_x++;
  }  

  else if (current_steps_x > current_steps_x + steps_x) {
  
  // Set the spinning direction counterclockwise:
 digitalWrite(dirPinM1, LOW);
 
   // These four lines result in 1 step:
   digitalWrite(stepPinM1, HIGH);
   delayMicroseconds(500);
   digitalWrite(stepPinM1, LOW);
   delayMicroseconds(500);

   current_steps_x--;
 }  
 else {
motor_status[0] = 1;
}
}

//motor2 code
{ 
if (current_steps_y < current_steps_y + steps_y) {
   
   digitalWrite(dirPinM2, HIGH);
    // These four lines result in 1 step:
    digitalWrite(stepPinM2, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPinM2, LOW);
    delayMicroseconds(500);

    current_steps_y++;
  }  

else if (current_steps_y > current_steps_y + steps_y) {

  
  // Set the spinning direction counterclockwise:
  digitalWrite(dirPinM2, LOW);
 
   // These four lines result in 1 step:
   digitalWrite(stepPinM2, HIGH);
   delayMicroseconds(500);
   digitalWrite(stepPinM2, LOW);
   delayMicroseconds(500);
   current_steps_y--;
 }  
 else {
  motor_status[1] = 1;
}
}

//motor3 code
{
  if (current_steps_z < current_steps_z + steps_z) {
   
   digitalWrite(dirPinM3, HIGH);
   
    // These four lines result in 1 step:
    digitalWrite(stepPinM3, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPinM3, LOW);
    delayMicroseconds(500);

    current_steps_z++;
  }  
else if (current_steps_z > current_steps_z + steps_z) {
  // Set the spinning direction counterclockwise:
   digitalWrite(dirPinM3, LOW);
  
  // These four lines result in 1 step:
   digitalWrite(stepPinM3, HIGH);
   delayMicroseconds(500);
   digitalWrite(stepPinM3, LOW);
   delayMicroseconds(500);

   current_steps_z--;
 }  
 else {
  motor_status[2] = 1;
}
}

//after all three motors are done 1 steps.
delay(100); 

} //ends the forward function 

else if (task_given == 2) {  
  
   if(motor_status[0] == 2 && motor_status[1] == 2 && motor_status[2] == 2 ){
    str_msg.data =2 ;
    task_status.publish(&str_msg);
   }
  //assumed tray to be (0.0.0)

  while (current_steps_z > 0) {           // reason to use while loop is so that hiegth kam ho pehle so no collision
  // Set the spinning direction counterclockwise:
   digitalWrite(dirPinM1, LOW);
  
  // These four lines result in 1 step:
   digitalWrite(stepPinM1, HIGH);
   delayMicroseconds(500);
   digitalWrite(stepPinM1, LOW);
   delayMicroseconds(500);

   current_steps_z--;
  if(current_steps_z==0) {
  motor_status[2]=2;
 }

 }

 if (current_steps_y > 0) {
  // Set the spinning direction counterclockwise:
  digitalWrite(dirPinM2, LOW);
 
   // These four lines result in 1 step:
   digitalWrite(stepPinM2, HIGH);
   delayMicroseconds(500);
   digitalWrite(stepPinM2, LOW);
   delayMicroseconds(500);
   current_steps_y--;
 }
 else {
   motor_status[1] = 2;
 }

if (current_steps_x > 0) {
  
  // Set the spinning direction counterclockwise:
   digitalWrite(dirPinM3, LOW);
  
  // These four lines result in 1 step:
   digitalWrite(stepPinM3, HIGH);
   delayMicroseconds(500);
   digitalWrite(stepPinM3, LOW);
   delayMicroseconds(500);

   current_steps_x--;
}
else {
 motor_status[0] = 2;
}
} //tray function is over 

else if (task_given == 4) {
// below is the function for task3 i.e. backward to final position
// else if (task_name == "backward_to_tray") {    //assumed headtop to be (50,50,50)

  steps_x=50;
  steps_y=50;
  steps_z=50;

  if(motor_status[0] == 3 && motor_status[1] == 3 && motor_status[2] == 3 ) {
    str_msg.data =0;
  task_status.publish(&str_msg);
  }
  
  if(current_steps_x <  steps_x) {
   
    digitalWrite(dirPinM1, HIGH);
    
    // These four lines result in 1 step:
    digitalWrite(stepPinM1, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPinM1, LOW);
    delayMicroseconds(500);

    current_steps_x++;
  }  

  else if (current_steps_x >  steps_x) {
  
  // Set the spinning direction counterclockwise:
 digitalWrite(dirPinM1, LOW);
 
   // These four lines result in 1 step:
   digitalWrite(stepPinM1, HIGH);
   delayMicroseconds(500);
   digitalWrite(stepPinM1, LOW);
   delayMicroseconds(500);

   current_steps_x--;
 }  
 else {
motor_status[0] = 3;
}
//motor2 code
if (current_steps_y < steps_y) {
   
   digitalWrite(dirPinM2, HIGH);
    // These four lines result in 1 step:
    digitalWrite(stepPinM2, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPinM2, LOW);
    delayMicroseconds(500);

    current_steps_y++;
  }  

else if (current_steps_y >  steps_y) {

  
  // Set the spinning direction counterclockwise:
  digitalWrite(dirPinM2, LOW);
 
   // These four lines result in 1 step:
   digitalWrite(stepPinM2, HIGH);
   delayMicroseconds(500);
   digitalWrite(stepPinM2, LOW);
   delayMicroseconds(500);
   current_steps_y--;
 }  
 else {
  motor_status[1] = 3;
}

//motor3 code
  if (current_steps_z <  steps_z) {
   
   digitalWrite(dirPinM3, HIGH);
   
    // These four lines result in 1 step:
    digitalWrite(stepPinM3, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPinM3, LOW);
    delayMicroseconds(500);

    current_steps_z++;
  }  
else if (current_steps_z >  steps_z) {
  // Set the spinning direction counterclockwise:
   digitalWrite(dirPinM3, LOW);
  
  // These four lines result in 1 step:
   digitalWrite(stepPinM3, HIGH);
   delayMicroseconds(500);
   digitalWrite(stepPinM3, LOW);
   delayMicroseconds(500);

   current_steps_z--;
 }  
 else {
  motor_status[2] = 3;
}
//after all three motors are done 1 steps.
delay(100); 

}
}
