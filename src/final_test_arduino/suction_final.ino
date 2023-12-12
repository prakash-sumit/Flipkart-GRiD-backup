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
//
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
  nh.loginfo("suction activated");
  
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
   nh.loginfo("suction deactivated");
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
    task = 2;
}
}