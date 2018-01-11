/* Código radiocontrol
   Sensores necesarios: Motores y Bluetooth
*/


const int in1PinRight = 9; // Right motor
const int in2PinRight = 10;
const int in1PinLeft = 5; // Left motor
const int in2PinLeft = 6;
int speed_Value = 100;
const float disminucion = 0.95;

void setup() {
    pinMode(in1PinRight, OUTPUT);
    pinMode(in2PinRight, OUTPUT);
    pinMode(in1PinLeft, OUTPUT);
    pinMode(in2PinLeft, OUTPUT);

    Serial1.begin(9600); // Bluetooth
}

void loop() {
  if (Serial1.available() > 0 ) {
    String s = Serial1.readStringUntil('#');
    Serial1.read();
        if (s=="a") {
          forwardMotor(speed_Value);
        } else {
          if (s=="b") {
            reverseMotor(speed_Value);
          } else {
            if (s=="d") {
              right(speed_Value);
            } else {
              if (s=="i") {
                left(speed_Value);
              } else {
                stopMotor();
              }
            }
          }
        }
    }

  }


  void reverseMotor (int speed_Value) {
      analogWrite(in1PinRight,speed_Value);
      analogWrite(in2PinRight,LOW);
      analogWrite(in1PinLeft,speed_Value*disminucion);
      analogWrite(in2PinLeft,LOW);
  }

  void forwardMotor (int speed_Value) {
      analogWrite(in1PinRight,LOW);
      analogWrite(in2PinRight,speed_Value);
      analogWrite(in1PinLeft,LOW);
      analogWrite(in2PinLeft,speed_Value*disminucion);
  }

  void left (int speed_Value) {
      analogWrite(in1PinRight,speed_Value);
      analogWrite(in2PinRight,LOW);
      analogWrite(in1PinLeft,LOW);
      analogWrite(in2PinLeft,speed_Value*disminucion);
  }

  void right (int speed_Value) {
      analogWrite(in1PinRight,LOW);
      analogWrite(in2PinRight,speed_Value);
      analogWrite(in1PinLeft,speed_Value*disminucion);
      analogWrite(in2PinLeft,LOW);
  }

  void stopMotor() {
      analogWrite(in1PinRight,LOW);
      analogWrite(in2PinRight,LOW);
      analogWrite(in1PinLeft,LOW);
      analogWrite(in2PinLeft,LOW);
  }
