
void setup(){
  Serial1.begin(9600);
  Serial.begin(9600);
}

int incomingByte = 0;

unsigned int i=0;
byte buf[8];

void loop(){
  if (Serial1.available() > 0) {
    String s = Serial1.readStringUntil('#');
    if (s == "Hola") {
      digitalWrite(12,HIGH);
      digitalWrite(11,LOW);
    }else{
      digitalWrite(12,LOW);
      digitalWrite(11,HIGH);
    }
    s = s + "#";
    Serial.println(s);
    Serial1.print(s);
    //Serial1.flush();
  }
}
