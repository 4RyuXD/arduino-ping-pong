# arduino-ping-pong
Play ping pong on a MAX7219 module with a joystick and an LCD Screen to keep track of points and welcome/try again messages

### Components
Arduino UNO R3 or any microcontroller\
MAX7219 Module\
Joystick Module\
LCD1602 Module\
10kOhm Potentiometer\
220Ohm Resistor\
Breadboard\
Male to Female Jumper Wires\
Male to Male Jumper Wires\

### Libraries
LedControl by Eberhard Fahle\
LiquidCrystal by Arduino, Adafruit\

## IMPORTANT
If you are using a different microcontroller other than the UNO R3 then, change these pins on your hardware and code accordingly\
CLK - SPI Serial Clock pin (UNO: 13, MEGA: 52)\
RS - SPI MISO/POCI pin (UNO: 12, MEGA: 50)\
E - SPI MOSI/PICO pin (UNO: 11, MEGA: 51)\
