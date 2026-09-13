#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

extern bool ledsInitialized; // true once setupLeds() has run

void setupLeds();                                  // call once in setup()
void setAllLeds(uint8_t r, uint8_t g, uint8_t b);  // set every LED to one color, call showleds() after
void clearLeds();                                   // turn all LEDs off immediately
void showleds();                                    // push buffered colors out to the strip
void setLedBrightness(int brightness);              // doesn't apply until showleds() is called