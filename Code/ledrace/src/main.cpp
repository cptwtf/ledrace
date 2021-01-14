#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN        32
#define NUMPIXELS 300

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  Serial.begin(115200);
  Serial.println("debug setup");
  pixels.begin();

  pixels.clear();
  //draw player 1 start position
  pixels.setPixelColor(0, pixels.Color(0, 150, 0));
  pixels.setPixelColor(1, pixels.Color(0, 150, 0));
  pixels.setPixelColor(2, pixels.Color(0, 150, 0));
  pixels.show();
}

//startposition
int player1DrawPosition = 2;
int player1LogicPosition = 2;
byte player1ColorArrayRGB[] = {0,0,255};

//Draw players
void draw(int playerDrawPosition, int playerLogicPosition, byte playerColorArrayRGB[])
{
  Serial.println("debug draw");
  int playerDrawPositionLocal = playerDrawPosition;
  //While player real location and player drawn location differ
  while(playerDrawPositionLocal != playerLogicPosition)
  {
    Serial.println("debug draw while");
    //if player real position is in front of current drawn position
    if(playerDrawPositionLocal < playerLogicPosition)
    {
      //Strip End/Beginning handling
      int sternmostPixel;
      if(playerDrawPositionLocal == 0 ){ sternmostPixel = NUMPIXELS - 2; }
      else if (playerDrawPositionLocal == 1 ){ sternmostPixel = NUMPIXELS - 1; }
      else { sternmostPixel = playerDrawPositionLocal - 2; }

      //unset last entity pixel
      pixels.setPixelColor(sternmostPixel, pixels.Color(0, 0, 0));

      //Strip End/Beginning handling
      int nextPixel;
      if(playerDrawPositionLocal == NUMPIXELS - 1 ){ nextPixel = 0; }
      else{ nextPixel = playerDrawPositionLocal + 1; }

       //set next pixel
       pixels.setPixelColor(nextPixel, pixels.Color(playerColorArrayRGB[0], playerColorArrayRGB[1], playerColorArrayRGB[2]));

       pixels.show();
       //update playerLastPosisition
       playerDrawPositionLocal++;
    }
    //remove this line for fancy bug
    player1DrawPosition = playerDrawPositionLocal;
  }
}

void update()
{
  Serial.println("debug update"+player1LogicPosition);
  delay(50);
  player1LogicPosition++;
}

void loop()
{
  Serial.println("debug loop");
  draw(player1DrawPosition, player1LogicPosition, player1ColorArrayRGB);
  update();


}
