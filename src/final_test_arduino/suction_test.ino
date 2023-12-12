// #include <Servo.h>

// Servo suckservo1;
// Servo suckservo2;
// Servo suckservo3;
// Servo suckservo4;

// void setup() {
// suckservo1.attach(38);       //bigmotor 1 
// suckservo2.attach(52);
 
// suckservo3.attach(23);       //bigmotor 2 
// suckservo4.attach(8);

// //suckservo2.write(0);
// //suckservo1.write(0);
// //suckservo3.write(0);
// //suckservo4.write(0);
// }

// void loop() {
 
//  suckservo1.write (180) ; 
//  suckservo2.write (0) ;  
//  suckservo3.write (180) ;
//  suckservo4.write (0) ;  
// //delay(3000) ;
// //
// suckservo1.write (0) ; 
// suckservo2.write (0) ;  
// suckservo3.write (0) ;
// suckservo4.write (0) ; 
// //delay(3000); 


// // suckservo1.write (0) ; 
// // suckservo2.write (180) ;  
// // suckservo3.write (0) ; 
// // suckservo4.write (180) ;  
// // delay(3000);

// }

#include <Servo.h>

Servo suckservo3;
Servo suckservo4;
int counter = 0;
// Servo suckservo3;
// Servo suckservo4;

void setup() {
suckservo3.attach(2);       //bigmotor 1 
suckservo4.attach(6);
 
// suckservo3.attach(23);       //bigmotor 2 
// suckservo4.attach(8);

// suckservo1.write (0) ; 
// suckservo3.write (0) ;
suckservo3.write (0) ;  
suckservo4.write (0) ; 
}

void loop() {
 if(counter<5){
 suckservo3.write (180) ; 
 delay(5000);
 suckservo4.write(180);
    delay(1000);
 counter+=1;
//  suckservo3.write (180) ;
 }
 suckservo3.write(0);
 suckservo4.write(0);

}