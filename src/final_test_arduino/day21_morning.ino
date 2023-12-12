#include <ros.h>
#include <geometry_msgs/Point.h>
// #include <std_msgs/String.h>
#include <Servo.h>
#include <std_msgs/Int64.h>
// #include <std_msgs/Int64MultiArray.h>
#include <math.h>

ros::NodeHandle nh;

#define stepPinM1 51
#define dirPinM1 49
#define stepPinM2 25
#define dirPinM2 29
#define stepPinM3 24
#define dirPinM3 22

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
    myservo1.write(170);
    nh.initNode();
    nh.subscribe(angles);
    nh.advertise(confirmation);
    nh.advertise(suction);
    // nh.advertise(randomness);
    // nh.advertise(debugger);
    Serial.begin(57600);
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
 
if(counter_y==1) {
    nh.loginfo("y chal raha");
  if (current_steps_y < steps_y) {
  move_clockwise(stepPinM2,dirPinM2);
  current_steps_y++ ;
    delay(15);
  }
  
  else if (current_steps_y > steps_y) {
  move_anticlockwise(stepPinM2,dirPinM2);
  current_steps_y-- ;
    delay(15);
  }
  else {
nh.loginfo("y completed for task 1");
  counter_y = 0;
  counter_x=1;
    delay(1000);
  }
} 

if(counter_x==1) {
    nh.loginfo("x chal raha");
  if (current_steps_x < steps_x) {
  move_clockwise(stepPinM1,dirPinM1);
  current_steps_x++ ;
    delay(2);
  }
  
  else if (current_steps_x > steps_x) {
  move_anticlockwise(stepPinM1,dirPinM1);
  current_steps_x-- ;
    delay(2);
  }
  else{ 
  counter_x=0;
  counter_z=1;
    counter_servo = 1;
    delay(1000);
  }
}

  if(counter_servo ==1) {
        //servo back to 0
        for(int j=170 ; j >=70 ; j--) {
        myservo1.write (j);
        delay(5);
        }
    counter_servo = 100;
  }

if(counter_z==1) {
    nh.loginfo("z chal raha");
  if (current_steps_z < steps_z ) {
  move_clockwise(stepPinM3,dirPinM3);
  current_steps_z++ ;
    delay(20);
  }
  
  else if (current_steps_z > steps_z) {
  move_anticlockwise(stepPinM3,dirPinM3);
  current_steps_z-- ;
    delay(20);
  }
  else {
  counter_z=0;
  }
}
        
if(counter_z==0) {
    confirmation_msg.data = 1;
    confirmation.publish(&confirmation_msg);
    delay(2000) ;
    counter_z = 4; 
//        delay(100);  // for suction
    }
 }

else if (task == 4)
    {
    nh.loginfo(" task 4 me ghusa");
    
  if(counter_z == 4){ 

  if (current_steps_z > 0) {
  move_anticlockwise(stepPinM3,dirPinM3);
  current_steps_z-- ;
    nh.loginfo("z kam");
    delay(20);
  }
  
  else {
    counter_z = 88;
  counter_x=44;
    delay(1000) ;
    nh.loginfo(" z jana khatam");

  } 
  }

if (counter_x == 44) {
    if (current_steps_x > -1200) {
        move_anticlockwise(stepPinM1,dirPinM1);
        current_steps_x--;
        delay(2);
    }
    else {
        counter_y = 4 ;
        counter_x = 88;
    }

}

if (counter_y == 4 && counter_z==88) {  
     if (current_steps_y < tray_theta) {
  move_clockwise(stepPinM2,dirPinM2);
  current_steps_y++ ;
    delay(15) ;
  }
  
  else if (current_steps_y > tray_theta) {
  move_anticlockwise(stepPinM2,dirPinM2);
  current_steps_y-- ;
    delay(15) ;
  }
  else{
  counter_y=44;
  counter_x=4;
    delay(1000) ;
  }
  }

if(counter_x ==4 && counter_y==44) {
    // nh.loginfo("galat jagah ghus gaya bc 2");
     if (current_steps_x < tray_r) {
  move_clockwise(stepPinM1,dirPinM1);
  current_steps_x++ ;
    delay(2) ;
  }
  else if (current_steps_x > tray_r) {
  move_anticlockwise(stepPinM1,dirPinM1);
  current_steps_x-- ;
    delay(2) ;
  }
  else{
  counter_x=102;
  counter_z = 44;
    delay(1000);
  }
  }
      
if (counter_z==44) {
     nh.loginfo("z goes to tray_z");
  
    if(current_steps_z  < tray_z) {
     move_clockwise(stepPinM3,dirPinM3);
     current_steps_z++;
     delay(20) ;
     }
     else{
     counter_z = 11;                  //11 is for suction     
     }
 }     
       
        
if(counter_z ==11) {
    // deactivate suction
    // nh.loginfo("galat jagah ghus gaya bc4");
       // add suction publisher

        // suction_msg.data = 1;
        // suction.publish(&suction_msg);
//        delay(1000);
        counter_z = 10;                   //10 is the counter to make every motor back to initial position
}
 

if (counter_z==10) {
    nh.loginfo("z comes back from tray_z ");
  
    if(current_steps_z  > 0) {
     move_anticlockwise(stepPinM3,dirPinM3);
     current_steps_z--;
     delay(20) ;
     }
     else{
     counter_z = 15;                    //15 means reached initial head top
     counter_x = 10;
     delay(1000) ;
     }
}
 
if (counter_x == 10) {
    // nh.loginfo("galat jagah ghus gaya bc6");
//  if (current_steps_x > 0) {                         // x brings to initial position
//   move_anticlockwise(stepPinM1,dirPinM1);
//   current_steps_x-- ;
//     delay(20) ;
//   }

   if (current_steps_x > -600) {                         // x brings to initial position
  move_anticlockwise(stepPinM1,dirPinM1);
  current_steps_x-- ;
    delay(2) ;
  }
  
  else{
  counter_x=15;
  counter_y = 10;
    delay(1000) ;
  }
}

if (counter_y ==10) {
    // nh.loginfo("galat jagah ghus gaya bc7");
    if (current_steps_y > 0) {
  move_anticlockwise(stepPinM2,dirPinM2);
  current_steps_y-- ;
    delay(15) ;
  }
        else{
  counter_y=15;
    delay(1000) ;
  }
  }
if (counter_y==15) {
    // nh.loginfo("galat jagah ghus gaya bc8");
    if(current_steps_x <=0) {
        move_clockwise(stepPinM1,dirPinM1);
        current_steps_x++;
        delay(2);

    }
    else{
        counter_x = 696 ; 
    }
}

if (counter_x == 696) {
    confirmation_msg.data = 1;
    confirmation.publish(&confirmation_msg);
    delay(2000);
    counter_y = 1;
    counter_x = 6969;
}
}
}  
//  }
    