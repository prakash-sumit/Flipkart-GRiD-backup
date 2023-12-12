//task1 : x and z motors running together -> need to setup threshhold for z min when coming up in task 4
//till test3 stepsize(x,y,z) = (400,6400,400)
//can play around with delay of (z and x)

// theta motor delay is lesser in the code : delay(5)  -> not working 

//theta motor delay(15) used here but when returning to zero it is delay(5); working fine 
//tested for 5 loops. theta motor nahi hagta

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
std_msgs::Int64 suction_msg;
// std_msgs::Int64MultiArray random_debug;
// std_msgs::Int64 random_steps;
int motor_status [3] = {0,0,0} ;

int current_steps_x = 0;
int current_steps_y = 0;
int current_steps_z = 0;

int counter_x = 1;
int counter_y = 1;
int counter_z = 100;
int counter_servo = 100;
// int counter = 68;

// int tray_r = 980/2;
int tray_r = 0;
int tray_theta= 200;
// int tray_z = 1537/2 ;
int tray_z = 0 ;

int steps_x;
int steps_y;
int steps_z,task;

ros::Publisher confirmation ("/info_from_arduino", &confirmation_msg); //publish on completion of task
ros::Publisher suction ("/task_for_suction", &suction_msg); //publish on completion of task

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
    myservo1.write(135);                 //degree 30 is the downward position as per night of oct23 
    nh.initNode();
    nh.subscribe(angles);
    nh.advertise(suction);
    nh.advertise(confirmation);
    // nh.advertise(randomness);
    // nh.advertise(debugger);
    Serial.begin(57600);
}
void move_theta_anticlockwise (int stepPin , int dirPin) {
    digitalWrite(dirPin, HIGH);
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delay(8);
    digitalWrite(stepPin, LOW);
    delay(8);
}

void move_theta_clockwise(int stepPin , int dirPin) {

    digitalWrite(dirPin, LOW);
    // These four lines result in 1 step:
    digitalWrite(stepPin, HIGH);
    delay(8);
    digitalWrite(stepPin, LOW);
    delay(8);
    
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
    // delay(100);
    
if (task == 1){
nh.loginfo("task 1");
  
if(counter_y==1){
    nh.loginfo("y chal raha");
  if (current_steps_y < steps_y) {
  move_theta_clockwise(stepPinM2,dirPinM2);
  current_steps_y++ ;
    delay(25);
  }
  
  else if (current_steps_y > steps_y) {
  move_theta_anticlockwise(stepPinM2,dirPinM2);
  current_steps_y-- ;
    delay(25);
  }
  else {
nh.loginfo("y completed for task 1");
  counter_y = 0;
  delay(500);
  }
} 

if(counter_x==1) {
    nh.loginfo("x chal raha");
  if (current_steps_x < steps_x) {
  move_clockwise(stepPinM1,dirPinM1);
  current_steps_x++ ;
    delay(1);    // 5 to 2
  }
  else if (current_steps_x > steps_x) {
  move_anticlockwise(stepPinM1,dirPinM1);
  current_steps_x-- ;
    delay(1);
  }
  else{ 
  counter_x=0; 
  counter_servo = 1; 
  delay(100);
  }
}

if(counter_servo ==1 && counter_y ==0 && counter_x ==0) {
        //servo back to 0
        for(int j=135 ; j >=45 ; j--) {
        myservo1.write (j);
        delay(5);
        }
    counter_servo = 100;
    counter_z =1;
}

if(counter_z==1 && counter_x ==0 && counter_y==0) {
    nh.loginfo("z chal raha");
  if (current_steps_z < steps_z ) {
  move_clockwise(stepPinM3,dirPinM3);
  current_steps_z++ ;
    delay(1);    // 5 to 2
  }
  
  else if (current_steps_z > steps_z) {
  move_anticlockwise(stepPinM3,dirPinM3);
  current_steps_z-- ;
    delay(1);
  }
  else {
  counter_z=0;
  }
}
if(counter_z==0 && counter_x ==0) {
    confirmation_msg.data = 1;
    confirmation.publish(&confirmation_msg);
    delay(100) ;   // 2000 to 100
    counter_z = 4; 
    delay(1000) ;
    }
}

else if (task == 4){    
nh.loginfo(" task 4 me ghusa");

if(counter_z == 4 ){ 
    if (current_steps_z > 0) {
    move_anticlockwise(stepPinM3,dirPinM3);
    current_steps_z-- ;
    nh.loginfo("z kam");
    delay(1);
  }
  else {
    counter_z = 5;
    counter_x = 4;
    counter_y = 4;
  }
}

if(counter_x ==4) {

  if (current_steps_x < tray_r) {
  move_clockwise(stepPinM1,dirPinM1);
  current_steps_x++ ;
    delay(1) ;
  }
  else if (current_steps_x > tray_r) {
  move_anticlockwise(stepPinM1,dirPinM1);
  current_steps_x-- ;
    delay(1) ;
  }
   else{
     counter_x=6;
     delay(100);   // extra delay
  }
}

if (counter_y == 4) {  
  if (current_steps_y < tray_theta) {
  move_theta_clockwise(stepPinM2,dirPinM2);
  current_steps_y++ ;
  delay(25) ;
  }
  
  else if (current_steps_y > tray_theta) {
  move_theta_anticlockwise(stepPinM2,dirPinM2);
  current_steps_y-- ;
    delay(25) ;
  }
  else{
  counter_y= 6;
    delay(500) ;
  }
}

if (counter_x == 6 && counter_y ==6 && counter_z == 5) {  
  if(current_steps_z  < tray_z) {
     move_clockwise(stepPinM3,dirPinM3);
     current_steps_z++;
     delay(1) ;
  }
  
  else{ 
    counter_z = 6;
    delay(100);
  }
}

if(counter_z ==6 && counter_x == 6 && counter_y ==6) {
    // deactivate suction
    // nh.loginfo("galat jagah ghus gaya bc4");
       // add suction publisher
         suction_msg.data = 1;
         suction.publish(&suction_msg);
//        delay(1000);
        counter_z =8;
        counter_x =8; 
              //10 is the counter to make every motor back to initial position
        delay(500);  // extra delay
}

if (counter_z==8 ) {
    nh.loginfo("z comes back from tray_z ");
    if(current_steps_z  > 0) {
     move_anticlockwise(stepPinM3,dirPinM3);
     current_steps_z--;
     delay(1) ;
     }
     else{
     counter_z = 9;                    //15 means reached initial head top
     delay(100) ;
     }
}

if (counter_x ==8) {
   if (current_steps_x >= 0) {                         // x brings to initial position
  move_anticlockwise(stepPinM1,dirPinM1);
  current_steps_x-- ;
    delay(1) ;
  }
  
  else{
  counter_x = 9;
  counter_y =8;
  // servo back to 90 degree
      delay(100) ; 
  for(int j=45 ; j <=135 ; j++) {
  myservo1.write (j);
  delay(2);
  }
  }
}

if (counter_y==8 && counter_x ==9 && counter_z ==9) {
  if (current_steps_y > 0) {
  move_theta_anticlockwise(stepPinM2,dirPinM2);
  current_steps_y-- ;
  delay(25) ;
  }
  else{
  counter_y = 9;
  delay(500) ;
  }
}

if (counter_x == 9 && counter_y ==9 && counter_z ==9) {
    confirmation_msg.data = 1;
    confirmation.publish(&confirmation_msg);
    delay(100);   // 2000 to 100
    counter_y = 1;
    counter_x = 1;
}
}
confirmation_msg.data = 0;
confirmation.publish(&confirmation_msg);
}  