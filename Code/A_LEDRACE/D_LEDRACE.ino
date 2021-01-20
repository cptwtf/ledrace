
void display1setup(){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.drawBitmap(0,0, pixelartbztg, 128, 64, 1);
  display.display();
  }

void display2setup(){
  display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display2.clearDisplay();
  display2.drawBitmap(0,0, pixelartbztg, 128, 64, 1);
  display2.display();
  }

void display12idle(){
  display2.clearDisplay();
  display2.drawBitmap(0,0, wartenart1, 128, 64, 1);
  display2.display();
}

void display1menue(){
   while(m == 1 && w == 1 || m < 1 && w == 1 || m == 6 && w == 1 || m > 6 && w == 1)//start
  {
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart7, 128, 64, 1);
  display.display();
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart2, 128, 64, 1);
  display.display();
  resetcount();
  abfrage();
}

 while(m == 2 && w == 1)//multispieler 
  {
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart8, 128, 64, 1);
  display.display();
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart3, 128, 64, 1);
  display.display();
  resetcount();
  abfrage();
}


 while(m == 2 && w == 2 && s == 1)//multispieler JA 
  {
  delay(300);
  display.clearDisplay();
  display.drawBitmap(0,0, spielenmenuart2, 128, 64, 1);
  display.display();
  abfrage2();
}
 while(m == 2 && w == 2 && s == 0)//multispieler NEIN
  {
  delay(300);
  display.clearDisplay();
  display.drawBitmap(0,0, spielenmenuart3, 128, 64, 1);
  display.display();
  abfrage2();
}
while(m == 2 && w == 3)//Logo
  {
  delay(300);
  display.clearDisplay();
  display.drawBitmap(0,0, logoart1, 128, 64, 1);
  display.display();
  light();

}
while(m == 3 && w == 1)//einzelspieler
  {
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart9, 128, 64, 1);
  display.display();
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart4, 128, 64, 1);
  display.display();
  resetcount();
  abfrage();
}

 while(m == 3 && w == 2 && s == 1)//einzelspieler JA 
  {
  delay(300);
  display.clearDisplay();
  display.drawBitmap(0,0, spielenmenuart2, 128, 64, 1);
  display.display();
  abfrage2();
}
 while(m == 3 && w == 2 && s == 0)//einzelspieler NEIN
  {
  delay(300);
  display.clearDisplay();
  display.drawBitmap(0,0, spielenmenuart3, 128, 64, 1);
  display.display();
  abfrage2();
}
while(m == 3 && w == 3)//einzelspielerLogo
  {
  delay(300);
  display.clearDisplay();
  display.drawBitmap(0,0, logoart1, 128, 64, 1);
  display.display();
  light();

}

while(m == 4 && w == 1)//optionen
  {
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart10, 128, 64, 1);
  display.display();
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart5, 128, 64, 1);
  display.display();
  resetcount();
  abfrage();
}

while(m == 5 && w == 1)//infos
  {
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart11, 128, 64, 1);
  display.display();
  display.clearDisplay();
  delay(300);
  display.drawBitmap(0,0, menuart6, 128, 64, 1);
  display.display();
  resetcount(); 
  abfrage();
}

 while(m == 5 && w == 2 && s == 1)//infobase 
  {
  delay(300);
  display.clearDisplay();
  display.drawBitmap(0,0, infoart2, 128, 64, 1);
  display.display();
  abfrage3();
}

 while(m == 5 && w == 2 && s == 0)//inforeturn 
  {
  delay(300);
  display.clearDisplay();
  display.drawBitmap(0,0, infoart3, 128, 64, 1);
  display.display();
  abfrage3();
}
}
