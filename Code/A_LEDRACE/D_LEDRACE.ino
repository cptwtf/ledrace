

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
  if      (a == 1) {
    m = m + 1;
  }
  else if (b == 1) {
    m = m - 1;
  }
  else if (c == 1) {
    w = w + 1;
  }
  Serial.println(m);
  Serial.println(s);
}

void abfrage2()
{
  Read();
  if (a == 1) {
    s = 0;
  }
  Read();
  if (b == 1) {
    s = 1;
  }
  Read();
  if (c == 1 && s == 1) {
    w = w + 1;
  }
  Read();
  if (c == 1 && s == 0) {
    w = w - 1;
  }
  Serial.println(m);
  Serial.println(s);
}
