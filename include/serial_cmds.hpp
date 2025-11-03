#pragma once
#include "config.hpp"
#include <ArduinoJson.h>
#include <Arduino.h>


// Call once per loop; non-blocking line reader.
void handleSerial();

// Expose global config & hits (provided by main)
extern BadgeConfig CFG;
extern JsonDocument hitsDoc;
