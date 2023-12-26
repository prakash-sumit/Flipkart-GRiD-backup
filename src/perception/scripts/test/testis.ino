#include <Servo.h>

Servo myservo1;
Servo myservo2;

void setup() {

    myservo1.attach(11);
    myservo1.write(114);

}