
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> //Display Libaries

#define SCREEN_WIDTH 128 // Set OLED display width, in pixels
#define SCREEN_HEIGHT 64 // Set LED display height, in pixels

void setup() {
  //Setup and Clear
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C0);
  delay(500);
  display.clearDisplay();




}

void loop() {
  // put your main code here, to run repeatedly:

}
