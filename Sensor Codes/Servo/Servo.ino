#include <Servo.h>

Servo miservo; //It declares servo object
int angulo=0;
const int pin = 11;

void setup() {
    miservo.attach(pin,1000,2000); //Set the pin to attach the servo
}

void loop() {
    //Neutral position
    miservo.write(90);
    delay(30);

    //Decrease the angle from 90
    for (angulo =90; angulo >0; angulo --) {
        miservo.write(angulo);
        delay(10);
    }

    delay(50);
}
