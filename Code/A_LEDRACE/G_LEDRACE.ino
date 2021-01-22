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
  

void abfrage0()
{
  button();
  if (readingA == 1)            m = m + 1;
  if (readingB == 1)           m = m - 1;
} 
void abfrage()
{
  abfrage0(); 
  if (readingC == 1)           w = w + 1;
}

void abfrage2()
{
  button();
  if (readingA == 1)           s = 0;
  if (readingB == 1)           s = 1;
  if (readingC == 1 && s == 1) w = w + 1;
  if (readingC == 1 && s == 0) w = w - 1;
  }

void abfrage3()
{
  button();
  if (readingA == 1)           s = 0;
  if (readingB == 1)           s = 1;
  if (readingC == 1 && s == 0) w = w - 1;
}

void abfrage4()
{
  button();
  if (readingA == 1)           n = n + 1;
  if (readingB == 1)           n = n - 1;
  if (readingC == 1)           w = w + 1;
}
