
  
void Read()
{
    a = digitalRead(19);
    b = digitalRead(18);
    c = digitalRead(5);
  //  d = digitalRead(3);
  //  e = digitalRead(4);
  //  f = digitalRead(5);
}

void abfrage()
{
  Read();
  if (a == 1){ Serial.println(2);}
  else if (b == 1){ Serial.println(0);}
  else if (c == 1){ Serial.println(4);}
}
