/* By Ari & Berwin */

#include <Servo.h>

#define BAUD_RATE 115200

#define SENSOR_PIN A0
#define Y_SERVO_PIN 9
#define X_SERVO_PIN 10

#define X_POS_MIN 90
#define X_POS_MAX 90
#define X_POS_STEP 1
#define Y_POS_MIN 75  // 45
#define Y_POS_MAX 100 // 135
#define Y_POS_STEP 1

int mode = 1;

Servo yServo;
int yPos = Y_POS_MIN;

Servo xServo;
int xPos = X_POS_MIN;

void setup()
{ 
  Serial.begin(BAUD_RATE);
  Serial.setTimeout(1);

  pinMode(SENSOR_PIN, INPUT);
  yServo.attach(Y_SERVO_PIN);
  yServo.write(yPos);

  xServo.attach(X_SERVO_PIN);
  xServo.write(xPos);
}

void loop() 
{  
  if (Serial.available() > 0)
  {
    mode = Serial.read();
    if (mode == 1 && Serial.available() >= 2)
    {
      xPos = constrain(Serial.read(), X_POS_MIN, X_POS_MAX);
      yPos = constrain(Serial.read(), Y_POS_MIN, Y_POS_MAX);
    }

    // drain the queue
    while (Serial.available()) Serial.read();
  }

  switch (mode)
  {
    case 0: // Disabled
      xPos = X_POS_MIN;
      yPos = Y_POS_MIN;
      break;
    case 1: // Scan
    {
      int sensorValue = min(min(analogRead(SENSOR_PIN), analogRead(SENSOR_PIN)), analogRead(SENSOR_PIN));

      Serial.print("0,");
      Serial.print(xPos);
      Serial.print(",");
      Serial.print(yPos);
      Serial.print(',');
      Serial.println(sensorValue);
  
      yPos += Y_POS_STEP;
      if (yPos > Y_POS_MAX)
      {
        yPos = Y_POS_MIN;
        xPos += X_POS_STEP;
      }
      if (xPos > X_POS_MAX)
      {
        xPos = X_POS_MIN;
        mode = 0;
        Serial.println("1,0,0,0");
      }
    }
  }

  yServo.write(yPos);
  xServo.write(xPos);

  delay(400);
}
