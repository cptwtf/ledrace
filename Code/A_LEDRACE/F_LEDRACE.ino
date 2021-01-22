void readButtons(){
  readingA = digitalRead(buttonA);
  readingB = digitalRead(buttonB);
  readingC = digitalRead(buttonC);
}
void button (){
  
  readButtons();
  
  if (readingA != lastButtonStateA) {
    lastDebounceTime = millis();
  }
  if (readingB != lastButtonStateB) {
    lastDebounceTime = millis();
  }
  if (readingC != lastButtonStateC) {
    lastDebounceTime = millis();
  }
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (readingA != buttonStateA) {
      buttonStateA = readingA;
    if (readingB != buttonStateB) {
      buttonStateB = readingB;
    if (readingC != buttonStateC) {
      buttonStateC = readingC;
      }
    }
  }
  lastButtonStateA = readingA;
  lastButtonStateB = readingB;
  lastButtonStateC = readingC;
 }
}
