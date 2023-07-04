
/*
  GY-271 Compass
  modified on 02 Sep 2020
  by Mohammad Reza Akbari @ Electropeak
  Home
*/

// I2C Library
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Lib.h"
// QMC5883L Compass Library
#include <QMC5883LCompass.h>
#include "OLED_I2C.h"

OLED myOLED(SDA, SCL, 8);
Pimoroni_IS31FL3731_5x5RGB matrix = Pimoroni_IS31FL3731_5x5RGB();

extern uint8_t TinyFont[];
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

QMC5883LCompass compass;

int variation_threshold = 50;

int MIN_COLOR_INTENSITY = 1;
int MAX_COLOR_INTENSITY = 32;

int PIN_SET_EARTH = 13;

int first = 1;

int current_x;
int current_y;
int current_z;

int prev_x = 0;
int prev_y = 0;
int prev_z = 0;


int color_intensity(int strength, int min, int max) {
  return ((strength - min) / (max - min)) * (MAX_COLOR_INTENSITY - MIN_COLOR_INTENSITY) + MIN_COLOR_INTENSITY;
}

void set_color(int r, int g, int b) {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      matrix.drawPixel(i + 1, j + 1, r, g, b);
    }
  }
}

void set_color_block(int r, int g, int b, int x, int dx, int y, int dy) {
  for (int i = x; i < (x + dx); i++) {
    for (int j = y; j < (y + dy); j++) {
      matrix.drawPixel(i + 1, j + 1, r, g, b);
    }
  }
}

void setup() {
  // Initialize the serial port.
  Serial.begin(9600);
  // Initialize I2C.
  Wire.begin();

  // Initialize the Compass.
  compass.init();

  myOLED.begin();
  myOLED.clrScr();

  matrix.begin();
  matrix.clear();

  pinMode(PIN_SET_EARTH, INPUT);
}

// 128×64
void to_screen(int diff_x, int diff_y, int diff_z) {
  return;
  myOLED.clrScr();

  myOLED.setFont(MediumNumbers);

  myOLED.print(diff_x, 5, 20);
  myOLED.print(diff_x, 15, 20);
  myOLED.print(diff_x, 25, 20);

  myOLED.update();
}
int max_x = 0, max_y = 0, max_z = 0;
void to_console(int diff_x, int diff_y, int diff_z) {
  char buffer[40];

   sprintf(buffer, "%+5d %+5d %+5d", current_x, current_y, current_z);
   Serial.println(buffer);
}

void to_led(int diff_x, int diff_y, int diff_z) {
  if (diff_x > variation_threshold) {
    // R, G, *B*
    set_color_block(0, 0, MAX_COLOR_INTENSITY, 0, 3, 0, 2);
  } else if (diff_x < -variation_threshold) {
    // R, *G*, B
    set_color_block(MAX_COLOR_INTENSITY, 0, 0, 0, 3, 0, 2);
  } else {
    // *R*, G, B
    set_color_block(0, 0, 0, 0, 3, 0, 2);
  }

  if (diff_y > variation_threshold) {
    // R, G, *B*
    set_color_block(0, 0, MAX_COLOR_INTENSITY, 0, 3, 3, 2);
  } else if (diff_y < -variation_threshold) {
    // R, *G*, B
    set_color_block(MAX_COLOR_INTENSITY, 0, 0, 0, 3, 3, 2);
  } else {
    // *R*, G, B
    set_color_block(0, 0, 0, 0, 3, 3, 2);
  }

  if (diff_z > variation_threshold) {
    // R, G, *B*
    set_color_block(0, 0, MAX_COLOR_INTENSITY, 3, 2, 1, 3);
  } else if (diff_z < -variation_threshold) {
    // R, *G*, B
    set_color_block(MAX_COLOR_INTENSITY, 0, 0, 3, 2, 1, 3);
  } else {
    // *R*, G, B
    set_color_block(0, 0, 0, 3, 2, 1, 3);
  }
}

#define DELAY 200
void loop() {
  compass.read();

  current_x = compass.getX();
  current_y = compass.getY();
  current_z = compass.getZ();

  if (!first) {
    to_console(current_x - prev_x, current_y - prev_y, current_z - prev_z);
    to_screen(current_x - prev_x, current_y - prev_y, current_z - prev_z);
    to_led(current_x - prev_x, current_y - prev_y, current_z - prev_z);
  } else {
    first = 0;
    Serial.print("Go! ");
    Serial.println(DELAY);
  }

  prev_x = current_x;
  prev_y = current_y;
  prev_z = current_z;


  delay(DELAY);
}
