



#include <Wire.h> 
#include <Adafruit_SSD1306.h> //Display Libary
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN        32
#define NUMPIXELS 300
#define SCREEN_WIDTH 128 // Set OLED display width, in pixels
#define SCREEN_HEIGHT 64 // Set LED display height, in pixels

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


int g = 100;
int r = 11;
int rz = 5;
int g2 = 200;
int r2 = 21;
int rz2 = 15;
byte m ;
byte w ;
bool s = 1;
bool gamestate = false;
int a; int b; int c; int d; int e; 
