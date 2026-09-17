#include <Arduino.h>
#include <screenController.h> //custom screen management script
#include <ledController.h> //custom led controller
#include <dateAndTime.h> // date and time via wifi script

const int swLeft = D1;
const int swMid = D2;
const int swRight = D3;

const int buzzerPin = D7;

unsigned long lastSyncMillis = 0;
const unsigned long syncInterval = 6UL * 60 * 1000; // resync every 6 hours

// enums are halla tuff, and im gonan use one for clocks state:
enum clockState { 
  clockIdle
};

clockState currentState = clockIdle;

String lastTimeShown = "";
String lastDateShown = "";

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
  // woaggg
  if (millis() - lastSyncMillis >= syncInterval) {
    connectToWiFi();
    syncNTPTime();
    disconnectWiFi();
    lastSyncMillis = millis();
  }

  switch (currentState) {
    case clockIdle:
      showIdleScreen();
      break;
  }
}