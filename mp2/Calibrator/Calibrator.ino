#include <Servo.h>

#define BAUD_RATE 115200
#define SENSOR_PIN A0
#define Y_SERVO_PIN 9
#define X_SERVO_PIN 10

#define X_SERVO_POS 90
#define Y_SERVO_POS 100

int mode = 0;

Servo xServo;
Servo yServo;

void setup()
{ 
  Serial.begin(BAUD_RATE);

  pinMode(SENSOR_PIN, INPUT);

  xServo.attach(X_SERVO_PIN);
  yServo.attach(Y_SERVO_PIN);
  xServo.write(X_SERVO_POS);
  yServo.write(Y_SERVO_POS);
}

void loop() 
{  
  // We want to write one sensor value per byte written
  if (Serial.available() > 0)
  {
    Serial.read(); // Consume one byte

    delay(250);

    int sensorValue = min(min(analogRead(SENSOR_PIN), analogRead(SENSOR_PIN)), analogRead(SENSOR_PIN));

    Serial.println(sensorValue);
  }
}