#include <Arduino.h>
#include <screenController.h> //custom screen management script
#include <ledController.h> //custom led controller
#include <dateAndTime.h> // date and time via wifi script


// pin declarations
const int swLeft = D1;
const int swMid = D2;
const int swRight = D3;

const int buzzerPin = D7;

// time and resyncing stuff
unsigned long lastSyncMillis = 0;
const unsigned long syncInterval = 60UL * 60 * 1000; // resync every hour


// enums are halla tuff, and im gonan use one for clocks state:
enum clockState { 
  clockIdle,
  timerRunning,
  timerFinished
};

// timer variable declarations
unsigned long timerStartMillis = 0;
unsigned long timerDurationMillis = 0;
uint8_t timerTargetR, timerTargetG, timerTargetB; // color the leds as time gets closer.
bool timerDoneScreenDrawn = false;
unsigned long lastFlashMillis = 0;
bool flashOn = false;
const unsigned long flashInterval = 400; // ms

// button edge detection
bool wasSwLeftPressed = false;
bool wasSwMidPressed = false;
bool wasSwRightPressed = false;

clockState currentState = clockIdle;

String lastTimeShown = "";
String lastDateShown = "";

//helper function for handling button presses isntead of digital reading everywherr.
bool buttonPressed(int pin, bool &wasPressed) {
  bool isPressed = digitalRead(pin) == HIGH;
  bool triggered = isPressed && !wasPressed;
  wasPressed = isPressed;
  return triggered;
}

void showIdleScreen() {
  String timeStr = getTimeString();
  String dateStr = getDateString();

  if (timeStr != lastTimeShown) {
    setScreenText(timeStr, 4, 0, 20);     // left display the time, large n shii
    lastTimeShown = timeStr;
  }

  if (dateStr != lastDateShown) {
    setScreenText(dateStr, 2, 170, 30);   // show the date on the right, js a little smaller
    lastDateShown = dateStr;
  }
}

// timer function
void startTimer(unsigned long minutes, uint8_t r, uint8_t g, uint8_t b) {
  timerDurationMillis = minutes * 60UL * 1000UL;
  timerStartMillis = millis();
  timerTargetR = r;
  timerTargetG = g;
  timerTargetB = b;
  timerDoneScreenDrawn = false;
  lastTimeShown = ""; // resets screen so it is redrawn
  currentState = timerRunning;
}

void showTimerRunning(){
  unsigned long elapsed = millis() - timerStartMillis;

  if(elapsed >= timerDurationMillis){
    //check if time has elapsed, clear leds then
    currentState = timerFinished;
    clearLeds();
    return;
  }
  
  //interpolate color based on how much time is left.
  float progress = (float)elapsed / (float)timerDurationMillis; // used for interpolating color
  uint8_t r = 255 + (int)((timerTargetR - 255) * progress);
  uint8_t g = 255 + (int)((timerTargetG - 255) * progress);
  uint8_t b = 255 + (int)((timerTargetB - 255) * progress);

  setAllLeds(r, g, b);
  showleds();

  unsigned long remainingSec = (timerDurationMillis - elapsed) / 1000;
  int mm = remainingSec / 60;
  int ss = remainingSec % 60;
  char buf[6];
  snprintf(buf, sizeof(buf), "%02d:%02d", mm, ss);
  String remainingStr = String(buf);

  if (remainingStr != lastTimeShown) {
    setScreenText(remainingStr, 4, 0, 20);
    lastTimeShown = remainingStr;
  }
}

void showTimerDone(){
  if(!timerDoneScreenDrawn){
    clearScreen();
    setScreenText("hop off bro!", 3, 20, 100);
    timerDoneScreenDrawn = true;
  }

  //flasing lights
  unsigned long now = millis();
  if(now-lastFlashMillis >= flashInterval){
    lastFlashMillis = now;
    flashOn = !flashOn;
    
    if(flashOn){
      setAllLeds(0, 255, 0);
      showleds();
      screenBacklightOn();
      tone(buzzerPin, 1000);
    } else {
      clearLeds();
      screenBacklightOff();
      noTone(buzzerPin);
    }
  }
}

void setup() {
  // set up switches
  pinMode(swLeft, INPUT);
  pinMode(swMid, INPUT);
  pinMode(swRight, INPUT);

  //setup buzzer thingy
  pinMode(buzzerPin, OUTPUT);

  // initialize subsystems(i feel so fancy writing that)
  setupLeds();
  setupTFT();
  connectToWiFi();
  syncNTPTime();
  disconnectWiFi();
  lastSyncMillis = millis();
}

void loop() {
  // hourly WiFi resync check
  if (millis() - lastSyncMillis >= syncInterval) {
    connectToWiFi();
    syncNTPTime();
    disconnectWiFi();
    lastSyncMillis = millis();
  }

  bool leftPressed = buttonPressed(swLeft, wasSwLeftPressed);
  bool midPressed = buttonPressed(swMid, wasSwMidPressed);
  bool rightPressed = buttonPressed(swRight, wasSwRightPressed);

  switch (currentState) {
    case clockIdle:
      if(leftPressed){
        startTimer(20, 255, 0, 0);
      } else if (rightPressed){
        startTimer(30, 0, 0, 255);
      }
      showIdleScreen();
      break;
    
    case timerRunning:
      if(midPressed){
        clearLeds();  
        currentState = clockIdle;
      }
      showTimerRunning();
      break;
    
    case timerFinished:
      showTimerDone();
      if(leftPressed || midPressed || rightPressed) {
        noTone(buzzerPin);
        clearLeds();
        screenBacklightOn();
        timerDoneScreenDrawn = false;
        currentState = clockIdle;
      }
      break;
  }
}