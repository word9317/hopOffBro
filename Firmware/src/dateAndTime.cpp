#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include "secrets.h"

const char *ntpServer = "pool.ntp.org";
const char *TZ_INFO = "PST8PDT,M3.2.0,M11.1.0";

// Function Declarations
void connectToWiFi()
{
    WiFi.begin(SECRET_SSID, SECRET_PASSWORD);
    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 100) // ~10s
    {
        delay(100);
        retry++;
    }
}

void syncNTPTime()
{
    configTzTime(TZ_INFO, ntpServer);

    struct tm timeinfo;
    int retry = 0;
    while (!getLocalTime(&timeinfo) && retry < 10)
    {   
        Serial.println("Waiting for NTP time sync...");
        delay(1000);
        retry++;
    }
}

// Returns time string  (e.g., "15:30:25")
String getTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Error: Time not set";
  }
  
  char buffer[32];
  strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);
  return String(buffer);
}

// Returns date string  (e.g., "2026-01-01")
String getDateString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Error: Date not set";
  }
  
  char buffer[32];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
  return String(buffer);
}