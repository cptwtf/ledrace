void setup() {
  //Setup and Clear
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(500);
  display.clearDisplay();
  display.drawBitmap(0,0, image_data_array, 128, 64, 1);
  display.display();



}

void loop() {
  // put your main code here, to run repeatedly:

}
