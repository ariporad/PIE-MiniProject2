//      ******************************************************************
//      *                                                                *
//      *                                                                *
//      *     Example Arduino program that transmits data to a laptop    *
//      *                                                                *
//      *                                                                *
//      ******************************************************************

#define SENSOR_PIN A0

//
// setup function to initialize hardware and software
//
void setup()
{ 
  //
  // start the serial port
  //
  long baudRate = 115200;       // NOTE1: The baudRate for sending & receiving programs must match
  Serial.begin(baudRate);     // NOTE2: Set the baudRate to 115200 for faster communication

  pinMode(SENSOR_PIN, INPUT);
}



//
// main loop
//
void loop() 
{  
  //
  // loop: calculate the data, then send it from the Arduino to the python program
  //
  while(true) {
    //
    // transmit one line of text to python with 4 numeric values
    // NOTE: commas are sent between values, after the last value a Newline is sent
    //
    int data = analogRead(SENSOR_PIN);
    Serial.print("0,");
    Serial.println(data);
    

    //
    // delay after sending data so the serial connection is not over run
    //
    delay(400);
  }
}



