void resets(){
  if(s != 1) { s = 1;}
  }

void resetcount(){
  if(m > 6) { m = 1;}
  if(m < 1) { m = 6;}
  resets();
  }
  
 void resetcount2(){
  if(n > 5) { n = 1;}
  if(n < 1) { n = 5;}
  resets();
  }
  
void Read()
{
  a = digitalRead(19);
  b = digitalRead(18);
  c = digitalRead(5);
  //  d = digitalRead(3);
  //  e = digitalRead(4);
  //  f = digitalRead(5);
}

void abfrage0()
{
  Read();
  if(a == 1)            m = m + 1;
  if (b == 1)           m = m - 1;
} 
void abfrage()
{
  abfrage0(); 
  if (c == 1)           w = w + 1;
}

void abfrage2()
{
  Read();
  if (a == 1)           s = 0;
  if (b == 1)           s = 1;
  if (c == 1 && s == 1) w = w + 1;
  if (c == 1 && s == 0) w = w - 1;
  }

void abfrage3()
{
  Read();
  if (a == 1)           s = 0;
  if (b == 1)           s = 1;
  if (c == 1 && s == 0) w = w - 1;
}

void abfrage4()
{
  Read();
  if (a == 1)           n = n + 1;
  if (b == 1)           n = n - 1;
  if (c == 1)           w = w + 1;
}
