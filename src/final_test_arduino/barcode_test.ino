// #include <ros.h>
// #include <geometry_msgs/Point.h>
// // #include <std_msgs/String.h>
// #include <Servo.h>
// #include <std_msgs/Int64.h>
// // #include <std_msgs/Int64MultiArray.h>
// #include <math.h>

// ros::NodeHandle nh;

// // Servo suckservo1;
// // Servo suckservo2;
// // Servo suckservo3;
// // Servo suckservo4;
// // int task= 2;
// bool conditionA = false; // Initialize the condition to false
// int iterations = 0;
// int maxIterations = 6; // Set the desired number of iterations
// int counter = 0;

// #define stepPinM1 51
// #define dirPinM1 49
// #define stepPinM2 25
// #define dirPinM2 29
// #define stepPinM3 24
// #define dirPinM3 22

// #define stepPinM4 41
// #define dirPinM4 39

// Servo myservo1;
// Servo myservo2;

// int tray_r = 980;
// int tray_theta= 1600;
// int tray_z = 1537 ;

// // Servo suckservo1;
// // Servo suckservo2;
// // Servo suckservo3;
// // Servo suckservo4;

// int current_steps_x = 0;

// void angles_callback(const std_msgs::Int64& msg) {

// int64_t my_job = msg.data;

// if(my_job == 1) {
//     for(int i =0; i<1 ; i++) {
//         move_clockwise(stepPinM4, dirPinM4); 
//     }
// }
// else {
//   digitalWrite(13, HIGH);
//   delay(50);
//   digitalWrite(13, LOW);
//   delay(50);
  
//   suckservo1.write(0);
//   suckservo1.detach();
//   suckservo3.write(0);
//   suckservo3.detach();
//   delay(10);
//   suckservo2.attach(9);
//   suckservo2.write(180);
//   suckservo4.attach(6);
//   suckservo4.write(180);
//     delay(100);
    
//    suckservo2.write(0);
//    suckservo4.write(0);
//    delay(10);
//    suckservo2.write(180);
//    suckservo4.write(180);
//     delay(100);
// }
// }
  
// ros::Subscriber<std_msgs::Int64> angles("/task_for_yaw_servo", &angles_callback);  //to take info

// void setup() {
//     // Initialize your stepper motors and servo here
//   Serial.begin(57600);

//   pinMode(stepPinM1, OUTPUT);
//   pinMode(dirPinM1, OUTPUT);

//   pinMode(stepPinM2, OUTPUT);
//   pinMode(dirPinM2, OUTPUT);

//   pinMode(stepPinM3, OUTPUT);
//   pinMode(dirPinM3, OUTPUT);

//   pinMode(stepPinM4, OUTPUT);
//   pinMode(dirPinM4, OUTPUT);

//   suckservo1.attach(10); //big_1
//   pinMode(13, OUTPUT);
//   suckservo2.attach(9); // small_1
//   suckservo2.write(0);

//   suckservo3.attach(3);  //big_1
//   suckservo4.attach(6); // small_1
//   suckservo4.write(0);

// //  nh.subscribe(tasklib);
//   // You can set initial positions if needed
//   suckservo3.write(0); 
//   suckservo2.detach();// Set Servo 1 to the middle position (90 degrees)
//   suckservo3.write(0); 
//   suckservo2.detach();
    
//     myservo1.attach(2);
//     myservo1.write(60);
//     nh.initNode();
//     nh.subscribe(angles);

//     // nh.advertise(randomness);
//     // nh.advertise(debugger);
    

// //for (int i= 0 ; i<= tray_z ; i++ ) {       
// //  move_clockwise(stepPinM3,dirPinM3) ;
// //  delay(2);
// //}
// //
// //for (int i= 0 ; i<= tray_r ; i++ ) {       
// //  move_clockwise(stepPinM1,dirPinM1) ;
// //  delay(100);
// //}
// }

// void move_anticlockwise (int stepPin , int dirPin) {
//  digitalWrite(dirPin, HIGH);
    
//     // These four lines result in 1 step:
//     digitalWrite(stepPin, HIGH);
//     delayMicroseconds(500);
//     digitalWrite(stepPin, LOW);
//     delayMicroseconds(500);
// }

// void move_clockwise(int stepPin , int dirPin) {

//     digitalWrite(dirPin, LOW);
//     // These four lines result in 1 step:
//     digitalWrite(stepPin, HIGH);
//     delayMicroseconds(500);
//     digitalWrite(stepPin, LOW);
//     delayMicroseconds(500);
    
// }


// void loop() {
//   suckservo2.detach();
//   suckservo4.detach();
  
//   if (counter == 0) {
//     suckservo1.attach(10);
//     suckservo1.write(180);
//     suckservo3.attach(3);
//     suckservo3.write(180);
//     delay(50); 
//     //Serial.println("inner");
//   }

//   else if (counter == 4) {
//   suckservo1.write(0);
//   suckservo1.detach();
//   suckservo3.write(0);
//   suckservo3.detach();
//   delay(30);
//   counter=0;
//   }

//   else {
//   suckservo1.write(0);
//   suckservo1.detach();
//   suckservo3.write(0);
//   suckservo3.detach();
//   delay(30);
//   counter++;
//   }
//  nh.spinOnce();


// }


#include <Servo.h>
#include <ros.h>
#include <std_msgs/Int64.h>

ros::NodeHandle nh;

Servo suckservo1;
Servo suckservo2;
Servo suckservo3;
Servo suckservo4;
int task= 2;
bool conditionA = false; // Initialize the condition to false
int iterations = 0;
int maxIterations = 6; // Set the desired number of iterations
int counter = 0;

void tasksCallback(const std_msgs::Int64& msg) {

 task = msg.data;
}


ros::Subscriber<std_msgs::Int64> tasklib("/suction", tasksCallback);

void setup() {

  Serial.begin(57600);
  suckservo1.attach(10); //big_1
  pinMode(13, OUTPUT);
  suckservo2.attach(9); // small_1
  suckservo2.write(0);

  suckservo3.attach(3);  //big_1
  suckservo4.attach(6); // small_1
  suckservo4.write(0);

 nh.subscribe(tasklib);
  // You can set initial positions if needed
  suckservo3.write(0); 
  suckservo2.detach();// Set Servo 1 to the middle position (90 degrees)
  suckservo3.write(0); 
  suckservo2.detach();
  
}
void loop() {
 nh.spinOnce() ;

if(task==2){
  suckservo2.detach();
  suckservo4.detach();
  
  if (counter == 0) {
    suckservo1.attach(10);
    suckservo1.write(180);
    suckservo3.attach(3);
    suckservo3.write(180);
    delay(50); 
    //Serial.println("inner");
  }

  else if (counter == 4) {
  suckservo1.write(0);
  suckservo1.detach();
  suckservo3.write(0);
  suckservo3.detach();
  delay(30);
  counter=0;
  }

  else {
  suckservo1.write(0);
  suckservo1.detach();
  suckservo3.write(0);
  suckservo3.detach();
  delay(30);
  counter++;
  }
  //Serial.println("outer");
}

else{
  digitalWrite(13, HIGH);
  delay(50);
  digitalWrite(13, LOW);
  delay(50);
  
  suckservo1.write(0);
  suckservo1.detach();
  suckservo3.write(0);
  suckservo3.detach();
  delay(10);
  suckservo2.attach(9);
  suckservo2.write(180);
  suckservo4.attach(6);
  suckservo4.write(180);
    delay(100);
    
   suckservo2.write(0);
   suckservo4.write(0);
   delay(10);
   suckservo2.write(180);
   suckservo4.write(180);
    delay(100);
}
}