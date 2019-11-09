#include "DHT.h"
#include <BlynkSimpleSerialBLE.h>
#include <TroykaMQ.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <Ultrasonic.h>

// BLUETOOTH
SoftwareSerial SerialBLE(10, 11); // RX, TX


// SERVO'S
#define servoWindowPin 9
#define WINDOW_CLOSE 180
#define WINDOW_OPEN 0

#define servoDoorPin 8
#define DOOR_CLOSE 170
#define DOOR_OPEN 70

Servo servoWindow;
Servo servoDoor;

// DVER's
#define sensorDoorPin A3

// DHT 11
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


// TROYKA
#define pinMQ A0 //MQ6
MQ6 mq6(pinMQ);

// BLYNK
#define BLYNK_PRINT Serial
char auth[] = "Z4RZ-AGqz24J_OSpPvW8-jZXDopaJNLw";

BLYNK_READ(V0) {
  Blynk.virtualWrite(V0, mq6.readLPG());
}

BLYNK_READ(V2) {
  //  float t = dht.readTemperature();
  ////  if (isnan(t)) {
  ////    Blynk.virtualWrite (V2, "error");
  ////    servoWindow.write(WINDOW_CLOSE);
  ////    Blynk.virtualWrite(V4, "CLOSE");
  //    return;
  //  }

  float t = 28

  if (t > 28) {
    servoWindow.write(WINDOW_OPEN);
    Blynk.virtualWrite(V4, "OPEN");
  }
  if (t < 28) {
    servoWindow.write(WINDOW_CLOSE);
    Blynk.virtualWrite(V4, "CLOSE");
  }

  Blynk.virtualWrite (V2, t);
}

BLYNK_READ(V3) {
  if (analogRead(sensorDoorPin) < 400) {
    servoDoor.write(DOOR_OPEN);
    Blynk.virtualWrite(V3, "OPEN");
    return;
  }

  servoDoor.write(DOOR_CLOSE);
  Blynk.virtualWrite(V3, "CLOSE");
}

void setup()
{
  Serial.begin(9600);
  servoWindow.attach(servoWindowPin);
  servoDoor.attach(servoDoorPin);
  servoWindow.write(WINDOW_CLOSE);
  servoDoor.write(DOOR_CLOSE);

  dht.begin();
  mq6.calibrate();

  SerialBLE.begin(9600);
  Blynk.begin(SerialBLE, auth);
}

void loop()
{
  Blynk.run();
}
