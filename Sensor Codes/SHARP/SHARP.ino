const float ResolutionADC=0.00488; //4.88mV

const int SharpI_Pin=A8; // J8
const int SharpD_Pin=A4; // J24

int Value_SharpI_Pin=0;
int Value_SharpD_Pin=0;
float VoltageI;
float VoltageD;
const float b = 0.024, m = 11.89;

void setup() {
    Serial1.begin(9600); //Enable the serial port
}
void loop() {
    // Reads the sensor and return a value between 0-1023
    Value_SharpI_Pin = analogRead(SharpI_Pin);
    // Calculates the equivalent voltage
    VoltageI = Value_SharpI_Pin*ResolutionADC;
    float xI = (VoltageI-b)/m;
    float LI = (1-0.42*xI)/xI;
    Serial1.print("Distancia Sharp I");
    Serial1.println(LI);
    //Serial1.println(Value_SharpI_Pin);

    Value_SharpD_Pin = analogRead(SharpD_Pin);
    // Calculates the equivalent voltage
    VoltageD = Value_SharpD_Pin*ResolutionADC;
    float xD = (VoltageD-b)/m;
    float LD = (1-0.42*xD)/xD;
    Serial1.print("Distancia Sharp D");
    Serial1.println(LD);
    //Serial1.println(Value_SharpD_Pin);

    delay(1000);
}
