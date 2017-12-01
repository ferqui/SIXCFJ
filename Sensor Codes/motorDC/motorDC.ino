/* Giro alterno de motor DC con driver L293D */

//Definición de variables
const int in1PinRight = 9; // Right motor
const int in2PinRight = 10;
const int in1PinLeft = 5; // Left motor
const int in2PinLeft = 6;
int Speed_Value = 100;

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
    analogWrite(in1PinRight,LOW);
    analogWrite(in2PinRight,speed_Value);
    analogWrite(in1PinLeft,LOW);
    analogWrite(in2PinLeft,speed_Value*0.91);
}

void forwardMotor (int speed_Value) {
    analogWrite(in1PinRight,speed_Value);
    analogWrite(in2PinRight,LOW);
    analogWrite(in1PinLeft,speed_Value*0.91);
    analogWrite(in2PinLeft,LOW);
}

void stopMotor() {
    analogWrite(in1PinRight,LOW);
    analogWrite(in2PinRight,LOW);
    analogWrite(in1PinLeft,LOW);
    analogWrite(in2PinLeft,LOW);
}
