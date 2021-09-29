#include <Servo.h>

#define BAUD_RATE 115200

#define SENSOR_PIN A0
#define Y_SERVO_PIN 9
#define X_SERVO_PIN 10

// All positions are servo positions, so within [0, 180]
#define X_POS_MIN 90
#define X_POS_MAX 90
#define X_POS_STEP 1
#define Y_POS_MIN 75
#define Y_POS_MAX 100
#define Y_POS_STEP 1

int mode = 1; // 0 = disabled, 1 = scanning

Servo yServo;
int yPos = Y_POS_MIN;

Servo xServo;
int xPos = X_POS_MIN;

void setup()
{ 
  // Configure the Serial connection
  Serial.begin(BAUD_RATE);
  Serial.setTimeout(1);

  // Setup the sensor
  pinMode(SENSOR_PIN, INPUT);

  // Initialize all servos to their starting positions
  yServo.attach(Y_SERVO_PIN);
  yServo.write(yPos);
  xServo.attach(X_SERVO_PIN);
  xServo.write(xPos);
}

void loop() 
{  
  // First, check if we've received data from Python
  if (Serial.available() > 0)
  {
    mode = Serial.read(); // First byte is always the new mode
    
    // If we've just started scanning, Python can also provide exactly two numbers as the starting
    // positions of the servos (used to resume a failed scan).
    if (mode == 1 && Serial.available() >= 2)
    {
      // The provided positions must fit within the allowed position boundaries
      // Python uses this when starting a fresh scan, by sending 0, 0 to get the minimum positions
      xPos = constrain(Serial.read(), X_POS_MIN, X_POS_MAX);
      yPos = constrain(Serial.read(), Y_POS_MIN, Y_POS_MAX);
    }

    // If we've just switched modes, drain all bytes currently in the Serial buffer
    // It's unclear exactly how beneficial this is
    while (Serial.available()) Serial.read();
  }

  switch (mode)
  {
    case 0: // Disabled
      // When disabled, always reset the servo positions
      xPos = X_POS_MIN;
      yPos = Y_POS_MIN;
      break;
    case 1: // Scan
    {
      // Read from the sensor, using Brad's suggestion to take the lowest of three readings
      int sensorValue = min(min(analogRead(SENSOR_PIN), analogRead(SENSOR_PIN)), analogRead(SENSOR_PIN));

      // Send data to Python
      Serial.print("0,"); // status = 0, because we are still scanning
      Serial.print(xPos);
      Serial.print(",");
      Serial.print(yPos);
      Serial.print(',');
      Serial.println(sensorValue);
  
      // Move the servos
      yPos += Y_POS_STEP;   // increment y
      if (yPos > Y_POS_MAX) // if we've finished a sweep along the y axis, reset and increment x
      {
        yPos = Y_POS_MIN;
        xPos += X_POS_STEP;
      }
      if (xPos > X_POS_MAX) // if we've just incremented x beyond the end of the x axis, we're done!
      {
        xPos = X_POS_MIN; // reset the x servo (y is already at the starting position if we're here)
        mode = 0; // Stop scanning
        Serial.println("1,0,0,0"); // Tell Python we finished
      }
    }
  }

  // Every loop, move the servos to whereever they should be
  yServo.write(yPos);
  xServo.write(xPos);

  // Delay to give servos time to move and to avoid overloading Serial
  delay(400);
}
