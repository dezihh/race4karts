#include <Wire.h>
//#include "SPI.h"
#include "mydisplay.h"

// Schalter
const int butLeft = 9;
const int butRight = 10;

#define DATA_PIN 12

extern CRGB leds[PIXELS_SUM];

int mynum;
char a2Num[7]; // 6 Zeichen für die Umwandlung von Int auf String + 1 Zeichen für das Nullzeichen

int ms = 0, sec = 0, minute = 0;
const unsigned long interval = 10; // Interval in Millisekunden (hier 1/100 Sekunde)
bool stopwatchRunning = false; // Variable zur Verfolgung, ob die Stoppuhr läuft
int combinedTime = 0;
unsigned long previousLoopTime = 0;
unsigned long previousSerialTime = 0;


void writeHello(const char* infoText, CRGB pixColo);
void timePrint();
void sortCharFromInt(int mytime, char* alphanumeric);

void setup() {
  Serial.begin(9600);


  pinMode(butLeft, INPUT_PULLUP);
  pinMode(butRight, INPUT_PULLUP);
  Serial.println("Starting");
  //LEDS.addLeds<NEOPIXEL,DATA_PIN,RGB>(leds,PIXELS_PER_SEGMENT * 7 * PIXELS_DIGITS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, PIXELS_SUM);
  //LEDS.addLeds<WS2812SERIAL,DATA_PIN,RGB>(leds,PIXELS_PER_SEGMENT * 7 * PIXELS_DIGITS);
  LEDS.setBrightness(84);
  writeHello((const char*)"GAHH0", RED);
  FastLED.show();
  sortChar("000000", '|', '|', GELB);
  }


void loop() {
  FastLED.show();
  // Lesen des Zeichens aus der seriellen Schnittstelle
  if (Serial.available() > 0) {
    char input = Serial.read();
    if (input == 'r') {
      // Reset der Stoppuhr, wenn 'r' eingegeben wird
      ms = sec = minute = 0;
      sortChar("000000", '|', '|', GREEN);
      //writeHello("000000");
      Serial.println("Stoppuhr zurückgesetzt");
      FastLED.show();
      previousSerialTime = millis(); // Zurücksetzen der Zeit
      stopwatchRunning = false; // Stoppuhr anhalten
    } else if (input == 's') {
      // Starten oder Anhalten der Stoppuhr bei Eingabe von 's'
      Serial.println("Stoppuhr läuft");
      stopwatchRunning = !stopwatchRunning;
      if (stopwatchRunning) {
        // Wenn die Stoppuhr gestartet wird, aktualisieren Sie die vorherige Zeit
        previousSerialTime = millis();
      }
    } else if (input == 'x') {
      // Stoppen der Stoppuhr ohne Zurücksetzen, wenn 'x' eingegeben wird
      Serial.println("Stoppuhr angehalten. Aktueller Wert:" + String(combinedTime));
      stopwatchRunning = false;
    }
  }
  
    timePrint();

}

void writeHello(const char* infoText, CRGB pixColo) {
    sortChar(infoText, ' ', ' ', pixColo);
}



void timePrint() {
  unsigned long currentLoopTime = millis(); // Aktuelle Zeit abrufen

  if (stopwatchRunning && currentLoopTime - previousLoopTime >= interval) {
    // Stoppuhr läuft und Zeitintervall ist vergangen
    previousLoopTime = currentLoopTime;

    // Zeitberechnung aktualisieren
    ms += (currentLoopTime - previousSerialTime);
    previousSerialTime = currentLoopTime;

    // Überprüfen, ob eine Sekunde vergangen ist
    if (ms >= 1000) {
      ms -= 1000;
      sec++;
      if (sec >= 60) {
        sec = 0;
        minute++;
      }
    }
    combinedTime = minute * 100000 + sec * 1000 + ms;
    // Integer in Zeichenkette umwandeln
    sortCharFromInt(combinedTime, a2Num);
    // Zeichenkette ausgeben
    sortChar(a2Num, '|', '|', GREEN);
  }
}

// Funktion zur Umwandlung von Integer in Zeichenkette
void sortCharFromInt(int mytime, char* alphanumeric) {
    sprintf(alphanumeric, "%06d", mytime);
}

