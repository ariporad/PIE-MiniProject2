from math import exp
from mp2.arduinolib import Arduino

# From MATLAB curve fit
def sensor_to_distance(sensorValue):
  return 164.8 * exp(-0.004518 * sensorValue)

with Arduino("/dev/cu.usbmodem14401", baudRate=115200) as arduino:
  print("Connected to Arduino!")

  arduino.write(1) # Turn on the scanner

  for xPos, yPos, sensorValue in arduino.packets():
    distance = sensor_to_distance(sensorValue)
    print(f"Got Packet: xPos = {xPos}, yPos = {yPos}, sensorValue = {sensorValue}, distance = {distance} cm = {(distance / 2.54)} in")

