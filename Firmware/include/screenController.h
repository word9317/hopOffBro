#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

extern bool screenInitialized;

void setupTFT();              
void setScreenText(String message, uint8_t textSize, int cursorX, int cursorY);void clearScreen();
void screenBacklightOn();
void screenBacklightOff();
void disconnectWiFi();