void player1screen(){
  display.clearDisplay();
  display.setFont();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  int x = 1; int y = 1;
  display.setCursor(x,y);
  display.print("Geschwindigkeit");
  x = 100; y = 1;
  display.setCursor(x,y);
  display.print("=");
  x = 110; y = 1;
  display.setCursor(x,y);
  display.print(g);
  
  x = 1; y = 10;
  display.setCursor(x,y);
  display.print("Runde");
  x = 100; y = 10;
  display.setCursor(x,y);
  display.print("=");
  x = 110; y = 10;
  display.setCursor(x,y);
  display.print(r);
  
  x = 1; y = 19;
  display.setCursor(x,y);
  display.print("Rundenzeit");
  x = 100; y = 19;
  display.setCursor(x,y);
  display.print("=");
  x = 110; y = 19;
  display.setCursor(x,y);
  display.print(rz);
  display.display();

  }
  
void player2screen(){
  display2.clearDisplay();
  display2.setFont();
  display2.setTextSize(1);
  display2.setTextColor(WHITE);
  int x = 1; int y = 1;
  display2.setCursor(x,y);
  display2.print("Geschwindigkeit");
  x = 100; y = 1;
  display2.setCursor(x,y);
  display2.print("=");
  x = 110; y = 1;
  display2.setCursor(x,y);
  display2.print(g2);
  
  x = 1; y = 10;
  display2.setCursor(x,y);
  display2.print("Runde");
  x = 100; y = 10;
  display2.setCursor(x,y);
  display2.print("=");
  x = 110; y = 10;
  display2.setCursor(x,y);
  display2.print(r2);
  
  x = 1; y = 19;
  display2.setCursor(x,y);
  display2.print("Rundenzeit");
  x = 100; y = 19;
  display2.setCursor(x,y);
  display2.print("=");
  x = 110; y = 19;
  display2.setCursor(x,y);
  display2.print(rz2);
  display2.display();
  }
