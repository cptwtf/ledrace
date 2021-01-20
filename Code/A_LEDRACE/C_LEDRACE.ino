void setup() {
  //Setup, Clear and BZTGLogo
  Serial.begin(115200);
  display1setup();
  display2setup();
  delay(2000);
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();
  m = 1;
  w = 1;
}

void loop() {
 if (gamestate = false){
      player1screen();
      player2screen();
 }else{
      display12idle();
      display1menue();
 }
  
}
