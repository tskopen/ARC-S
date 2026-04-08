/*
 * Header file for ARC-S Flash Chip Controller
 *
 * Name:      Thor Kopenkoskey
 * Created:   January 15, 2025
 * Edited:    November 15, 2025
 */

#include <Preferences.h>


void setupFlash();
void logData(float roll, float pitch, float yaw, float alt);
void closeFlash();
void startLogging();
void transmitLogs();


