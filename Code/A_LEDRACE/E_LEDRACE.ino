void player1screen(){
  display.setCursor(1,1);
  display.setTextColor(0xFFFF);
  display.write("Geschwindigkeit = ",g);
  display.setCursor(128,1);
  display.setTextColor(0xFFFF);
  display.write("Runde = ",r);
  display.setCursor(1,32);
  display.setTextColor(0xFFFF);
  display.write("Rundenzeit = ",rz);
  display.setCursor(128,32);
  display.setTextColor(0xFFFF);
  display.write("Geschwindigkeit = ",g);
  display.display();
  }
  
void player2screen(){
  display2.setCursor(1,1);
  display2.setTextColor(0xFFFF);
  display2.write("Geschwindigkeit = ",g);
  display2.setCursor(128,1);
  display2.setTextColor(0xFFFF);
  display2.write("Runde = ",r);
  display2.setCursor(1,32);
  display2.setTextColor(0xFFFF);
  display2.write("Rundenzeit = ",rz);
  display2.setCursor(128,32);
  display2.setTextColor(0xFFFF);
  display2.write("Geschwindigkeit = ",g);
  display2.display();
  }
