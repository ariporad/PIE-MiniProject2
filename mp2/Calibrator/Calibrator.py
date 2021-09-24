import os
from time import sleep

##################### ARDUINOLIB STARTS HERE ###########################
from serial import Serial

class Arduino:
  """
  A wrapper class around a serial port with some helpers for easy communication to/from an Arduino.

  Example:

  ```python
  with Arduino('/dev/com.whatever', baudRate=115200) as arduino:
    for line in arduino.lines():
      print("Got line from Arduino: " + line)
      arduino.write(55) # Writes 0x55 as a byte. Also accepts raw bytes or strings.
  ```
  """

  def __init__(self, port, baudRate=115200, timeout=1, logging=False):
    """
    Connect to an Arduino
    """
    self.logging = logging
    self.serial_port = Serial(port, baudRate, timeout=timeout)

  def _log(self, *args, **kwargs):
    """
    Helper method that acts like `print`, when logging=True but does nothing otherwise.
    """
    if self.logging:
      print("[Arduino]", *args, **kwargs)
  
  # We want to just pass through to the serial port's context manager
  def __enter__(self):
    """
    When using an Arduino as a context manager, the Arduino will intelligently open/close the serial
    port upon entering/exiting the context manager, including doing so multiple times.
    """
    self._log("Entering Arduino context manager, connecting serial port...")
    self.serial_port.__enter__()

    # But return self so you can do `with Arduino(...) as arduino:`
    return self

  def __exit__(self, __exc_type, __exc_value, __traceback):
    """
    When using an Arduino as a context manager, the Arduino will intelligently open/close the serial
    port upon entering/exiting the context manager, including doing so multiple times.
    """
    self._log("Exiting Arduino context manager, disconnecting serial port...")
    return self.serial_port.__exit__(__exc_type, __exc_value, __traceback)
  
  # NB: Calling lines() or packets() more than once is undefined behavior
  def lines(self, drain_first=True):
    """
    Return an iterator that yields each line the Arduino sends over the Serial connection.

    If drain_first is True, any serial data already received and buffered but not yet processed will
    be erased.

    NOTE: This iterator will block while waiting for a line
    NOTE: Calling this method more than once, or calling it after packets() has been called, is
          undefined behavior.
    """
    if drain_first:
      self.serial_port.reset_input_buffer()

    while True:
      # NOTE: technically this would get rid of leading spaces too if that was something you cared about
      line = self.serial_port.readline().decode().strip()
      if len(line) > 0:
        self._log(f"Received Line: {line}")
        yield line

  def packets(self, drain_first=True):
    """
    Return an iterator that yields each packet the Arduino sends over the Serial connection.

    A packet is defined as a newline-terminated, comma-separated list of integers. In other words,
    this method expects that your Arduino writes data over serial that looks like this: `1,2,3\n`.

    If drain_first is True, any serial data already received and buffered but not yet processed will
    be erased.

    NOTE: This iterator will block while waiting for a line
    NOTE: Calling this method more than once, or calling it after lines() has been called, is
          undefined behavior.
    """
    for line in self.lines(drain_first=drain_first):
      packet = tuple(int(data) for data in line.split(','))
      self._log(f"Received Packet: {packet}")
      yield packet

  def write(self, data):
    """
    Write data to the Arduino over Serial. If data is bytes, it will be sent as-is. If data is an
    int, it will be converted to an unsigned 8-bit integer and sent that way (attempting to write an
    integer outside of the range 0-255 is an error). If data is a string it will be utf-8 encoded.
    If data is a list each element will be individually written as per the above rules.
    """
    self._log(f"Writing data (may need conversion to bytes): {data}")

    if not isinstance(data, bytes):
      if isinstance(data, str):
        self.write(data.encode('utf-8'))
      elif isinstance(data, int):
        self.write(data.to_bytes(1, 'big', signed=True))
      elif isinstance(data, list):
        for data_item in data:
          self.write(data_item)
      else:
        raise Exception("Cannot write data of unknown type!")
    else:
      self.serial_port.write(data)
  
  def writeln(self, data):
    """
    Write a string to the Arduino over Serial, and add a newline at the end.
    """
    return self.write(f"{data}\n")
##################### ARDUINOLIB ENDS HERE ###########################

START_DISTANCE = 5.5
END_DISTANCE = 70.5
STEP_DISTANCE = 1.5

def floatrange(start, stop, step):
  value = start
  while value <= stop:
    yield value
    value += step

# https://stackoverflow.com/a/4060259
__location__ = os.path.realpath( os.path.join(os.getcwd(), os.path.dirname(__file__)))

data = []

with Arduino("/dev/cu.usbmodem14401", baudRate=115200, logging=True) as arduino:
  print(f"Connected to Arduino! Calibrating from {START_DISTANCE}in to {END_DISTANCE}in ({STEP_DISTANCE}in steps)...")
  packets = arduino.packets()

  for distance in floatrange(START_DISTANCE, END_DISTANCE + 1, STEP_DISTANCE):
    input(f"Move the target to {distance}in from the sensor, then press ENTER: ")
    arduino.write(1)
    reading = next(packets)[0]
    print(f"Got: {reading} @ {distance}in")
    data.append((distance, reading))
  
  print("Done!")
  
  name = input("Name this calibration: ").strip()
  path = os.path.join(__location__, "calibrations", name + '.csv')

  with open(path, 'w') as f:
    f.write("distance,sensorReading\n")
    for distance, reading in data:
      f.write(f"{distance},{reading}\n")
  
  print("Calibration written to: " + path)
  

    

