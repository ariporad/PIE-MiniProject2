/* By Ari & Berwin */

#include <Servo.h>

#define BAUD_RATE 115200

#define SENSOR_PIN A0
#define Y_SERVO_PIN 9
//#define X_SERVO_PIN 10

#define SERVO_STEP 5

int mode = 1;

Servo yServo;
int yPos = 0;

//Servo xServo;
//int xPos = 0;

void setup()
{ 
  Serial.begin(BAUD_RATE);

  pinMode(SENSOR_PIN, INPUT);
  yServo.attach(Y_SERVO_PIN);
  yServo.write(90);

//  xServo.attach(X_SERVO_PIN);
//  xServo.write(90);
}

void loop() 
{  
  if (Serial.available() > 0)
  {
    mode = Serial.read();
  }

  switch (mode)
  {
    case 0: // Disabled
      break;
    case 1: // Scan
    {
      int sensorValue = analogRead(SENSOR_PIN);

      Serial.print("0,");
      Serial.print(yPos);
      Serial.print(',');
      Serial.println(sensorValue);
  
      yPos += SERVO_STEP;
//      xPos += SERVO_STEP;
      if (yPos > 180)
      {
        yPos = 0;
      }
//      if (xPos > 180)
//      {
//        xPos = 0;
//      }
      yServo.write(yPos);
//      xServo.write(xPos);

      //
      // delay after sending data so the serial connection is not over run
      //
      delay(400);
    }
  }
}
