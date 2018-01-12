const float ResolutionADC=0.00488; //4.88mV

const int SharpI_Pin=A8; // J8
const int SharpD_Pin=A4; // J24
int Value_SharpI_Pin=0;
int Value_SharpD_Pin=0;
float VoltageI;
float VoltageD;
const float b = 0.024, m = 11.89;

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
  // Reads the sensor and return a value between 0-1023
  Value_SharpI_Pin = analogRead(SharpI_Pin);
  VoltageI = Value_SharpI_Pin*ResolutionADC;
  float xI = (VoltageI-b)/m;
  float LI = (1-0.42*xI)/xI;
  // Serial1.print("Distancia Sharp I");
  // Serial1.println(LI);
  // Serial1.println(Value_SharpI_Pin);

  Value_SharpD_Pin = analogRead(SharpD_Pin);
  VoltageD = Value_SharpD_Pin*ResolutionADC;
  float xD = (VoltageD-b)/m;
  float LD = (1-0.42*xD)/xD;
  // Serial1.print("Distancia Sharp D");
  // Serial1.println(LD);
  // Serial1.println(Value_SharpD_Pin);
  Serial1.println(LD-LI);
  if (LD-LI>2 && LD-LI<30) {
    right(speed_Value);
  } else {
    if (LD-LI<-2 && LD-LI>-30) {
      left(speed_Value);
    } else {
      forwardMotor(speed_Value);
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
