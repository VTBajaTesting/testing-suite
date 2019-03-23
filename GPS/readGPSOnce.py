def read_once():
	import serial
	import Adafruit_BBIO.UART as UART
	UART.setup("UART4")
	GPS=serial.Serial('/dev/ttyO4',9600)
	str=GPS.readline();
	print(str)
	return str
