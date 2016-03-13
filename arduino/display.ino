
//  End all serial transmissions with +
//  90: Shut off backlight           80:  Status LED Off
//  91: Turn on backlight            81:  Status LED Red
//  92: Internet Active              82:  Status LED Green
//  93: Internet Not Active          83:  Status LED Blue
//  94: Wifi Quality                 84:  Status LED Yellow
//  95: Wifi Signal Strength         89:  Status LED White
//  96:
//  97: Temp
//  98:
//  99: Clear LCD

#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
Adafruit_NeoPixel statled = Adafruit_NeoPixel(1, 12, NEO_GRB + NEO_KHZ800);  // Status LED

String inData;
uint8_t bell[8]  = {0x4, 0xe, 0xe, 0xe, 0x1f, 0x0, 0x4};
uint8_t note[8]  = {0x2, 0x3, 0x2, 0xe, 0x1e, 0xc, 0x0};
uint8_t clock[8] = {0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0};
uint8_t heart[8] = {0x0, 0xa, 0x1f, 0x1f, 0xe, 0x4, 0x0};
uint8_t duck[8]  = {0x0, 0xc, 0x1d, 0xf, 0xf, 0x6, 0x0};
uint8_t check[8] = {0x0, 0x1 ,0x3, 0x16, 0x1c, 0x8, 0x0};
uint8_t cross[8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};
uint8_t retarrow[8] = {0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};
byte bar4[8] = { 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};
byte bar3[8] = { 0b00000, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};
byte bar2[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111};
byte bar1[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111};
byte bar0[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};

void statusled(byte color) {
  switch (color) {
    case 0:
      statled.setPixelColor(0, 0, 0, 0);
      break;
    case 1:
      statled.setPixelColor(0, 255, 0, 0);
      break;
    case 2:
      statled.setPixelColor(0, 0, 255, 0);
      break;
    case 3:
      statled.setPixelColor(0, 0, 0, 255);
      break;
    case 4:
      statled.setPixelColor(0, 255, 255, 0);
      break;
    case 5:
      statled.setPixelColor(0, 255, 0, 255);
      break;
    case 9:
      statled.setPixelColor(0, 255, 255, 255);
      break;
  }
  statled.show();
}

void setup() 
{
  Serial.begin(9600);  // Used to type in characters
  lcd.begin(20,4);         // initialize the lcd for 20 chars 4 lines, turn on backlight
  lcd.noBacklight(); // finish with backlight on 
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.createChar(0, bar0);
  lcd.createChar(1, bar1);
  lcd.createChar(2, bar2);
  lcd.createChar(3, bar3);
  lcd.createChar(4, bar4);
  lcd.createChar(6, check);
}

void loop()  { 
    while (Serial.available() > 0)
    {
        char recieved = Serial.read();
        inData += recieved;
        if (recieved == '+')
        {
            inData.remove(inData.length()-1,1);
            byte y = inData.charAt(1) - '0';
            byte x = inData.charAt(2) - '0';
            inData.remove(0,3);
//            Serial.print("Arduino Received: ");
//            Serial.println(inData);
          if (y == 8) {  //Status LED Commands
            switch (x) {
              case 0: statusled(0); break;
              case 1: statusled(1); break;
              case 2: statusled(2); break;
              case 3: statusled(3); break;
              case 4: statusled(4); break;
              case 5: statusled(5); break;              
          }
          } else if (y == 9) {  // LCD Commands
            switch (x) {
              case 1: lcd.backlight(); break;
              case 0: lcd.noBacklight(); break;
              case 9: lcd.clear(); break;
              case 7: lcd.setCursor(16,3); // Temp
                 if (inData.length() == 2) {
                  inData=" "+inData;
                 }
                 lcd.print(inData);
                 lcd.print((char)223);
                 break;
              case 2: lcd.setCursor(0,3);// Internet Active
                 lcd.write((uint8_t)6);
                 break;
              case 3: lcd.setCursor(0,3);// Internet Inactive
                 lcd.write((uint8_t)0);
                 break;
              case 5: {  lcd.setCursor(10,3); 
                 inData=inData+"dBm";
                 lcd.print(inData);
                 int dinData=inData.toInt();
                 int str = map(dinData, 90, 30, 0, 4);
                 lcd.setCursor(5,3);
                switch (str) {
                  case 0: lcd.write((uint8_t)0); lcd.write((uint8_t)0); lcd.write((uint8_t)0); lcd.write((uint8_t)0); break;
                  case 1: lcd.write((uint8_t)1); lcd.write((uint8_t)0); lcd.write((uint8_t)0); lcd.write((uint8_t)0); break;
                  case 2: lcd.write((uint8_t)1); lcd.write((uint8_t)2); lcd.write((uint8_t)0); lcd.write((uint8_t)0); break;
                  case 3: lcd.write((uint8_t)1); lcd.write((uint8_t)2); lcd.write((uint8_t)3); lcd.write((uint8_t)0); break;
                  case 4: lcd.write((uint8_t)1); lcd.write((uint8_t)2); lcd.write((uint8_t)3); lcd.write((uint8_t)4); break;
                 }
                 break; }
              case 4:{
                 int dinData=inData.toInt();
                 int str = map(dinData, 0, 70, 0, 100);
                lcd.setCursor(1,3);
                String stri=String(str);
                if (stri.length() == 2) {
                  stri=" "+stri;
                 }
                lcd.print(stri);
                 break;
              }
            }
          } else {
            lcd.setCursor(x-1,y-1);
            lcd.print(inData);
        }
      inData = ""; // Clear recieved buffer
    }
  }
}
