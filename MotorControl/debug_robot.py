# scan_port = True
# if scan_port:
# 	import serial.tools.list_ports
# 	ports = list(serial.tools.list_ports.comports())
# 	count = 0
# 	# for p in ports:
# 	# 	print(str(p))
# 	avr_port_name = str(ports[1])
# 	port_number = avr_port_name[18:18+5]

# 	import serial 
# 	import time 
# 	port_val= '/dev/cu.usbserial-' + str(port_number)
# 	arduino = serial.Serial(port=port_val, baudrate=9600,  timeout=.1) 
# 	def write_read(x):
# 		# x += "x\n"
# 		time.sleep(1)
# 		# print(x)
# 		# arduino.write(bytes(x,   'utf-8'))
# 		data = arduino.readline() 
# 		return data 
# 	print("Begin..")
# 	while True: 
# 		num = "w-255--\n"
# 		byte_string = write_read(num)
# 		print(byte_string)
# 		regular_string = byte_string.decode('utf-8')
# 		print(regular_string) # printing the value 

potValue = 955
a = ((potValue-6) * 255 ) / 1018
print(a)