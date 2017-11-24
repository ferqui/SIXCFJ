/* Giro alterno de motor DC con driver L293D */

//Definición de variables
const int in1PinRight = 5; // Right motor
const int in2PinRight = 6;
const int in1PinLeft = 9; // Left motor
const int in2PinLeft = 10;
int Speed_Value = 1;

void setup() {
    pinMode(in1PinRight, OUTPUT);
    pinMode(in2PinRight, OUTPUT);
    pinMode(in1PinLeft, OUTPUT);
    pinMode(in2PinLeft, OUTPUT);
}

void loop() {
    if (Speed_Value<=500) {
        int sp_v = map(Speed_Value,500,0,0,255);
        forwardMotor(sp_v);
    }else {
        if((500<Speed_Value)&&(Speed_Value<520))
            stopMotor();
        else{
            int sp_v = map(Speed_Value,520,1023,0,255);
            reverseMotor(sp_v);
        }
    }
}

void reverseMotor (int speed_Value) {
    digitalWrite(in1PinRight,LOW);
    digitalWrite(in2PinRight,speed_Value);
    digitalWrite(in1PinLeft,LOW);
    digitalWrite(in2PinLeft,speed_Value);
}

void forwardMotor (int speed_Value) {
    digitalWrite(in1PinRight,speed_Value);
    digitalWrite(in2PinRight,LOW);
    digitalWrite(in1PinLeft,speed_Value);
    digitalWrite(in2PinLeft,LOW);
}

void stopMotor() {
    digitalWrite(in1PinRight,LOW);
    digitalWrite(in2PinRight,LOW);
    digitalWrite(in1PinLeft,LOW);
    digitalWrite(in2PinLeft,LOW);
}
