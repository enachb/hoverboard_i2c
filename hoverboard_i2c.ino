// program to control hoverboard over i2c

#include <Wire.h>

#include <SoftwareSerial9.h>

#define TX1 4
#define RX1 5
#define TX2 6
#define RX2 7

#define I2CADDR 8
#define DEBUG

SoftwareSerial9 wheel1(RX1, TX1);
SoftwareSerial9 wheel2(RX2, TX2);

const byte statuses[] = {0x55, 0xAA}; //[unlocked, immobile]
signed int speed1 = 0, speed2 = 0;
bool immobile = 0;
unsigned long lastUpdate = 0;

void setup() {
  Wire.begin(I2CADDR);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  Serial.println("*************************** READY");         // print the integer

  // put your setup code here, to run once:
  wheel1.begin(26315);
  wheel2.begin(26315);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  updateWheelSpeed();
}

// Callback receiving left and right speed integers in one transmission
void receiveEvent(int howMany) {
  int16_t spLeft = 0;
  int16_t spRight = 0;

  byte sp1_1 = Wire.read(); // receive byte as a character
  byte sp1_2 = Wire.read(); // receive byte as a character

  byte sp2_1 = Wire.read(); // receive byte as a character
  byte sp2_2 = Wire.read(); // receive byte as a character

  spLeft = sp1_1;
  spLeft = spLeft << 8 | sp1_2;

  spRight = sp2_1;
  spRight = spRight << 8 | sp2_2;

  setWheelSpeed(spLeft, spRight);

#ifdef DEBUG
  Serial.print("Received ");         // print the value
//  Serial.print(sp1_1, HEX);         // print the value
//  Serial.print(sp1_2, HEX);         // print the value
  Serial.print("Left: ");
  Serial.print(spLeft);         // print the value
  Serial.print(" Right:");
  Serial.print(spRight);         // print the value
  Serial.print(" ");
  Serial.print(howMany);         // print the value
  Serial.print("\n");
#endif
}

void setWheelSpeed(signed int spd1, signed int spd2) {
  speed1 = spd1;
  speed2 = spd2;
}

void updateWheelSpeed() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  wheel1.write9(0x100);                   //initialization of data packet
  wheel1.write9(speed1 & 0xFF);           //send low byte
  wheel1.write9((speed1 >> 8) & 0xFF);    //send high byte
  wheel1.write9(speed1 & 0xFF);           //repeat low byte
  wheel1.write9((speed1 >> 8) & 0xFF);    //repeat high byte
  wheel1.write9(statuses[immobile]);      //send immobile state
  //same thing for the second wheel
  wheel2.write9(0x100);                   //initialization of data packet
  wheel2.write9((-speed2) & 0xFF);        //send low byte
  wheel2.write9(((-speed2) >> 8) & 0xFF); //send high byte

  wheel2.write9((-speed2) & 0xFF);        //repeat low byte
  wheel2.write9(((-speed2) >> 8) & 0xFF); //repeat high byte
  wheel2.write9(statuses[immobile]);      //send immobile state
  delayMicroseconds(300);
  lastUpdate = millis();
//  digitalWrite(LED_BUILTIN, LOW);
}


