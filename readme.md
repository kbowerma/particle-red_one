# Red One
this is on P1 redboard from sparkfun so I have modified the master make file and adpated the ```build``` to a new ```p1build``` which includes ```PLATFORM=P1 PRODUCT_ID=8 ```.  You have to remember to do a ```make clean``` if you forget and do just a ```make build``` instead of a ```make p1build```

pin | Function | pin | Function
----| ------- ----| -------
VIN| - | 3v3 | -
GND| - | RST | -
Tx| - | VBAT | -
Rx| - | GND | -
WKP| - | D7 | -
DAC| - | D6 | -
A5| - | D5 | -
A4| - | D4 | -
A3| - | D3 | -
A2| - | D2 | -
A1| - | D1 | -
A0| - | D0 | DHT22


## Components
### DHT-22
[RHT03 aka DHT-2](http://cdn.sparkfun.com/datasheets/Sensors/Weather/RHT03.pdf)  and [Sparkfun link](https://www.sparkfun.com/products/10167)


pin | Function
----|---------
1 | VDD
2 | Data
3 | NULL
4 | GND

### [SX1509 I/O Expander Breakout BoB 13601](https://www.sparkfun.com/products/13601)
![](https://cdn.sparkfun.com//assets/parts/1/0/9/5/6/13601-01.jpg)

**Description**: Are you low on I/O? No problem! The SX1509 Breakout is a 16-channel GPIO expander with an I2C interface – that means with just two wires, your microcontroller can interface with 16 fully configurable digital input/output pins. But the SX1509 can do so much more than just simple digital pin control. It can produce PWM signals, so you can dim LEDs. It can be set to blink or even breathe pins at varying rates. And, with a built-in keypad engine, it can interface with up to 64 buttons set up in an 8x8 matrix.

Two headers at the top and bottom of the breakout board function as the input and control headers to the board. This is where you can supply power to the SX1509, and where your I2C signals – SDA and SCL – will terminate. GPIO and power buses are broken out in every-which direction, and configurable jumpers cover most of the rest of the board.

Since the I/O banks can operate between 1.2V and 3.6V (5.5V tolerant) independent of both the core and each other, this device can also work as a level-shifter. The SX1509 breakout makes it easy to prototype so you can add more I/O onto your Arduino or I/O limited controller. We’ve even spun up an Arduino Library to get you started!
