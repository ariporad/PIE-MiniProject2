#define BAUD_RATE 115200
#define SENSOR_PIN A0

int mode = 0;

void setup()
{ 
  Serial.begin(BAUD_RATE);

  pinMode(SENSOR_PIN, INPUT);
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