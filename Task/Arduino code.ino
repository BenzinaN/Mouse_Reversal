/****************************************************
   TouchScreens control code by Nabil BENZINA
 ****************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>       // this is needed for display
#include <Adafruit_ILI9341.h>
#include <Wire.h>      // this is needed for FT6206
#include <Adafruit_FT6206.h>

// The FT6206 uses hardware I2C (SCL/SDA) but i don't need it if i use the interrupt pin (except for sensitivity tweaking)
Adafruit_FT6206 tactile = Adafruit_FT6206();

// The displays also uses hardware SPI, plus #5 & #4
#define CS 5
#define DC 4
Adafruit_ILI9341 ecran = Adafruit_ILI9341(CS, DC);

// We will use these pins to interface with Med Associates equipments
const int fig1 = A0; // input
const int fig2 = A1; // input
const int succes = A2; // input
const int touch = 1; // output

// some variables
int touch_time;
int last_touch_time;

void setup() {

  // Initiate Med output pins
  pinMode(touch, OUTPUT);
  digitalWrite(touch, LOW);

  // Initiation of interrupt pin (the int.2 = 0)
  attachInterrupt(2, appui, LOW);

  // Initiate touch part and set the threshold
  tactile.begin(10); // number in () is the threshold value (from 0 to 255)

  // Initiate the TFT
  ecran.begin();
  ecran.setRotation(0);
  ecran.fillScreen(ILI9341_BLACK);
}

void loop () {
  while (analogRead(fig1) < 100 && analogRead(fig2) < 100 && analogRead(succes) < 100) {
    ecran.fillRect(0, 0, 240, 240, ILI9341_WHITE);
  }
  ecran.fillRect(0, 0, 240, 240, ILI9341_BLACK);
  if (analogRead(fig1) > 100 && analogRead(fig2) > 100 && analogRead(succes) > 100) ecran.fillRect(0, 0, 240, 240, ILI9341_BLACK);
  if (analogRead(fig1) < 100 && analogRead(fig2) > 100 && analogRead(succes) > 100) grid();
  ecran.fillRect(0, 0, 240, 240, ILI9341_BLACK);
  if (analogRead(fig1) > 100 && analogRead(fig2) < 100 && analogRead(succes) > 100) line();
  ecran.fillRect(0, 0, 240, 240, ILI9341_BLACK);
  if (analogRead(fig1) < 100 && analogRead(fig2) < 100 && analogRead(succes) > 100) oblique(130, 66, 174);
  ecran.fillRect(0, 0, 240, 240, ILI9341_BLACK);
  while (analogRead(succes) < 100 && analogRead(fig1) > 100 && analogRead(fig2) > 100) {
    feedback();
  }
}

void oblique(int largeur, int ymin, int ymax) {
  int miLargeur = largeur / 2;
  for (int y = ymin; y < ymax; y++) {
    if (y % largeur > miLargeur)
      ecran.drawFastHLine(0, y, y % largeur - miLargeur, ILI9341_WHITE);
    for (int x = y % largeur; x < 240; x = x + largeur) {
      if (x + miLargeur > 240)
        ecran.drawFastHLine(x, y, 240 - x, ILI9341_WHITE);
      else
        ecran.drawFastHLine(x, y, miLargeur, ILI9341_WHITE);
    }
  }
  for (;;) {
    if (analogRead(fig1) > 100 || analogRead(succes) < 100) return;
  }
}

void line() {
  int space = 3;
  for (int x = 0; x <= 240; x = x + space + 6) {
    for (int xBis = x; xBis <= x + 6; xBis++) {
      ecran.drawFastVLine(xBis, 0, 240, ILI9341_WHITE);
    }
    space = space + (space / 2);
  }
  for (;;) {
    if (analogRead(fig2) > 100 || analogRead(succes) < 100) return;
  }
}

void grid() {
  for (int x = 2; x <= 240; x = x + 58) {
    if (x >= 235) {
      x = 236;
    }
    if (x < 3) {
      x = 3;
    }
    for (int ep = 0; ep <= 3; ep++) {
      ecran.drawFastVLine(x + ep, 0, 240, ILI9341_WHITE);
      ecran.drawFastVLine(x - ep, 0, 240, ILI9341_WHITE);
      ecran.drawFastHLine(0, x + ep, 240, ILI9341_WHITE);
      ecran.drawFastHLine(0, x - ep, 240, ILI9341_WHITE);
    }
  }
  for (;;) {
    if (analogRead(fig1) > 100 || analogRead(succes) < 100) return;
  }
}

void appui() {
  //if (tactile.touched()) {
  touch_time = millis();
  // check to see if appui() was called in the last 200 milliseconds
  if (touch_time - last_touch_time > 200) {
    digitalWrite(touch, HIGH);
    delayMicroseconds(15000);
    delayMicroseconds(10000);
    delayMicroseconds(15000);
    delayMicroseconds(10000);
    delayMicroseconds(15000);
    delayMicroseconds(10000);
    delayMicroseconds(15000);
    delayMicroseconds(10000);
    digitalWrite(touch, LOW);
    touch_time = touch_time + 100;
  }
  last_touch_time = touch_time;
  //}
}

void feedback() {
  ecran.fillRect(40, 0, 160, 240, ILI9341_WHITE);
  ecran.fillRect(40, 0, 160, 240, ILI9341_BLACK);
}











