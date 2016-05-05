import smbus
bus = smbus.SMBus(1)
address = 0x08

class Robot:
  def __init__(self):
    self.LEFT = 0x80
    self.RIGHT = 0x80
    self.SPEED = 1

  def write(self,b4=1):
    bus.write_i2c_block_data(address,0x00,[self.LEFT,self.RIGHT,self.SPEED,b4])
  
  def joystick(self,x,y):
    self.LEFT = ((x-y)/2) + 127
    self.RIGHT = ((x+y)/2) + 127
    self.write()

  def speed(self, speed):
    self.SPEED = speed
    self.write()

  def left(self):
    self.LEFT += 0x10
    self.write()

  def right(self):
    self.RIGHT += 0x11
    self.write()

  def turnright(self):
    self.LEFT += 0x10
    self.RIGHT -= 0x11
    self.write()

  def turnleft(self):
    self.LEFT -= 0x10
    self.RIGHT += 0x11
    self.write()

  def forward(self):
    self.LEFT -= 0x10
    self.RIGHT -= 0x11
    self.write()

  def backward(self):
    self.LEFT += 0x10
    self.RIGHT += 0x11
    self.write()
  
  def stop(self):
    self.RIGHT = 0x80
    self.LEFT = 0x80
    self.write()

robot = Robot()
robot.speed(4)

def f():
  robot.forward()
def b():
  robot.backward()

def l():
  robot.left()
def r():
  robot.right()

def s():
  robot.stop()

def tl():
  robot.turnleft()
def tr():
  robot.turnright()
