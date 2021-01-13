#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN        2
#define NUMPIXELS 300

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels.begin();

  //draw player 1 start position
  pixels.setPixelColor(0, pixels.Color(0, 150, 0));
  pixels.setPixelColor(1, pixels.Color(0, 150, 0));
  pixels.setPixelColor(2, pixels.Color(0, 150, 0));
  pixels.show();
}

//startposition
int player1LastPosition = 2;
int player1CurrentPosition = 2;

//Draw players
void draw(int playerLastPosition, int playerCurrentPosition)
{
  int playerLastPositionLocal = playerLastPosition;
  //While player real location and player drawn location differ
  while(playerLastPositionLocal != playerCurrentPosition)
  {
    //if player real position is in front of current drawn position
    if(playerLastPositionLocal < playerCurrentPosition)
    {
      //unset last entity pixel
      int lastEntityPixel = playerLastPosition - 2;
      pixels.setPixelColor(lastEntityPixel, pixels.Color(0, 0, 0));

       //set next pixels
       int nextEntityPixel = playerLastPosition + 1;
       pixels.setPixelColor(nextEntityPixel, pixels.Color(0, 150, 0));

       pixels.show();
       //update playerLastPosisition
       playerLastPositionLocal++;
    }
  }
}

void update()
{
  delay(300);
  player1CurrentPosition++;
}

void loop()
{
  draw(player1LastPosition, player1CurrentPosition);
  update();


}
