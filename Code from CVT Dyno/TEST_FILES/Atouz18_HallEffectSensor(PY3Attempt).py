# Author: Andrew Touzinsky
# Date: March 4th, 2017
#
# This reads a hall effect sensor on GPIO17 according
# to the switch on GPIO27.
#----------------------------------------------------------
import RPi.GPIO as GPIO
import time
import datetime
global time_file
global stamp
global text_file

def sensorCallback1(channel):  # Called if sensor output goes LOW
  stamp = str(time.time()) #datetime.datetime.fromtimestamp(stamp).strftime('%H:%M:%S.%f') #The brackets can be added to define the decimal acuracy[:-2]
#  try:
  text_file.write(stamp + '\n')
  print('Sensor LOW ' + stamp)
#  except:
#    print('Sensor not writing')
#    print(stamp)
    
def switchCallback1(channel): # Called once if switch status changes
  time.sleep(0.1)
  if GPIO.input(27) == True:
    global text_file
    file_name = 'HallEffectData.' + time.strftime('%Y.%m.%d-%H.%M.%S') + '.txt'
    text_file = open(file_name, "w")
    print('File Open')
  elif GPIO.input(27) == False:
    try:
      text_file.close()
      print('File Closed')
    except:
      print('File Already Closed')

def main():
  # Wrap main content in a try block so we can catch the user pressing CTRL-C and run the GPIO cleanup function. This will also prevent the user seeing lots of unnecessary error messages.
  try:    # Loop until users quits with CTRL-C
    while True :
      time.sleep(0.0005) 

  except KeyboardInterrupt:
    # Reset GPIO settings
    GPIO.cleanup()

# Tell GPIO library to use GPIO references
GPIO.setmode(GPIO.BCM)
print('Setup GPIO pin as input')

# Set Switch GPIO as input
GPIO.setup(17 , GPIO.IN)
GPIO.add_event_detect(17, GPIO.FALLING, callback=sensorCallback1)
GPIO.setup(27, GPIO.IN)
GPIO.add_event_detect(27, GPIO.BOTH, callback=switchCallback1, bouncetime=200)#bouncetime is measured in miliseconds

if __name__=="__main__":
   main()
