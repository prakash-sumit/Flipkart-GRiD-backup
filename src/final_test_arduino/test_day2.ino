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

int c = 22; //testing
  
std_msgs::Int64 str_msg; 
geometry_msgs::Point steps_msg;

Servo myservo1;  // neeche wali to rotate clockwise or anti 
Servo myservo2;

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
double curr_poss = 47.424;      // Your current position in cm
//double M_PI = 3.1415926;/
double theta_angle = 0;

ros::Publisher task_status ("/dm/info_from_controls", &str_msg); //publish to dm on completion of task
ros::Publisher stepspub("/dm/steps", &steps_msg); //publish to dm on completion of task


struct PolarCoordinates {
    double theta;
    double r;
};



PolarCoordinates calculatePolarCoordinates(double x_wrt_camera, double y_wrt_camera, double curr_pos) {
    PolarCoordinates polar_coords;

    theta_angle = atan2(y_wrt_camera, curr_pos + x_wrt_camera);
    polar_coords.theta = (180.0/3.1415926)*(theta_angle);  // Convert to degrees

    polar_coords.r = ((x_wrt_camera + curr_pos) / cos(theta_angle)) - curr_pos;
    return polar_coords;
}

void motortargetCallback(const geometry_msgs::Point& motor_target) {  //motor_angles.x should give the angle in degree !!
  
  x_wrt_camera = motor_target.x *1.00 ;
  y_wrt_camera = motor_target.y ;
  steps_z = floor((motor_target.z-20.32)/0.018) ;   // 20.32 is the length of end effector
 
 PolarCoordinates result = calculatePolarCoordinates(x_wrt_camera , y_wrt_camera , curr_poss); 
 steps_x = floor(result.r /0.0192) ;  // to move ahead.
 steps_y = floor(result.theta *17.78); //to move clockwise  

steps_msg.x = steps_x;
steps_msg.y = steps_y;
steps_msg.z = steps_z;
stepspub.publish(&steps_msg);

}


ros::Subscriber<std_msgs::Int64> tasklib("/task", &tasksCallback);  //to take info from dm about which task to perform
ros::Subscriber<geometry_msgs::Point> motortargetSub("/motor/target_coordinates", &motortargetCallback); //take angles from controlls code



void motor_1(double steps_x){
 
  if(steps_x>0) {
    for(int i=1 ; i<=steps_x ; i++) {
    move_clockwise(stepPinM1,dirPinM1) ;
    current_steps_x++;
    delay(10);
  }
  motor_status[0] = 1;  
  }
  else if(steps_x<0) {
    for(int i=1 ; i<=-(steps_x) ; i++) {
    move_anticlockwise(stepPinM1,dirPinM1) ;
    current_steps_x--;
    delay(10);
  }
   motor_status[0] = 1; 
  }

 else {
motor_status[0] = 1;
  }
}

void motor_2(double steps_y) {
  //motor2 code
{
  if(steps_y>0) {
    for(int i=1 ; i<=steps_y ; i++) {
    move_clockwise(stepPinM2,dirPinM2) ;
    current_steps_y++;
    delay(30);
    }
    motor_status[1] = 1;
  }
  else if(steps_y<0) {
    for(int i=1 ; i<=-(steps_y) ; i++) {
    move_anticlockwise(stepPinM2,dirPinM2) ;
    current_steps_y--;
    delay(30);
    } 
    motor_status[1] = 1; 
  }

 else {
motor_status[1] = 1;
  }
}

}

void motor_3(double steps_z)
{
  if(steps_z>0) {
    for(int i=1 ; i<=steps_z ; i++) {
    move_clockwise(stepPinM3,dirPinM3) ;
    current_steps_z++;
    delay(5);
    }  
    motor_status[2] = 1;
  }
  else if(steps_z<0) {
    for(int i=1 ; i<=-(steps_z) ; i++) {
    move_anticlockwise(stepPinM3,dirPinM3) ;
    current_steps_z--;
    delay(5);
    }
    motor_status[2] = 1;  
  }

    else {
motor_status[2] = 1;
  }
}


void tasksCallback(const std_msgs::Int64& msg) {
if (msg.data == 0) {
task_number = 0; 
str_msg.data =0;    
task_status.publish(&str_msg); 

}  //start msg

if (msg.data == 1) {
task_number = 1;
motor_2(steps_y);
motor_1(steps_x);

//servo back to 0
for(int j=90 ; j >=0 ; j--) {
  myservo1.write (j);
  delay(5);
}

motor_3(steps_z);

str_msg.data =1;    
task_status.publish(&str_msg);  

}  //start msg

if (msg.data == 2) {
task_number = 2;
}  // Backward_to_tray msg

if (msg.data == 3) {
task_number = 3;
}  // Backward_to_final


if (msg.data == 4) {
task_number = 4;
motor_3(-(steps_z));
motor_2(tray_theta-steps_y);
motor_1(tray_r-steps_x);
motor_3(tray_z);
 str_msg.data =2;    
task_status.publish(&str_msg); 

} 
}



void setup() {
  // Declare pins as output:
  pinMode(stepPinM1, OUTPUT);
  pinMode(dirPinM1, OUTPUT);

  pinMode(stepPinM2, OUTPUT);
  pinMode(dirPinM2, OUTPUT);

  pinMode(stepPinM3, OUTPUT);
  pinMode(dirPinM3, OUTPUT);
  
  myservo1.attach(2);
  myservo1.write(90);

  nh.initNode();
  nh.subscribe(motortargetSub);
  nh.subscribe(tasklib);
  nh.advertise(task_status, 5);
  nh.advertise(stepspub, 5);
  

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
}