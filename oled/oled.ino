#include "OLED_I2C.h"
OLED myOLED(SDA, SCL, 8);

extern uint8_t TinyFont[];
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

void setup(){
   myOLED.begin();
}

// 128×64
void loop(){


   myOLED.setFont(SmallFont);
   myOLED.print("X:", LEFT, 5);
   myOLED.print("Y:", LEFT, 25);
   myOLED.print("Z:", LEFT, 50);

   myOLED.setFont(MediumNumbers);
   myOLED.print("123456789", 10, 0);
   myOLED.print("123456789", 10, 20);
   myOLED.print("123456789", 10, 45);

   myOLED.update();
   delay(1000);
   myOLED.clrScr();

   myOLED.setFont(SmallFont);
   myOLED.print("X:", LEFT, 5);
   myOLED.print("Y:", LEFT, 25);
   myOLED.print("Z:", LEFT, 50);

   myOLED.setFont(MediumNumbers);
   myOLED.print("000000000", 10, 0);
   myOLED.print(String(1234), 10, 20);
   myOLED.print("9999", 10, 45);
   myOLED.update();
   delay(1000);
   /*
   myOLED.setFont(TinyFont);
   myOLED.print("ABCDEFGHIJKLMNOPQRSTUVWXYZ", LEFT, 10);
   myOLED.update();
   delay(1000);

   myOLED.clrScr();
   myOLED.update();
   myOLED.invert(true);
   delay(1000);

   myOLED.setFont(MediumNumbers);
   myOLED.print("123456789", RIGHT, 5);
   myOLED.update();
   delay(1000);

   myOLED.setFont(BigNumbers);
   myOLED.print("123456789", 5, 30);
   myOLED.update();
   delay(1000);

   myOLED.clrScr();
   myOLED.update();
   myOLED.invert(false);
   delay(1000);
   */
}
