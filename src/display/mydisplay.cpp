#include "mydisplay.h"
#include <ctype.h>
#include <string.h>
#include <FastLED.h>

// Definition der externen Variablen
const uint8_t patterns[6] = {
    0b1100,  // ","
    0b1001,  // "."
    0b1010,  // ":"
    0b1101,  // ";"
    0b1111,  // "|"
    0b0000   // " "
};

const byte digits[21] = {
    0b0111111,  // 0
    0b0000110,  // 1
    0b1011011,  // 2
    0b1001111,  // 3
    0b1100110,  // 4
    0b1101101,  // 5
    0b1111101,  // 6
    0b0000111,  // 7
    0b1111111,  // 8
    0b1101111,  // 9
    0b1110111,  // A
    0b1111100,  // B
    0b0111001,  // C
    0b1011110,  // D
    0b1111001,  // E
    0b1110001,  // F
    0b1110110,  // H
    0b0111000,  // L
    0b1011100,  // o
    0b1110011,  // P
    0b0111110   // U
};

// Globales LED-Array basierend auf der konstanten PIXELS_SUM
CRGB leds[PIXELS_SUM];

// Funktion zur Initialisierung der LEDs
void initializeLEDs() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN, RGB>(leds, PIXELS_SUM);  // LED-Streifen initialisieren
}

// Funktion zum Einstellen der Helligkeit
void setLEDBrightness(uint8_t brightness) {
    FastLED.setBrightness(brightness);  // Helligkeit einstellen
}

// Funktion zum Aktualisieren der LED-Anzeige
void updateLEDs() {
    FastLED.show();  // Aktualisiert die Anzeige der LEDs
}

void sortChar(const char* alphanumeric, char dotL, char dotR, CRGB pixColo) {
    int length = strlen(alphanumeric);
    for (int j = length - 1; j >= 0; j--) {
        char currentChar = alphanumeric[j];
        int val;
        if (isdigit(currentChar)) {
            val = currentChar - '0';
        } else if (isalpha(currentChar)) {
            val = 10 + (toupper(currentChar) - 'A');
        } else {
            continue; // Ignore non-alphanumeric characters
        }
        writeDigit(j, val, pixColo);
        if (j == 0) {
            writePoints(dotL, dotR, pixColo);
        }
    }
}

void writeDigit(int index, int val, CRGB pixColo) {
    byte digit = digits[val];
    if (index < PIXELS_DIGITS) {
        int offset = index * (PIXELS_PER_SEGMENT * 7);
        for (int i = 0; i < 7; i++) {
            int pixelIndex = offset + i * PIXELS_PER_SEGMENT;
            CRGB color = (digit & (1 << i)) ? pixColo : COL_BL;
            for (int j = 0; j < PIXELS_PER_SEGMENT; j++) {
                leds[pixelIndex + j] = color;
            }
        }
    }
}

void writePoints(char left, char right, CRGB pixColo) {
    const char characters[6] = {',', '.', ':', ';', '|', ' '};
    uint8_t index_left = 0;
    uint8_t index_right = 0;

    for (int i = 0; i < 6; ++i) {
        if (right == characters[i]) {
            index_right = i;
            break;
        }
    }

    for (int i = 0; i < 6; ++i) {
        if (left == characters[i]) {
            index_left = i;
            break;
        }
    }

    byte combinedByte = (patterns[index_left] << 4) | patterns[index_right];
    int offset = PIXELS_DIGITS * (PIXELS_PER_SEGMENT * 7); // offset = 126

    for (int i = 0; i < PIXELS_EXTRA; i++) {
        int pixelIndex = offset + i;
        CRGB color = (combinedByte & (1 << i)) ? pixColo : COL_BL;
        for (int j = 0; j < PIXELS_PER_SEGMENT; j++) {
            leds[pixelIndex + j] = color;
        }
    }
}
