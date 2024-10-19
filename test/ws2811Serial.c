/* From: https://github.com/PaulStoffregen/WS2812Serial/tree/master */
#include <WS2812Serial.h>

const int numled = 63;
const int pin = 1;

byte drawingMemory[numled*3];
DMAMEM byte displayMemory[numled*12];

WS2812Serial leds(numled, displayMemory, drawingMemory, pin, WS2812_GRB);

#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define YELLOW 0xFFFF00
#define PINK   0xFF1088
#define ORANGE 0xE05800
#define WHITE  0xFFFFFF

void setup() {
  leds.begin();
}

void loop() {
  // Display numbers 0 to 9
  for (int i = 0; i <= 9; i++) {
    displayNumber(i, 500000);
    delay(1000);  // Delay for 1 second between numbers
  }
}

void displayNumber(int num, int wait) {
  switch (num) {
    case 0:
      colorWipe(RED, wait);
      break;
    case 1:
      colorWipe(GREEN, wait);
      break;
    case 2:
      colorWipe(BLUE, wait);
      break;
    case 3:
      colorWipe(YELLOW, wait);
      break;
    case 4:
      colorWipe(PINK, wait);
      break;
    case 5:
      colorWipe(ORANGE, wait);
      break;
    case 6:
      colorWipe(WHITE, wait);
      break;
    // Add cases for 7 to 9 as needed
    default:
      // Handle unsupported numbers or do nothing
      break;
  }
}

void colorWipe(int color, int wait) {
  for (int i = 0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
    delayMicroseconds(wait);
  }
}
