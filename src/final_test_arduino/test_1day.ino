#include <ros.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/String.h>
#include <Servo.h>
#include <std_msgs/Int64.h>
#include <math.h>

ros::NodeHandle nh;

#define stepsPerRevolution 200

#define enPinM1 43
#define dirPinM1 47
#define stepPinM1 51

#define stepPinM2 25
#define dirPinM2 29
#define enPinM2 33

#define dirPinM3 4
#define stepPinM3 7
#define enPinM3 10

#define eps_x 10 // threshold
#define eps_y 10
#define eps_z 10

  
std_msgs::Int64 str_msg; 

Servo myservo1;  // neeche wali to rotate clockwise or anti 
Servo myservo2;

Servo myservo3; // for suction choti motor
Servo myservo4; // for suction badi motor

int pos_servo1 = 0;    // Variable to store servo1 position
int pos_servo2 = 0; 

int motor_status [3] = {0,0,0} ;

double steps_x=200000 ;
double steps_y=200000 ;
double steps_z=200000 ;

int tray_r = 980;
int tray_theta= 200;
int tray_z = 1537 ;

int task_number = 15;

int current_steps_x=0;
int current_steps_y=0;
int current_steps_z=0;

float Area_task=0;

double x_wrt_camera = 0.00; 
double y_wrt_camera = 0.00;  
double curr_poss = 47.424 ;      // Your current position in cm


struct PolarCoordinates {
    double theta;
    double r;
};

PolarCoordinates calculatePolarCoordinates(double x_wrt_camera, double y_wrt_camera, double curr_pos) {
    PolarCoordinates polar_coords;

    polar_coords.theta = atan2(y_wrt_camera, curr_pos + x_wrt_camera);
    polar_coords.theta = polar_coords.theta * 180.0 / M_PI;  // Convert to degrees

    polar_coords.r = ((x_wrt_camera + curr_pos) / cos(polar_coords.theta)) - curr_pos;

    return polar_coords;
}

void motortargetCallback(const geometry_msgs::Point& motor_target) {  //motor_angles.x should give the angle in degree !!
  
  x_wrt_camera = motor_target.x ;
  y_wrt_camera = motor_target.y ;
  steps_z = motor_target.z/0.018 ;

 curr_poss = curr_poss + current_steps_x * 0.0192;
 PolarCoordinates result = calculatePolarCoordinates(x_wrt_camera , y_wrt_camera , curr_poss); 
 steps_x = floor(result.r /0.0192) ;  // to move ahead.
 steps_y = floor(result.theta *17.78); //to move clockwise
  
}


void tasksCallback(const std_msgs::Int64& msg) {
if (msg.data == 0) {
task_number = 0;
}  //start msg

if (msg.data == 1) {
task_number = 1;
}  //start msg

if (msg.data == 2) {
task_number = 2;
}  // Backward_to_tray msg

if (msg.data == 3) {
task_number = 3;
}  // Backward_to_final


if (msg.data == 4) {
task_number = 4;
}  //start msg

}
geometry_msgs::Point steps_msg;
ros::Subscriber<std_msgs::Int64> tasklib("/task", tasksCallback);  //to take info from dm about which task to perform
ros::Subscriber<geometry_msgs::Point> motortargetSub("/motor/target_coordinates", &motortargetCallback,1); //take angles from controlls code
ros::Publisher task_status ("/dm/info_from_controls", &str_msg); //publish to dm on completion of task
ros::Publisher stepspub("/dm/steps", &steps_msg); //publish to dm on completion of task

