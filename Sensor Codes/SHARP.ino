const float ResolutionADC=0.00488; //4.88mV

//const int Sharp_Pin=A3; // J23
const int Sharp_Pin=A4; // J24

int Value_Sharp_Pin=0;
float Voltage;
const float b = 0.024, m = 11.89;

void setup() {
    Serial.begin(9600); //Enable the serial port
}
void loop() {
    // Reads the sensor and return a value between 0-1023
    Value_Sharp_Pin = analogRead(Sharp_Pin);

    // Calculates the equivalent voltage
    Voltage = Value_Sharp_Pin*ResolutionADC;

    float x = (Voltage-b)/m;
    float L = (1-0.42*x)/x;

    Serial.print("Distancia ");
    Serial.println(L);
    Serial.println(Value_Sharp_Pin);
    Serial.print(" Voltage: ");
    Serial.print(Voltage);
    Serial.println(" V");
    delay(1000);
}
