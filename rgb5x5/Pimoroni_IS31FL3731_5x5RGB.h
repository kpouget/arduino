#ifndef _PIMORONI_IS31FL3731_5X5RGB_H_
#define _PIMORONI_IS31FL3731_5X5RGB_H_

#include <Adafruit_IS31FL3731.h>

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                     \
  {                                             \
    int16_t t = a;                              \
    a = b;                                      \
    b = t;                                      \
  }
#endif

class Pimoroni_IS31FL3731_5x5RGB : public Adafruit_IS31FL3731
{
public:
  Pimoroni_IS31FL3731_5x5RGB(void)
    : Adafruit_IS31FL3731(15, 5) {}

  void Pimoroni_IS31FL3731_5x5RGB::drawPixel(int16_t x, int16_t y, uint16_t color) {
    int _x = x;
    int _y = y;

    // check rotation, move pixel around if necessary
    switch (getRotation()) {
    case 1:
      _swap_int16_t(x, y);
      x = 16 - x - 1;
      break;
    case 2:
      x = 16 - x - 1;
      y = 9 - y - 1;
      break;
    case 3:
      _swap_int16_t(x, y);
      y = 9 - y - 1;
      break;
    }

    // if ((x < 0) || (x >= 16))
    //   return;
    // if ((y < 0) || (y >= 9))
    //   return;
    if (color > 255)
      color = 255; // PWM 8bit max
    //added jack
    if (y > 5) {
      x = x + 8;
      y = y - 6;
    }

    Serial.print("x: ");
    Serial.print(_x);
    Serial.print(", y: ");
    Serial.print(_y);
    Serial.print(" --> ");
    Serial.print(x + y * 16);
    Serial.println(";");
    //added jack end
    setLEDPWM(x + y * 16, color, _frame);
    return;
  }

};
#endif