void setup() {
  // Declare pins as output:
  pinMode(stepPinM1, OUTPUT);
  pinMode(dirPinM1, OUTPUT);

  pinMode(stepPinM2, OUTPUT);
  pinMode(dirPinM2, OUTPUT);

  pinMode(stepPinM3, OUTPUT);
  pinMode(dirPinM3, OUTPUT);

  nh.initNode();
  nh.subscribe(motortargetSub);
  nh.subscribe(tasklib);
  nh.advertise(task_status);
  nh.advertise(stepspub);
  

  // myservo1.attach(16); 
  // myservo2.attach(15);
  // myservo1.write(0); 
  // myservo2.write(0); 

  // myservo3.attach(17);  // choti wali motor of suction 
  // myservo4.attach(18);  // badi wali motor of suction
  // myservo3.write(0); 
  // myservo4.write(0); // attaches the servo on pin 9 to the servo object

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

steps_msg.x = steps_x;
steps_msg.y = steps_y;
steps_msg.z = steps_z;
stepspub.publish(&steps_msg);

if (task_number==0) {
   str_msg.data =0;    
  task_status.publish(&str_msg);     //5:Reached headtop 
}

else if (task_number==1) {

if(steps_x != 200000) 
   {

 if(motor_status[0] == 1 && motor_status[1] == 1 && motor_status[2] == 1 ) {
  str_msg.data =1;    
  task_status.publish(&str_msg);     //1:box_picked
}
    
//motor1 code
{
  if(current_steps_x < current_steps_x + steps_x ) {
    move_clockwise(stepPinM1,dirPinM1) ;
    current_steps_x++;
  }  

  else if (current_steps_x > current_steps_x + steps_x) {
  move_anticlockwise(stepPinM1,dirPinM1) ;
  current_steps_x--;
 }  
 
else if ( steps_x >= -(eps_x)  && steps_x <= eps_x)
{
  if(steps_x>0) {
    for (int i = 0; i < steps_x; i++)
    {
       move_clockwise(stepPinM1,dirPinM1) ;
       current_steps_x++;
       delay(100);
    }
  }
  else if(steps_x<0) {
    for (int i = 0; i < -(steps_x); i++)
    {
       move_anticlockwise(stepPinM1,dirPinM1) ;
       current_steps_x--;
       delay(100);
    }
  }
motor_status[0] = 1;
  }
}

//motor2 code
{ 
if (current_steps_y < current_steps_y + steps_y) {
   move_clockwise(stepPinM2,dirPinM2) ;        // motor2 goes anti when given clockwise
current_steps_y++;
delay(10);
  }  

else if (current_steps_y > current_steps_y + steps_y) {
 move_anticlockwise(stepPinM2,dirPinM2) ;
 current_steps_y--;
 delay(10);
 }  
else if ( steps_y >= -(eps_y)  && steps_y <= eps_y)
{
  if(steps_y>0) {
    for (int i = 0; i < steps_y; i++)
    {
       move_clockwise(stepPinM2,dirPinM2) ;
       current_steps_y++;
       delay(50);
    }
  }
  else if(steps_y<0) {
    for (int i = 0; i < -(steps_y); i++)
    {
       move_anticlockwise(stepPinM2,dirPinM2) ;
       current_steps_y--;
       delay(50);
    }
  }
motor_status[1] = 1;
  }
}
//motor3 code
{
  if (current_steps_z < current_steps_z + steps_z) {
   move_clockwise(stepPinM3,dirPinM3);
   current_steps_z++;
  }  
else if (current_steps_z > current_steps_z + steps_z) {
  move_anticlockwise(stepPinM3,dirPinM3);

   current_steps_z--;
 }  
 else if ( steps_z >= -(eps_z)  && steps_z <= eps_z)
{
  if(steps_z>0) {
    for (int i = 0; i < steps_z; i++)
    {
       move_clockwise(stepPinM3,dirPinM3) ;
       current_steps_z++;
       delay(50);
    }
  }
  else if(steps_z<0) {
    for (int i = 0; i < -(steps_z); i++)
    {
       move_anticlockwise(stepPinM3,dirPinM3) ;
       current_steps_z--;
       delay(50);
    }
  }
motor_status[2] = 1;
  }
}
//after all three motors are done 1 steps.
delay(10); 
   }
}


 //ends the forward function 

else if (task_number==2) {  //backward to tray   
  
   if(motor_status[0] == 2 && motor_status[1] == 2 && motor_status[2] == 2 ) {
    str_msg.data = 2;                           //"2:Reached_tray" ;
    task_status.publish(&str_msg);
   }

//motor1
  while (current_steps_z > tray_z) {           // reason to use while loop is so that hiegth kam ho pehle so no collision
 move_anticlockwise(stepPinM3,dirPinM3) ;
  current_steps_z--;
  
  if(current_steps_z== tray_z) {
  motor_status[2]=2;
  break;
 }
 }

while (current_steps_z < tray_z) {       // reason to use while loop is so that hiegth kam ho pehle so no collision
 
  move_anticlockwise(stepPinM3,dirPinM3) ;
  current_steps_z++;
  delay(100);
  if(current_steps_z== tray_z) {
  motor_status[2]=2;
  break;
 }
 }   

//motor2
 if (current_steps_y > tray_theta) {
  move_clockwise(stepPinM2,dirPinM2) ;
  current_steps_y--;
 }
 else if(current_steps_y > tray_theta) {
    move_anticlockwise(stepPinM2,dirPinM2) ;
  current_steps_y++;
 }
 else {
   motor_status[1] = 2;
 }


//motor1
if (current_steps_x > tray_r ) {
  move_anticlockwise(stepPinM1,dirPinM1) ;
  current_steps_x--;
}

else if (current_steps_x > tray_r ) {
  move_clockwise(stepPinM1,dirPinM1) ;
  current_steps_x++;
}
else {
 motor_status[0] = 2;
}
delay(10);
} 
}

//tray function is over 