// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);           // start serial for output
  Serial.println("*************************** READY");
}

int16_t speed1 = 0;
int16_t speed2 = 0;
char c = ' ';
signed int sp = 0;

void loop() {

  Serial.println(c);
  if (c == ' ') {
    sp = 0;
  } else if (c == 'q') {
    sp -= 10;
  } else if (c == 'w') {
    sp += 10;
  } else if (c == '2') {
    sp += 100;
  }  else if (c == '1') {
    sp -= 100;
  }

  do {
    byte myArray[4];

    speed1 = sp;
    speed2 = sp;

    myArray[0] = (speed1 >> 8) & 0xFF;
    myArray[1] = speed1 & 0xFF;

    myArray[2] = (speed2 >> 8) & 0xFF;
    myArray[3] = speed2 & 0xFF;

//    Serial.print(myArray[0], HEX);
//    Serial.print("  ");
//    Serial.println(myArray[1], HEX);

    Serial.print("speed ");
    Serial.print(sp);
    Serial.print(" low byte ");
    Serial.print((sp & 0xFF), HEX);
    Serial.print(" high byte ");
    Serial.println((sp >> 8) & 0xFF, HEX);

    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(myArray, 4);              // sends 4 bytes
    Wire.endTransmission();    // stop transmitting

  } while (!Serial.available());
  c = Serial.read();
  delayMicroseconds(500);
}
