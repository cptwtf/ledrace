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
int player1LastPosition = 2;
int player1CurrentPosition = 2;

//Draw players
void draw(int playerLastPosition, int playerCurrentPosition)
{
  Serial.println("debug draw");
  int playerLastPositionLocal = playerLastPosition;
  //While player real location and player drawn location differ
  while(playerLastPositionLocal != playerCurrentPosition)
  {
    Serial.println("debug draw while");
    //if player real position is in front of current drawn position
    if(playerLastPositionLocal < playerCurrentPosition)
    {
      Serial.println("debug draw while if");
      //unset last entity pixel
      int lastEntityPixel = playerLastPositionLocal - 2;
      pixels.setPixelColor(lastEntityPixel, pixels.Color(0, 0, 0));

       //set next pixel
       int nextEntityPixel = playerLastPositionLocal + 1;
       pixels.setPixelColor(nextEntityPixel, pixels.Color(0, 150, 0));


       pixels.show();
       //update playerLastPosisition
       playerLastPositionLocal++;
    }
    //remove this line for fancy bug
    //player1LastPosition = playerLastPositionLocal;
  }
}

void update()
{
  Serial.println("debug update"+player1CurrentPosition);
  delay(50);
  player1CurrentPosition++;
}

void loop()
{
  Serial.println("debug loop");
  draw(player1LastPosition, player1CurrentPosition);
  update();


}
