# A simple termios example written in C
I will configure an UART defice connected to a usb port (`/dev/ttyUSB0`) and send some data to it and receive data from it.

# Build Instructions
First change the `SERIAL_PORT` definition to your serial port.
Compile using
```
make
```
(make should already be installed)
Execute using 
```
./main
```
