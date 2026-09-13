#include <Arduino.h>
#include <Adafruit_GFX.h>    // graphics library
#include <Adafruit_ST7789.h> // driver for the ST7789 screen
#include <SPI.h>

// the screens pins(hopefully right)
const int TFT_SCLK = 0; // labeled SCL on the screen
const int TFT_MOSI = 1; // labeled SDA on the screen
const int TFT_RST = 2;
const int TFT_DC = 3;
const int TFT_CS = 4;
const int TFT_BL = 5;

bool screenInitialized = false;
// create a class for the screen
class MyST7789 : public Adafruit_ST7789
{
public:
    MyST7789(int8_t cs, int8_t dc, int8_t mosi, int8_t sclk, int8_t rst)
        : Adafruit_ST7789(cs, dc, mosi, sclk, rst) {}
    void setOffsets(uint8_t col, uint8_t row)
    {
        _colstart = _colstart2 = col;
        _rowstart = _rowstart2 = row;
    }
};
// give the tft screen its pin values
MyST7789 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// set text on the tft display
void setScreenText(String message, uint8_t textSize)
{
    if (screenInitialized)
    {
        tft.fillScreen(ST77XX_BLACK); // clear the screen
        tft.setTextSize(textSize);    // set text size
        tft.setTextColor(ST77XX_WHITE);  // set the color
        tft.setCursor(0, 0);          // cursor reset
        tft.print(message);         // set message on clock to the message inputed into function
    }
}

//clear le screen
void clearScreen(){
    if(screenInitialized){
        tft.fillScreen(ST77XX_BLACK); //clear the screen
    }
}

//functions for backlight
void screenBacklightOn(){
    digitalWrite(TFT_BL, LOW);
}
void screenBacklightOff(){
    digitalWrite(TFT_BL, HIGH);
}

// function to set up the TFT display, will run during setup of main.cpp
void setupTFT()
{
    pinMode(TFT_BL, OUTPUT);   // Set the backlight pin mode, or just wire it to 3.3V
    digitalWrite(TFT_BL, LOW); // Turns the backlight ON, for some reason this screen is active Low, so setting it to LOW is really HIGH

    // ok now we init le screen
    tft.init(76, 284);
    tft.setOffsets(82, 18);
    tft.invertDisplay(false);
    tft.setRotation(1); // might have to change to 3 if display is flipped
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(6);
    tft.setCursor(0, 0);
    screenInitialized = true;
}