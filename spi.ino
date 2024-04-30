/************************************************************
Developed by Saurav Sajeev
sauravsajeev007@gmail.com

If you are not using the LCD display, you may disable 
the selected lines by commenting
************************************************************/

#include <SPI.h>
#include <TFT.h>

#define cs 10	 //disable if you are not using the display
#define dc 9	 //disable if you are not using the display
#define rst 8	 //disable if you are not using the display

#define SCLK 13
#define SDIO 11

int pID = 0, resArr[49];
int rectLPX = 0, rectLPY = 0;
char str[2];
TFT scr = TFT(cs, dc, rst); 	//disable if you are not using the display

void setup() {

  Serial.begin(9600);
  scr.begin();

  pinMode(SCLK, OUTPUT);
  digitalWrite(SCLK, HIGH);

  pinMode(6, OUTPUT);

  pinMode(SDIO, INPUT);
  digitalWrite(SDIO, LOW);

  scr.background(0, 0, 0);	 //disable if you are not using the display

  writeToSPI(0x80 | 0x05, 0xA0);  //disable sleep mode
  writeToSPI(0x80 | 0x06, 0x07);  //set dpi 1600
  writeToSPI(0x80 | 0x09, 0x5A);  //to enable r/w in registers 0x0A ~ 0x7F
  writeToSPI(0x80 | 0x0D, 0x04);  //set image threshold to 4
  writeToSPI(0x80 | 0x0E, 0xE0);  //set read recogonition rate to HIGH
}

void loop() {
  pID = transmitViaSerial(0x00, 0x00);
  Serial.print("Product ID: 0x");
  Serial.print(pID, HEX);
  Serial.println(transmitViaSerial(0x01, 0x00), HEX);
  Serial.println("--------------------------");
  Serial.print("Sleep Mode: 0x");
  Serial.println(transmitViaSerial(0x05, 0x00), HEX);
  Serial.print("Motion detect: ");
  if (transmitViaSerial(0x02, 0x00) >= 0x84)
    Serial.println("IN MOTION");
  else
    Serial.println("IDLE");
  Serial.print("Delta X: ");
  Serial.print(transmitViaSerial(0x03, 0x00));
  Serial.print("\tDelta Y: ");
  Serial.println(transmitViaSerial(0x04, 0x00));
  Serial.print("Image quality: ");
  Serial.println(transmitViaSerial(0x07, 0x00));
  Serial.print("Operation state: ");
  Serial.println(transmitViaSerial(0x08, 0x00));
  Serial.print("Image threshold: ");
  Serial.println(transmitViaSerial(0x0D, 0x00));
  Serial.print("Image recogonition rate: ");
  Serial.println(transmitViaSerial(0x0E, 0x00));
  Serial.println();
  for (int i = 0x0F; i <= 0x40; i++) {  //disable if you are not using the display
    scr.text("\0\0", 100, 100);
    resArr[i - 0x0F] = transmitViaSerial(i, 0x00);
    scr.fill(resArr[i - 0x0F], resArr[i - 0x0F], resArr[i - 0x0F]);
    scr.rect(rectLPX, rectLPY, 10, 10);
    if (i == 0x0F) {
      scr.setTextSize(2);
      scr.stroke(30, 30, 255);
      scr.setTextSize(2);
      scr.text("MX8650A", 40, 85);
      scr.stroke(0, 255, 255);
      scr.setTextSize(1);
      scr.text("7x7 image from internal", 15, 105);
      scr.text("registers", 50, 115);
      scr.noStroke();
    }
    rectLPX += 10;
    if (i % 7 == 0) {
      rectLPX = 0;
      rectLPY += 10;
    }
  }
  rectLPX = 0;
  rectLPY = 0;
}

byte transmitViaSerial(uint8_t addr, uint8_t wBit) {
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE1));
  digitalWrite(6, HIGH);
  delay(10);
  digitalWrite(SCLK, LOW);

  SPI.transfer(addr);
  pinMode(SDIO, INPUT);
  byte data = SPI.transfer(wBit);

  pinMode(SDIO, OUTPUT);
  digitalWrite(SCLK, HIGH);
  SPI.endTransaction();
  digitalWrite(6, LOW);
  SPI.end();
  return data;
}

void writeToSPI(uint8_t addr, uint8_t data) {
  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE1));
  digitalWrite(6, HIGH);
  delay(10);
  digitalWrite(SCLK, LOW);
  pinMode(SDIO, OUTPUT);
  SPI.transfer(addr);
  Serial.println(SPI.transfer(data), HEX);
  SPI.endTransaction();
  digitalWrite(6, LOW);
  SPI.end();
}
