import serial
import struct
import time

def init_Serial():
    print("Opening Serial Port COM 10")

    ser = serial.Serial(
        port='COM10',
        baudrate=115200,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS
    )
    return ser

def wait_for_Pi(ser_i):
    print("Wait until Raspberry Pi is ready")

    cnt = 0
    while cnt < 3:
        x = ser_i.read()
        print(": %s" %x.encode("hex"))
        if x == '\x03':
            cnt = cnt + 1

    print("Raspberry is ready")


def open_Kernel():
    print("Opening File")

    fid = open("..\05_uart0\kernel8.img", "rb")


    # Get binaries
    data = fid.read()

    # Get file size
    f_size = fid.tell()
    print("Filesize: %d" % f_size)

    fid.close();
    return f_size, data


def send_Kernel_size(ser_i, size):
    print("Send Kernel size to RPI")
    data = struct.pack('<i',size)

    for i in data:
        ser_i.write(i)

    print("Waiting for Acknowledgment")
    recv = ser_i.read(2)
    if recv == "OK":
        print("Received Acknowledgment")
    else:
        print("Error after sending size")
        print("restart")
        return False

    return True



def send_Kernel(ser_i, kernel_data):
    print("sending Kernel...")
    print(len(kernel_data))
	
    for tmp, byte in enumerate(kernel_data):
        ser_i.write(byte)
        print(tmp+1)
		
		# Check if sended byte == received byte
        #recv = ser_i.read(1)
        #print(": %s == %s" % (recv.encode("hex"), byte.encode("hex")))
		
		#check the size
        """recv = ser_i.read(4)
        print(": %s" % recv[0].encode("hex"))
        print(": %s" % recv[1].encode("hex"))
        print(": %s" % recv[2].encode("hex"))
        print(": %s" % recv[3].encode("hex"))"""

    print("finished sending")

	# check if Raspberry Pi is sending a charakter after the while-loop
    #test = ser_i.read(1)
    #print(": %s" % test.encode("hex"))
    #print("Successfull")

    return True


def main():
    print("Serial Transmit Kernel.img")

    ser_i = init_Serial()
    wait_for_Pi(ser_i)

    size, kernel_data = open_Kernel()
    guard = send_Kernel_size(ser_i, size)

    if(guard == True):
        send_Kernel(ser_i, kernel_data)


if __name__ == "__main__":
    main()
