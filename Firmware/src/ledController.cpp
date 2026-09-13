#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


const int ledPin = D0;
const int numLeds = 4;

bool ledsInitialized = false;

//changeable led stuff
int ledBrightness = 50;

// create the strip of led
Adafruit_NeoPixel strip(numLeds, ledPin, NEO_GRB + NEO_KHZ800);

// function to set up the led strip, will run during setup of main.cpp
void setupLeds()
{
    strip.begin();
    strip.show(); // make sure all led start off
    strip.setBrightness(ledBrightness); // tune for looks
    ledsInitialized = true;
}


// set every LED to the same color doesnt apply changes. rn showleds
void setAllLeds(uint8_t r, uint8_t g, uint8_t b)
{
    if (ledsInitialized)
    {
        for (int i = 0; i < numLeds; i++)
        {
            strip.setPixelColor(i, strip.Color(r, g, b));
        }
    }
}
 
// turn all leds off immediately
void clearLeds()
{
    if (ledsInitialized)
    {
        strip.clear();
        strip.show();
    }
}
 
// push whatever colors were set to led strip
void showleds()
{
    if (ledsInitialized)
    {
        strip.show();
    }
}

//set brightness of leds
void setLedBrightness(int brightness){
    if(ledsInitialized){
        ledBrightness = brightness;
        strip.setBrightness(brightness);
    }
}
