#include <Servo.h>

Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0; // variable to store the servo position
const int redPin=2;
const int speedPin=3;
const int echoPin = 4;
const int trigPin = 5;
const int servoPin = 9;

void setup()
{
    myservo.attach(servoPin); // attaches the servo on pin 9 to the servo object
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(redPin, INPUT);
    attachInterrupt(0, danger, FALLING);
    Serial.begin(9600);
}

int n=0;
void loop()
{
    /*
    for (pos = 0; pos <= 180; pos += 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        servoTurn(pos);
        Serial.println(getDistance());
        delay(150); // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1)
    { // goes from 180 degrees to 0 degrees
        servoTurn(pos);
        Serial.println(getDistance());
        delay(150); // waits 15ms for the servo to reach the position
    }
    */
   if (n>0 && digitalRead(redPin)==1)
        undanger();
    delay(15); 
}

void servoTurn(int angle)
{
    if (angle < 0)
        angle = 0;
    if (angle > 180)
        angle = 180;
    myservo.write(angle);
}

double getDistance()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    double cm=pulseIn(echoPin, HIGH)/58.0;
    return cm;
}
void danger()
{
    char str[100];
    memset(str, 0, 100);
    sprintf(str, "Danger %d", n);
    Serial.println(str);
    n++;
}
void undanger()
{
    n=0;
    Serial.println("Undanger");
}