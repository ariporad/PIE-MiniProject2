#include <Servo.h>

#define BAUD_RATE 115200

#define SENSOR_PIN A0
#define Y_SERVO_PIN 9
#define X_SERVO_PIN 10

// See setup()
#define X_SERVO_POS 90
#define Y_SERVO_POS 100

Servo xServo;
Servo yServo;

void setup()
{ 
  // Connect to Python
  Serial.begin(BAUD_RATE);

  // Configure the pins
  pinMode(SENSOR_PIN, INPUT);

  // Calibration doesn't move the servos, but we want to set them to a fixed position.
  // If the servos aren't physically attached, this does nothing which is fine.
  xServo.attach(X_SERVO_PIN);
  yServo.attach(Y_SERVO_PIN);
  xServo.write(X_SERVO_POS);
  yServo.write(Y_SERVO_POS);
}

void loop() 
{  
  // Python writes one byte for every sensor reading it wants to receive
  if (Serial.available() > 0)
  {
    Serial.read(); // Consume one byte

    delay(250); // Go slow to avoid overwhelming things

    // Getting the lowest of three sensor readings, as per Brad's instructions
    int sensorValue = min(min(analogRead(SENSOR_PIN), analogRead(SENSOR_PIN)), analogRead(SENSOR_PIN));

    // Send the sensor value back to Python
    Serial.println(sensorValue);
  }
}