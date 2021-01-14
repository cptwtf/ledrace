
#include <Wire.h> 
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> //Display Libaries

#define SCREEN_WIDTH 128 // Set OLED display width, in pixels
#define SCREEN_HEIGHT 64 // Set LED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

byte m = constrain(m,0,6);
byte w = constrain(w,0,5);
bool s = 1;
int a; int b; int c; int d; int e; 
