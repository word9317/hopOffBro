#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

// connects to WiFi using SECRET_SSID / SECRET_PASSWORD from secrets.h.
// gives up after ~10s if it can't connect.
void connectToWiFi();

// starts NTP sync against pool.ntp.org using the TZ_INFO offset/DST rule.
// call this after connectToWiFi() succeeds. retries for up to ~10s.
void syncNTPTime();

// returns the current local time as "HH:MM:SS", or an error string if
// the time hasn't been synced yet.
String getTimeString();

// returns the current local date as "YYYY-MM-DD", or an error string if
// the time hasn't been synced yet.
String getDateString();
