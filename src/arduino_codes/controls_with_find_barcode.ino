//bottom barcode and also speed 
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
std_msgs::Int64 suck_drop;

int counter_x = 1;
int counter_y = 1;
int counter_z = 100;
int counter_servo = 100;

int tray_r = 0;
int tray_theta= 200;
int tray_z = 0 ;    //for debugging
int final_r = 0;
int final_theta= 200;
int final_z = 0 ;    //for debugging
int yaw_r = -160;

int steps_x;
int steps_y;
int steps_z,task;
int yaw_msg = 100;
int vansh_count = 2;
 
ros::Publisher confirmation ("/info_from_arduino", &confirmation_msg); //publish on completion of task
ros::Publisher yaw ("/yaw_task", &msg_for_yaw);
ros::Publisher suction ("/suction_drop", &suck_drop); 

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

 if(task == 4){
    steps_x = final_r - steps_x;
    steps_y = final_theta - steps_y;
    steps_z = final_z - steps_z;
 }
 if(task==3){
  steps_x = yaw_r - steps_x;
  steps_y = final_theta - steps_y;
  steps_z = final_z - steps_z;
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
//   //("callback ke end me");    
}

void yaw_callback(const std_msgs::Int64& msg) {
yaw_msg = msg.data;
// //("yaw callback run hua");
}

ros::Subscriber<std_msgs::Int64> angles("/motor/target_angles", &angles_callback);  //to take info
ros::Subscriber<std_msgs::Int64> confirmation_from_yaw("/yaw_confirm", &yaw_callback);  //to take info
//ros::Subscriber<std_msgs::Int64> confirmation_from_yaw("/yaw_confirm", &yaw_callback);  //to take info


void setup() {

  motor1.setMaxSpeed(10000.0);
  motor1.setAcceleration(4000.0);

  // Set the initial position to 0
  motor1.setCurrentPosition(0);

  motor2.setMaxSpeed(65.0);
  motor2.setAcceleration(30.0);

  // Set the initial position to 0
  motor2.setCurrentPosition(0);

  motor3.setMaxSpeed(6000.0);
  motor3.setAcceleration(4000.0);

  // Set the initial position to 0
  motor3.setCurrentPosition(0);

  pinMode(dirPinM1, OUTPUT);
  pinMode(dirPinM2, OUTPUT);
  pinMode(dirPinM3, OUTPUT);

//sucktion 2 mechanim
    myservo1.attach(11);
    myservo1.write(30);                 //degree 110 is the downward position as per night of dec19th    >> 20 to 150 as of jan2 17:37
    nh.initNode();
    nh.subscribe(angles);
    nh.subscribe(confirmation_from_yaw);
    nh.advertise(yaw);
    nh.advertise(confirmation);
    nh.advertise(suction);
    Serial.begin(57600);
}


