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
    self.serialPort = Serial(port, baudRate, timeout=timeout)

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
    self.serialPort.__enter__()

    # But return self so you can do `with Arduino(...) as arduino:`
    return self

  def __exit__(self, __exc_type, __exc_value, __traceback):
    """
    When using an Arduino as a context manager, the Arduino will intelligently open/close the serial
    port upon entering/exiting the context manager, including doing so multiple times.
    """
    self._log("Exiting Arduino context manager, disconnecting serial port...")
    return self.serialPort.__exit__(__exc_type, __exc_value, __traceback)
  
  # NB: Calling lines() or packets() more than once is undefined behavior
  def lines(self):
    """
    Return an iterator that yields each line the Arduino sends over the Serial connection.

    NOTE: This iterator will block while waiting for a line
    NOTE: Calling this method more than once, or calling it after packets() has been called, is
          undefined behavior.
    """
    while True:
      # NOTE: technically this would get rid of leading spaces too if that was something you cared about
      line = self.serialPort.readline().decode().strip()
      if len(line) > 0:
        self._log(f"Received Line: {line}")
        yield line

  def packets(self):
    """
    Return an iterator that yields each packet the Arduino sends over the Serial connection.

    A packet is defined as a newline-terminated, comma-separated list of integers. In other words,
    this method expects that your Arduino writes data over serial that looks like this: `1,2,3\n`.

    NOTE: This iterator will block while waiting for a line
    NOTE: Calling this method more than once, or calling it after lines() has been called, is
          undefined behavior.
    """
    for line in self.lines():
      packet = tuple(int(data) for data in line.split(','))
      self._log(f"Received Packet: {packet}")
      yield packet

  def write(self, data):
    """
    Write data to the Arduino over Serial. If data is bytes, it will be sent as-is. If data is an
    int, it will be converted to an unsigned 8-bit integer and sent that way (attempting to write an
    integer outside of the range 0-255 is an error). If data is a string it will be utf-8 encoded.
    """
    self._log(f"Writing data (may need conversion to bytes): {data}")

    if not isinstance(data, bytes):
      if isinstance(data, str):
        return self.write(data.encode('utf-8'))
      elif isinstance(data, int):
        return self.write(data.to_bytes(1, 'big', signed=True))
      else:
        raise Exception("Cannot write data of unknown type!")
    else:
      self.serialPort.write(data)
  
  def writeln(self, data):
    """
    Write a string to the Arduino over Serial, and add a newline at the end.
    """
    return self.write(f"{data}\n")