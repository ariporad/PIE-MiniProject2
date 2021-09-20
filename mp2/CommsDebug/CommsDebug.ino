/* By Ari & Berwin */
#define BAUD_RATE 115200

int mode = 0;

void setup()
{ 
  Serial.begin(BAUD_RATE);
}

void loop() 
{  
  if (Serial.available() > 0 && Serial.peek() != 0)
  {
    mode = Serial.read();
    Serial.print("Got: ");
    Serial.println(mode);
  }

  switch (mode)
  {
    case 1:
      Serial.println(millis());
      break;
    case 2:
      Serial.println('2');
      break;
    default:
      Serial.println('0');
      break;
  }

  delay(100);
}
