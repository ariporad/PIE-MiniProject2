from mp2.arduinolib import Arduino

with Arduino('/dev/cu.usbmodem14401', baudRate=115200) as arduino:
  print("Connected to Arduino!")

  for i, line in enumerate(arduino.lines()):
    print(f"Line({i:06d}): {line}")

    if i % 20 == 0:
      print("Switching to Mode 1 (prints millis())...")
      arduino.write(1)
    elif i % 10 == 0:
      print("Switching to Mode 2 (just prints 2s)...")
      arduino.write(2)


