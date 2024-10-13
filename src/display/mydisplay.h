#ifndef MYDISPLAY_H
#define MYDISPLAY_H

#include <FastLED.h>

// Farben der LEDs
#define COL_BL CRGB(0, 0, 0)
const CRGB GELB = CRGB(247, 255, 12);
const CRGB RED = CRGB(255, 0, 0);
const CRGB GREEN = CRGB(68, 214, 44);
const CRGB BLUE = CRGB(5, 93, 255);

// Symbole für die Zwischen-LEDs
#define S_COMMA 0b1100
#define S_SEMI 0b1101
#define S_ON 0b1111
#define S_OFF 0b0000

extern const uint8_t patterns[6];
extern const byte digits[21];

const int PIXELS_PER_SEGMENT = 3;
const int PIXELS_DIGITS = 6;
const int PIXELS_EXTRA = 8;
const int PIXELS_SUM = (PIXELS_PER_SEGMENT * 7 * PIXELS_DIGITS) + PIXELS_EXTRA;

#define DATA_PIN 12     // Pin für die LEDs

extern CRGB leds[PIXELS_SUM]; // Deklaration des globalen LED Arrays

// Funktionsdeklarationen
void initializeLEDs();
void updateLEDs();
void setLEDBrightness(uint8_t brightness);
void sortChar(const char* alphanumeric, char dotL, char dotR, CRGB pixColo);
void writeDigit(int index, int val, CRGB pixColo);
void writePoints(char left, char right, CRGB pixColo);

#endif // MYDISPLAY_H
