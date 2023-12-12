#include <ros.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/String.h>
#include <Servo.h>
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

Servo myservo1;  // neeche wali to rotate clockwise or anti 
Servo myservo2;

int pos_servo1 = 0;    // Variable to store servo1 position
int pos_servo2 = 0; 

int motor_status [3] = {0,0,0} ;

int steps_x=20000 ;
int steps_y =20000 ;
int steps_z =20000;

int task_number = 15;

int current_steps_x=0;
int current_steps_y=0;
int current_steps_z=0;

void motorAnglesCallback(const geometry_msgs::Point& motor_angles) {  //motor_angles.x should give the angle in degree !!
  steps_x = motor_angles.x;
  steps_y = static_cast<int>(motor_angles.y);
  steps_z = static_cast<int>(motor_angles.z);
}
void tasksCallback(const std_msgs::Int64& msg) {
if (msg.data == 0) {
task_number = 0;
}  //start msg

if (msg.data == 1) {
task_number = 1;
}  //Forward

if (msg.data == 2) {
task_number = 2;
}  // Backward_to_tray msg

if (msg.data == 4) {
task_number = 4;
}  // Backward_to_final


if (msg.data == 5) {
task_number = 5;
}  //start msg



}
ros::Subscriber<std_msgs::Int64> tasklib("/task", tasksCallback);  //to take info from dm about which task to perform
ros::Subscriber<geometry_msgs::Point> motorAnglesSub("/motor/angle_rotations", &motorAnglesCallback); //take angles from controlls code

ros::Publisher task_status ("/dm/info_from_controls", &str_msg); //publish to dm on completion of task

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

void move_anticlockwise (int stepPin , int dirPin) {
 digitalWrite(dirPin, HIGH);
    
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
}

void move_clockwise(int stepPin , int dirPin) {

    digitalWrite(dirPin, LOW);
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    
}


void loop() {
nh.spinOnce();

if (task_number==0) {
  str_msg.data =0;    
  task_status.publish(&str_msg);     //0:Reached_trolley_top 
}

else if (task_number==1) {

if(steps_x != 20000) 
   {

  if(motor_status[0] == 1 && motor_status[1] == 1 && motor_status[2] == 1 ) {
  str_msg.data =1;    
  task_status.publish(&str_msg);     //1:box_picked
 }
    
//motor1 code
{
  if(current_steps_x < current_steps_x + steps_x) {
    move_anticlockwise(stepPinM1,dirPinM1) ;
    current_steps_x++;
  }  

  else if (current_steps_x > current_steps_x + steps_x) {
  move_clockwise(stepPinM1,dirPinM1) ;
  current_steps_x--;
 }  
 else {
motor_status[0] = 1;
}
}

//motor2 code
{ 
if (current_steps_y < current_steps_y + steps_y) {
   
   move_clockwise(stepPinM2,dirPinM2) ;

    current_steps_y++;
  }  

else if (current_steps_y > current_steps_y + steps_y) {
 move_anticlockwise(stepPinM2,dirPinM2) ;
 }  
 else {
  motor_status[1] = 1;
}
}

//motor3 code
{
  if (current_steps_z < current_steps_z + steps_z) {
   move_clockwise(stepPinM3,dirPinM3) ;
   current_steps_z++;
  }  
else if (current_steps_z > current_steps_z + steps_z) {
  move_anticlockwise(stepPinM3,dirPinM3) ;

   current_steps_z--;
 }  
 else {
  motor_status[2] = 1;
}
}

//after all three motors are done 1 steps.
delay(1); 

}

else {
    str_msg.data =4;    
  task_status.publish(&str_msg);  //4:No_target_position_from_cv_found
}
}//ends the forward function 


else if (task_number==2) {  //backward to tray   
  
   if(motor_status[0] == 2 && motor_status[1] == 2 && motor_status[2] == 2 ){
    str_msg.data = 2;                           //"Reached_tray" ;
    task_status.publish(&str_msg);
   }
  //assumed tray to be (0.0.0)

  while (current_steps_z > 0) {           // reason to use while loop is so that hiegth kam ho pehle so no collision
 
  move_anticlockwise(stepPinM3,dirPinM3) ;
  current_steps_z--;
  if(current_steps_z==0) {
  motor_status[2]=2;
  break;
 }
 }

 if (current_steps_y > 0) {
  move_anticlockwise(stepPinM2,dirPinM2) ;
  current_steps_y--;
 }
 else {
   motor_status[1] = 2;
 }

if (current_steps_x > 0) {
  move_clockwise(stepPinM1,dirPinM1) ;
  current_steps_x--;
}
else {
 motor_status[0] = 2;
}
} //tray function is over 




else if (task_number==4) {                           //Backward_to_final function


  steps_x=50;
  steps_y=50;
  steps_z=50;

  if(motor_status[0] == 3 && motor_status[1] == 3 && motor_status[2] == 3 ) {
    str_msg.data =0;
  task_status.publish(&str_msg);    // 0: "reached_trolley_top"
  }

  {
  if(current_steps_x <  steps_x) {
    move_anticlockwise(stepPinM1,dirPinM1) ;
    current_steps_x++;
  }  

  else if (current_steps_x >  steps_x) {
   move_clockwise(stepPinM1,dirPinM1) ;
   current_steps_x--;
 }  
 
 else {
motor_status[0] = 3;
}
}

//motor2 code
{

if (current_steps_y < steps_y) {
   move_clockwise(stepPinM2,dirPinM2) ;
   current_steps_y++;
  }  

else if (current_steps_y >  steps_y) {
 move_anticlockwise(stepPinM2,dirPinM2) ;
   current_steps_y--;
 }  
 else {
  motor_status[1] = 3;
}
}

//motor3 code
{
if (current_steps_z <  steps_z) {
    move_clockwise(stepPinM3,dirPinM3) ;
    current_steps_z++;
  }  

else if (current_steps_z >  steps_z) {
  move_anticlockwise(stepPinM3,dirPinM3);
  current_steps_z--;
}  
else {
  motor_status[2] = 3;
}
}
//after all three motors are done 1 steps.
delay(1); 

} //back to final function ends here



}   //code ends



// else if (strcmp(task_name, "Find_barcode") == 0) {
//   bool barcode_mila_kya = cvfunction() ;

// for (int i=0; i<3 ; i++) {
//   if(barcode_mila_kya == true) { 
//       break;
//   }
//   else {
//     for (pos_servo1 = 0; pos_servo1 <= 90; pos_servo1 ++) {
//     myservo1.write(pos_servo1);
//     delay(15);  // Adjust the delay for desired rotation speed
//   }
//   barcode_mila_kya = cvfunction() ;
//   }
// }
// if (barcode_mila_kya == true) {
//     for (pos_servo2 = 0; pos_servo2 <= 90; pos_servo2 ++) {
//     myservo2.write(pos_servo2);
//     delay(15); 
//     }
//     str_msg.data ="Barcode_found";
//     task_status.publish(&str_msg);
// }
// else {
//   //rotate servo2
// for (pos_servo2 = 0; pos_servo2 <= 90; pos_servo2 ++) {
//     myservo2.write(pos_servo2);
//     delay(15); 
//     }
//   //drop the box mechanism
//   // ......//
//   //bring up the z motor 
//   //.......//
//   //again detect and attach with the box
//   //......//
  

// }
