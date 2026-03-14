/*
 * Header file for ARC-S SD Card logging Controller
 *
 * Name:      Thor Kopenkoskey
 * Created:   January 11, 2025
 * Edited:    January 15, 2025
 */

#include <Preferences.h>
#include "telemetry.h"

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

void initializeSDCard();

void writeToSDCard();
