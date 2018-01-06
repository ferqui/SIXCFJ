/* Mirian Cifredo - University of Cádiz
* 24/06/2015
*/

/*Variables*/
float distance;
const int pin = A3;
unsigned long time_bounce;

void setup() {
    Serial.begin(9600); // Setup the serial port
}

void loop() {
    pinMode(pin, OUTPUT); // Setup pin9 as an output.(TRIGGER)
    // It will send the ultrasonic ping

    /* Keep to LOW the sensor for 5us to ensure a later rising-edge */
    digitalWrite(pin, LOW);
    delayMicroseconds(5);

    /* Send a pulse HIGH for 10 us (According to datasheet) */
    digitalWrite(pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin, LOW);
    /* "pulseIn" measures how many takes a signal(in microseconds) goes
    from HIGH (when it receives the bounce) to LOW(when it stops to receive
    the bounce) or LOW to HIGH. “pulseIN” works right on pulses
    from 10 microseconds to 3 minutes in length.*/

    pinMode(pin, INPUT); // Setup pin9 as an input.(ECHO)
    // It will receive the ultrasonic bounce

    time_bounce = pulseIn(pin, HIGH);
    /* Formula to figure out the distance
     * speed = space /time --> space = speed * time
     * sound speed = 340 m/s --> 0.034 cm/us
     * space --> to travel from the sensor to the object and from de object to
     sensor
     * distance to object = space / 2
     */

    distance = 0.017 * time_bounce; //Fórmula para calcular la distancia
    
    /*Show in cm the distance measured, by serial monitor */
    Serial.println("Distancia ");
    Serial.print(distance);
    Serial.println(" cm");

    delay(1000);
}