void loop() {
  nh.spinOnce();

if (task == 2){
//("task2 chala ") ;
if(counter_y==1919){
  if (motor2.distanceToGo() != 0) {
    motor2.run();
  }
  else {
  counter_y = 0;
  //("y = 0 ");
  }
} 

if(counter_x==1919) {
  if (motor1.distanceToGo() != 0) {
  motor1.run();
  }
  else{ 
  counter_x=0; 
  counter_servo = 1; 
  //("counter_servo = 1");
  }
}

if(counter_servo ==1 && counter_y ==0 && counter_x ==0) {
        for(int j=30 ; j <=150 ; j++) {
        myservo1.write (j);
        delay(2);
        }
    counter_servo = 100;
    counter_z =1;
}

if(counter_z==1 && counter_x ==0 && counter_y==0) {
    // //("z chal raha");
  if (motor3.distanceToGo() != 0) {
  motor3.run();
  }
  else {
  counter_z=0;
  delay(100);
  }
}

if(counter_z==0 && counter_x ==0) {
  
  steps_x = -(steps_x);
  steps_y = -(steps_y);
  steps_z = 500 - steps_z;

    counter_z = 4;
    counter_x = 4;
    counter_y = 4;

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
    // delay(1000);
  }

if(counter_z == 4 ){ 
    if (motor3.distanceToGo() != 0) {
    motor3.run();
  // //(" z is moving to drop the bottom barcode box now");
  }
  else {
    counter_z =5;
  }
}

if(counter_x ==4) {

  if (motor1.distanceToGo() != 0) {
  motor1.run();
  // //(" x is moving to drop the bottom barcode box now");
  }
   else{
     counter_x=5;
    //  delay(100);   // extra delay
  }
}

if (counter_y == 4) {  
  if (motor2.distanceToGo() != 0) {
  motor2.run();
  // //(" y is moving to drop the bottom barcode box now");
  }
  else{
  counter_y= 5;
    // delay(500) ;
  }
}

if (counter_x ==5 && counter_y ==5 && counter_y ==5) {
  for(int j=150 ; j >=30 ; j--) {
        myservo1.write (j);
        delay(2);
        }
  delay(1000);
  // //(" servo rotated with delay");
  counter_x=6;
  counter_y=6;
  counter_z=6;
}


if(counter_z ==6 && counter_x == 6 && counter_y ==6) {

/////////////suction drop mechanism ////////////////////////////////
  suck_drop.data = 1503;   // 69 STARTS THE OTHER AURDINO AND ALSO THE BARCODE_YAW CODE
  suction.publish(&suck_drop);
  delay(1000);
  //(" box dropped with delay");
        counter_z =8;
        counter_x =8; 
              //10 is the counter to make every motor back to initial position

        steps_x = -(final_r);
        steps_y = -(final_theta);
        steps_z = -(500);
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

}

if (counter_z==8 ) {

    if(motor3.distanceToGo() != 0) {
     motor3.run();
     //(" z is comming back to head");
     }
     else{
     counter_z = 9;                    //15 means reached initial head top
    //  delay(100) ;
     }
}

if (counter_x ==8) {
   if (motor1.distanceToGo() != 0) {                         // x brings to initial position
  motor1.run();
  //(" x is comming back to head");
  }
  
  else{
  counter_x = 9;
  counter_y =8;
  }
}

if (counter_y==8 && counter_x ==9 && counter_z ==9) {
  if (motor2.distanceToGo() != 0) {
  motor2.run();
  //(" y is comming back to head");
  }
  else{
  counter_y = 9;
//   delay(500) ;
  }
}

if (counter_x == 9 && counter_y ==9 && counter_z ==9) {
    confirmation_msg.data = 1;
    confirmation.publish(&confirmation_msg);
    //("  reached head");
    // delay(100);   // 2000 to 100
    counter_y = 1;
    counter_x = 1;
}
}

///////////////////////////////////////////////////////////task 2 ends ////////////////////////////////////////////////////////////////////

else if (task == 1){

if(counter_y==1){
  if (motor2.distanceToGo() != 0) {
    motor2.run();
  }
  else {
  counter_y = 0;
  }
} 

if(counter_x==1) {
  if (motor1.distanceToGo() != 0) {
  motor1.run();
  }
  else{ 
  counter_x=0; 
  counter_servo = 1; 
  }
}

if(counter_servo ==1 && counter_y ==0 && counter_x ==0) {
        for(int j=30 ; j <=150 ; j++) {
        myservo1.write (j);
        delay(1);
        }
    counter_servo = 100;
    counter_z =1;
}

if(counter_z==1 && counter_x ==0 && counter_y==0) {
    // //("z chal raha");
  if (motor3.distanceToGo() != 0) {
  motor3.run();
  }
  else {
  counter_z=0;
  }
}
if(counter_z==0 && counter_x ==0) {
    confirmation_msg.data = 1;
    confirmation.publish(&confirmation_msg);
    // delay(100) ;   // 2000 to 100
    counter_z = 4; 
    // delay(1000) ;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

else if (task == 3){    
// //(" task 3 me ghusa");

if(counter_z == 4 ){ 
    if (motor3.distanceToGo() != 0) {
    motor3.run();
    // //("z kam");
  }
  else {                                          // MAKE SURE THERE IS NO INSTANCE WHERE TASK_3 RUNS DIRECTLY BEFORE TASK_4
    counter_z = 15;    
    counter_x = 14;
    counter_y = 14;
    digitalWrite(dirPinM3, HIGH);
}
}

if(counter_x ==14) {
  if (motor1.distanceToGo() != 0) {
  motor1.run();
  ////("x to yaw_r");
  }
   else{
     counter_x=99; //99 is random to make other counters in sync with task 4 function
     msg_for_yaw.data = 69;   // 69 STARTS THE OTHER AURDINO AND ALSO THE BARCODE_YAW CODE
     yaw.publish(&msg_for_yaw);

     motor1.move(final_r - yaw_r);
     if (final_r - yaw_r > 0) {
            digitalWrite(dirPinM1, HIGH); // Set direction forward
        } 
        else {
            digitalWrite(dirPinM1, LOW); // Set direction backward
        } 

  }
}

if (counter_y == 14) {  
  if (motor2.distanceToGo() != 0) {
  motor2.run();
  //("y chala");
  }
  else{
  counter_y= 16;     
    // delay(500) ;

  }
}

if(yaw_msg == 1) {    //this part is important // no change without vansh
    motor3.move(final_z);
    vansh_count = 88;
    //("vansh_count = 88");
    yaw_msg = 200;
  }
  if(yaw_msg == 77) {
    motor3.move(500);
    vansh_count = 89;
    //("vansh_count = 89");
    yaw_msg = 200;
  }

if (counter_y == 16 && counter_z == 15 && vansh_count == 88) {     //yaw_msg = 1 should be sent when you find barcode
  if(motor3.distanceToGo() != 0) {
     motor3.run();
  //("z chala after not bottom");   
  }
  
  else{ 
                           //99 is random to make other counters in sync with task 4 function
  delay(100);
  //("locha hogya");
  for(int j=150 ; j >=30 ; j--) {
  myservo1.write (j);
  delay(1);
  }
  delay(100);
  counter_z = 16;  
  }
}

if (counter_x == 99  && vansh_count == 88) {  //yaw_msg = 1 should be sent when you find barcode   
  if(motor1.distanceToGo() != 0) {     
     motor1.run();
     //("x chala after fb not bottom");
  }
  
  else{  
    counter_x = 16;
    // delay(100);
     //("x me locha hogya not bottom");
  }
}

if (counter_y == 16 && counter_z == 15 && vansh_count == 89) {     //yaw_msg = 1 should be sent when you find barcode
  if(motor3.distanceToGo() != 0) {
     motor3.run();
     //nh.loginfo("z chala after bottom ");
  }
  
  else{                            //99 is random to make other counters in sync with task 4 function
  delay(100);
  //("locha hogya");
  for(int j=150 ; j >=30 ; j--) {
  myservo1.write (j);
  delay(1);
  }
  delay(1000);
  counter_z = 16;  
  }
}

if (counter_x == 99 && vansh_count == 89) {  //yaw_msg = 1 should be sent when you find barcode
     //ASK VANSH 
  if(motor1.distanceToGo() != 0) {     
     motor1.run();
     //("x chala after bottom ");
  }
  
  else{  
    counter_x = 16;
    //("x me locha hogya");
  }
}

if (vansh_count == 89 && counter_z == 16 && counter_y == 16 && counter_x == 16) {
  suck_drop.data = 1503;   
  suction.publish(&suck_drop);
  delay(1000);

  counter_z = 1818;
        motor3.move(-500);
  
}

if(counter_z == 1818) {
    if(motor3.distanceToGo() != 0) {
     motor3.run();
     //("z is reaching again detecting plaace after dropping ");
     }
     else{
     counter_z = 1919;
     counter_y = 1919;
     counter_x = 1919;
     confirmation_msg.data = 5;       // 5 is for box is droped at bottom_barcode_tray and then z is brought to zero
     confirmation.publish(&confirmation_msg);   
     //("5 is published means now task2 should come");                
    //  delay(100) ;
     }
}


if(counter_z == 16 && counter_x == 16 && counter_y == 16 && vansh_count == 88) {


// delay(1000);
  suck_drop.data = 1503;   // 69 STARTS THE OTHER AURDINO AND ALSO THE BARCODE_YAW CODE
  suction.publish(&suck_drop);
  delay(100);
   ////////suction drop mechanism /////// 
        counter_z = 18;
        counter_x = 18;              
        steps_x = -(final_r);
        steps_y = -(final_theta);
        steps_z = -(final_z);
      
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

        // delay(500);  // extra delay
}


if (counter_z==18 ) {
    //("z comes back from tray_z ");
    if(motor3.distanceToGo() != 0) {
     motor3.run();
     }
     else{
     counter_z = 19;                    //15 means reached initial head top
    //  delay(100) ;
     }
}

if (counter_x ==18) {
  //("x comes back from tray_z ");
   if (motor1.distanceToGo() != 0) {                         // x brings to initial position
  motor1.run();
  }
  
  else{
  msg_for_yaw.data = 10;
  yaw.publish(&msg_for_yaw); 
  counter_x = 19;
  counter_y =18;
    //   delay(100) ;  
  }
  }


if (counter_y==18 && counter_x ==19 && counter_z ==19) {
  //("y comes back from tray_z ");
  if (motor2.distanceToGo() != 0) {
  motor2.run();
  }
  else{
  counter_y = 19;
//   delay(500) ;
  }
}

if (counter_x == 19 && counter_y ==19 && counter_z ==19) {
    confirmation_msg.data = 1;
    confirmation.publish(&confirmation_msg);
    // delay(100);   // 2000 to 100
    counter_y = 1;
    counter_x = 1;
}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
else if (task == 4){    
//(" task 4 me ghusa");

if(counter_z == 4 ){ 
    if (motor3.distanceToGo() != 0) {
    motor3.run();
    //("z kam");
  }
  else {
    counter_z = 5;
    counter_x = 4;
    counter_y = 4;
    digitalWrite(dirPinM3, HIGH);
    motor3.move(final_z);
  }
}

if(counter_x ==4) {

  if (motor1.distanceToGo() != 0) {
  motor1.run();
  }
   else{
     counter_x=6;
    //  delay(100);   // extra delay
  }
}

if (counter_y == 4) {  
  if (motor2.distanceToGo() != 0) {
  motor2.run();
  }
  else{
  counter_y= 6;
    // delay(500) ;
  }
}

if (counter_x == 6 && counter_y ==6 && counter_z == 5) {  
  if(motor3.distanceToGo() != 0) {
     motor3.run();
  }
  
  else{ 
    counter_z = 6;
    // delay(100);
  }
}

if(counter_z ==6 && counter_x == 6 && counter_y ==6) {

/////////////suction drop mechanism ////////////////////////////////
  suck_drop.data = 1503;   // 69 STARTS THE OTHER AURDINO AND ALSO THE BARCODE_YAW CODE
  suction.publish(&suck_drop);
  delay(2000);
        counter_z =8;
        counter_x =8; 
              //10 is the counter to make every motor back to initial position
        steps_x = -(final_r);
        steps_y = -(final_theta);
        steps_z = -(final_z);
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

        // delay(500);  // extra delay
}

if (counter_z==8 ) {
    //("z comes back from tray_z ");
    if(motor3.distanceToGo() != 0) {
     motor3.run();
     }
     else{
     counter_z = 9;                    //15 means reached initial head top
    //  delay(100) ;
     }
}

if (counter_x ==8) {
   if (motor1.distanceToGo() != 0) {                         // x brings to initial position
  motor1.run();
  }
  
  else{
  counter_x = 9;
  counter_y =8;
  // servo back to 90 degree
    //   delay(100) ; 
  for(int j=150 ; j >=30 ; j--) {
  myservo1.write (j);
  delay(2);
  }
  }
}

if (counter_y==8 && counter_x ==9 && counter_z ==9) {
  if (motor2.distanceToGo() != 0) {
  motor2.run();
  }
  else{
  counter_y = 9;
//   delay(500) ;
  }
}

if (counter_x == 9 && counter_y ==9 && counter_z ==9) {
    confirmation_msg.data = 1;
    confirmation.publish(&confirmation_msg);
    // delay(100);   // 2000 to 100
    counter_y = 1;
    counter_x = 1;
}
}
confirmation_msg.data = 0;
confirmation.publish(&confirmation_msg);
// msg_for_yaw.data = 16;
// yaw.publish(&msg_for_yaw);   // Call Sumit if wo continous publishing wala issue aata hai. DONT uncomment without calling WARNA FUCK HOJAYEGA
}  