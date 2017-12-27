
void setup(){
  Serial1.begin(9600);
  Serial.begin(9600);
}

int incomingByte = 0;

unsigned int i=0;
byte buf[8];

void loop(){
  if (Serial1.available() > 0) {
    String s = Serial1.readStringUntil('#')+"XS#";
    Serial.println(s);
    Serial1.print(s);
    Serial1.flush();
  }
  /*if (Serial1.available() >= 8) {
    Serial1.readBytes(buf, 8);
    Serial.print("I received: ");
    for(int i = 0; i < 8; ++i)
      Serial.println(buf[i], BIN);
    Serial.println("****************");
    Serial1.println("Hola#");
  }*/
  /*if (Serial1.availableForWrite() > 0) {
    sprintf(buf, "Hello!%d#", i++);
    Serial1.println(buf);
  }*/
}
