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
//std_msgs::Int64 suction_msg;
// std_msgs::Int64MultiArray random_debug;
// std_msgs::Int64 random_steps;
int motor_status [3] = {0,0,0} ;

int current_steps_x = 0;
int current_steps_y = 0;
int current_steps_z = 0;

// int counter = 68;

int tray_r = 980;
int tray_theta= 50;
int tray_z = 1537 ;

int start_r = 2470;

int steps_x;
int steps_y;
int steps_z ,task;


ros::Publisher confirmation ("/info_from_arduino", &confirmation_msg); //publish on completion of task
//ros::Publisher suction ("/task_for_suction", &suction_msg); //publish on completion of task

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

 combined_msg = 0;
   
   if (task == 1){

        run_motor_2(steps_y);
        run_motor_1(steps_x);

        //servo back to 0
        for(int j=170 ; j >=70 ; j--) {
        myservo1.write (j);
        delay(5);
        }

        run_motor_3(steps_z);
        delay(2000);

    }
 

    else if (task == 4)
    {
        run_motor_3(-(steps_z));
        run_motor_2(tray_theta-steps_y);
        run_motor_1(tray_r-steps_x);
        run_motor_3(tray_z);
        // deactivate suction

        // suckservo1.write (0) ; 
        // suckservo2.write (180) ;  
        // suckservo3.write (0) ;  
        // suckservo4.write (180) ; 

        // publish for suction
        
//        suction_msg.data = 1;
//        suction.publish(&suction_msg);

        delay(1000);
        //back to start position
        run_motor_3(-(tray_z));
        run_motor_1(-(tray_r));
        run_motor_2(-(tray_theta));
        delay(3000);

    }
    
    
    confirmation_msg.data = 1;
    confirmation.publish(&confirmation_msg);
//  }
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
  
  pinMode(stepPinM4, OUTPUT);
  pinMode(dirPinM4, OUTPUT);
// //sucktion 1 mechanism 
//    suckservo1.attach(10);
//    suckservo2.attach(9); //small1
//    suckservo2.write(0);
//    suckservo1.write(0);

//   suckservo3.attach(3);
//   suckservo4.attach(6); //small2
//   suckservo3.write(0);
//   suckservo4.write(0);
   
    
// //sucktion 2 mechanim
    myservo1.attach(7);
    myservo1.write(170);
    nh.initNode();
    nh.subscribe(angles);
    nh.advertise(confirmation);
//    nh.advertise(suction);
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

void run_motor_1(int steps_x){
 
  if(steps_x>0) {
    for(int i=1 ; i<=steps_x ; i++) {
    move_clockwise(stepPinM1,dirPinM1) ;
    current_steps_x++;
    delay(2); 
  }
  motor_status[0] = 1;  
  }
  else if(steps_x<0) {
    for(int i=1 ; i<=-(steps_x) ; i++) {
    move_anticlockwise(stepPinM1,dirPinM1) ;
    current_steps_x--;
    delay(2);
  }
   motor_status[0] = 1; 
  }

 else {
motor_status[0] = 1;
  }
}

void run_motor_2(int steps_y) {
  //motor2 code
{
  if(steps_y>0) {
    for(int i=1 ; i<=steps_y ; i++) {
    move_clockwise(stepPinM2,dirPinM2) ;
    current_steps_y++;
    delay(40);
    }
    motor_status[1] = 1;
  }
  else if(steps_y<0) {
    for(int i=1 ; i<=-(steps_y) ; i++) {
    move_anticlockwise(stepPinM2,dirPinM2) ;
    current_steps_y--;
    delay(40);
    } 
    motor_status[1] = 1; 
  }

 else {
motor_status[1] = 1;
  }
}

}

void run_motor_3(int steps_z)
{
  if(steps_z>0) {
    for(int i=1 ; i<=steps_z ; i++) {
    move_clockwise(stepPinM3,dirPinM3) ;
    current_steps_z++;
    delay(2);
    }  
    motor_status[2] = 1;
  }
  else if(steps_z<0) {
    for(int i=1 ; i<=-(steps_z) ; i++) {
    move_anticlockwise(stepPinM3,dirPinM3) ;
    current_steps_z--;
    delay(2);
    }
    motor_status[2] = 1;  
  }

    else {
motor_status[2] = 1;
  }
}


void loop() {
  while (nh.connected())
  {  
    nh.spinOnce();
    delay(10);}
//   suckservo1.write (180) ; 
//  suckservo3.write (180) ; 
//  delay() 
//  suckservo2.write(0);
//  suckservo4.write (0) ;
}