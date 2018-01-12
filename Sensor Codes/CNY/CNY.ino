// const float ResolutionADC=0.00488; //4.88mV
const int CNY1_Pin=A5;
const int CNY2_Pin=A1;
const int CNY3_Pin=A0;
int Value_CNY1_Pin=0;
int Value_CNY2_Pin=0;
int Value_CNY3_Pin=0;
int lowerlimit_black = 700;
int upperlimit_white = 600;

void setup(){
  Serial1.begin(9600); //Enable the Bluetooth
}

void loop(){
// Reads the sensor and return a value between 0-1023
  Value_CNY1_Pin=analogRead(CNY1_Pin);
  Value_CNY2_Pin=analogRead(CNY2_Pin);
  Value_CNY3_Pin=analogRead(CNY3_Pin);
  
  if (Value_CNY1_Pin<upperlimit_white) {
    Serial1.print(Value_CNY1_Pin);
    Serial1.println(" CNY1 BLANCO");
  } else {
    if (Value_CNY1_Pin>lowerlimit_black) {
      Serial1.print(Value_CNY1_Pin);
      Serial1.println(" CNY1 NEGRO");
    }
  }
  
  if (Value_CNY2_Pin<upperlimit_white) {
      Serial1.print(Value_CNY2_Pin);
      Serial1.println(" CNY2 BLANCO");
  } else {
    if (Value_CNY2_Pin>lowerlimit_black) {
      Serial1.print(Value_CNY2_Pin);
      Serial1.println(" CNY2 NEGRO");
    }
  }
  
  if (Value_CNY3_Pin<upperlimit_white) {
      Serial1.print(Value_CNY3_Pin);
      Serial1.println(" CNY3 BLANCO");
  } else {
    if (Value_CNY3_Pin>lowerlimit_black) {
      Serial1.print(Value_CNY3_Pin);
      Serial1.println(" CNY3 NEGRO");
    }
  }

  delay(1000);
}
