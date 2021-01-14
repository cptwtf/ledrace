void setup() {
  //Setup, Clear and BZTGLogo
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(500);
  display.clearDisplay();
  display.drawBitmap(0,0, pixelartbztg, 128, 64, 1);
  display.display();
  delay(2000);



}

void loop() {
  
  while(m == 1)//start
  {
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart7, 128, 64, 1);
  display.display();
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart2, 128, 64, 1);
  display.display();
  abfrage();
}


 while(m == 2)//einzelspieler
  {
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart8, 128, 64, 1);
  display.display();
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart3, 128, 64, 1);
  display.display();
  abfrage();
}

while(m == 3)//multispieler
  {
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart9, 128, 64, 1);
  display.display();
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart4, 128, 64, 1);
  display.display();
  abfrage();
}

while(m == 4)//optionen
  {
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart10, 128, 64, 1);
  display.display();
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart5, 128, 64, 1);
  display.display();
  abfrage();
}

while(m == 5)//infos
  {
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart11, 128, 64, 1);
  display.display();
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart6, 128, 64, 1);
  display.display();
  abfrage();
}
}
