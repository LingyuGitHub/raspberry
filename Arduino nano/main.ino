#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include "./l298n.hpp"
#include "./ds18b20.hpp"
#include "./echo.hpp"

Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards

const int redPin=2;
const int speedPin=3;
const int echoPin = 4;
const int trigPin = 5;
const int rightPin1=6;
const int rightPin2=7;
const int leftPin1=8;
const int leftPin2=9;
const int softRXPin=10;
const int softTXPin=11;
const int servoPin = 12;
bool isDanger=false;

SoftwareSerial softSerial(softRXPin,softTXPin);
L298N car(rightPin1, rightPin2, leftPin1, leftPin2);

void creatap()
{
    softSerial.write("AT+CWMODE=2\r\n");
    delay(1000);
    if (softSerial.available()) {
        Serial.write(softSerial.read());
    }
    softSerial.write("AT+RST\r\n");
    delay(5000);
    if (softSerial.available()) {
        Serial.write(softSerial.read());
    }
    Serial.println("Create soft ap");
    softSerial.write("AT+CWSAP=\"esp8266\",\"1234567890\",1,4\r\n");
    delay(1000);
    if (softSerial.available()) {
        Serial.write(softSerial.read());
    }
}

void openSocket()
{
    softSerial.println("AT+RST\r\n");
    if (softSerial.available()) {
        Serial.println(softSerial.readString());
    }
    delay(1000);
    softSerial.println("AT+CIPMUX=1\r\n");
    if (softSerial.available()) {
        Serial.println(softSerial.readString());
    }
    delay(1000);
    softSerial.println("AT+CIPSERVER=1,8000\r\n");
    if (softSerial.available()) {
        Serial.println(softSerial.readString());
    }
    delay(1000);
    softSerial.println("AT+CIPMODE=1\r\n");
    if (softSerial.available()) {
        Serial.println(softSerial.readString());
    }
}

void setup()
{
    myservo.attach(servoPin); // attaches the servo on pin 9 to the servo object
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    attachInterrupt(0, danger, FALLING);
    Serial.begin(9600);
    softSerial.begin(19200);
    if (softSerial.available()) 
    {
        Serial.write(softSerial.read());
    }
    delay(2000);
    openSocket();
}

void loop()
{
    String cmd;
    int act=-1;
    int val=-1;

    cmd=getmsg();
    if (cmd.length()>0)
    {
        Serial.println(cmd);
        if (parseCMD(cmd, act, val))
        {
            setAction(act, val);
        }
    }
    return;

    if (isDanger && digitalRead(redPin)==1)
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
    Serial.println(str);
    isDanger=true;
}
void undanger()
{
    Serial.println("Undanger");
    isDanger=false;
}


bool parseCMD(String cmd, int &act, int &val)
{
    act=-1;
    val=-1;
    
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, cmd);
    if (error) 
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return false;
    }
    JsonObject root = doc.as<JsonObject>();

    act=root["act"];
    val=root["val"];

    return true;
}

void setAction(int act, int val)
{
    switch(act)
    {
        case STOP:
            car.stop();
            break;
        case GOFORE:
            car.goFore();
            break;
        case GOBACK:
            car.goBack();
            break;
        case TURNLEFT:
            car.turnLeft();
            break;
        case TURNRIGHT:
            car.turnRight();
            break;
    }
}

String getmsg()
{
    String cmd="";
    if (softSerial.available()) 
    {
        Serial.println("available");
        cmd=softSerial.readString();
        Serial.println("get");
    }

    int i=cmd.indexOf(":");
    if (i>=0)
    {
        cmd=cmd.substring(i+1);
    }
    return cmd;
}