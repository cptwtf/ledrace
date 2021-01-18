#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN               2
#define PLAYERONEBUTTONPIN   4
#define NUMPIXELS         300
#define MAX_SPEED 100


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//start/finish line
int startFinishLine = 3;
byte startFinishLineColorArrayRGB[] = {254,254,254};
uint32_t startFinishLineColorInteger = pixels.Color(startFinishLineColorArrayRGB[0], startFinishLineColorArrayRGB[1], startFinishLineColorArrayRGB[2]);
//startposition
int player1LogicPosition = startFinishLine - 1;
int player1DrawPosition = player1LogicPosition;
byte player1ColorArrayRGB[] = {0,0,254};
uint32_t player1ColorInteger = pixels.Color(player1ColorArrayRGB[0], player1ColorArrayRGB[1], player1ColorArrayRGB[2]);
int player1Speed = 0;
bool buttonPlayer1IsDown = false;

const int SPEED30PERCENT = MAX_SPEED / 100 * 30;
const int SPEED60PERCENT = MAX_SPEED / 100 * 60;
const int SPEED90PERCENT = MAX_SPEED / 100 * 90;
unsigned long lastSpeedDecay = 0;

//interval between speed decays in ms
const long speedDecayInterval = 200;

bool gameInitDone = false;

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  Serial.begin(9600);
  Serial.println("debug setup");
  pinMode(PLAYERONEBUTTONPIN, INPUT);

  pixels.begin();
  pixels.clear();

}

void initGame()
{
  //draw player 1 start position
  pixels.setPixelColor(player1DrawPosition - 2,
                        pixels.Color(player1ColorArrayRGB[0], player1ColorArrayRGB[1], player1ColorArrayRGB[2]));
  pixels.setPixelColor(player1DrawPosition - 1,
                        pixels.Color(player1ColorArrayRGB[0], player1ColorArrayRGB[1], player1ColorArrayRGB[2]));
  pixels.setPixelColor(player1DrawPosition,
                        pixels.Color(player1ColorArrayRGB[0], player1ColorArrayRGB[1], player1ColorArrayRGB[2]));

  //draw start/finish line
  pixels.setPixelColor(startFinishLine,
                        pixels.Color(startFinishLineColorArrayRGB[0], startFinishLineColorArrayRGB[1], startFinishLineColorArrayRGB[2]));

  //draw color merge test NUMPIXELS
  for(int i = 0; i < NUMPIXELS; i++)
  {
    if(i % 6 == 0) { pixels.setPixelColor(i, pixels.Color(254,0,0)); }
  }
  pixels.setPixelColor(0, pixels.Color(0,0,0));

  pixels.show();
  gameInitDone = true;
}


//Draw players
void draw(int playerDrawPosition, int playerLogicPosition, byte playerColorArrayRGB[])
{
  //Serial.println("debug draw");
  int playerDrawPositionLocal = playerDrawPosition;
  delay(20);
  //While player real location and player drawn location differ
  while(playerDrawPositionLocal != playerLogicPosition)
  {
    //Serial.println(playerDrawPositionLocal);
  //  Serial.println(playerLogicPosition);
    //Serial.println("debug draw while");
    //if player logic position is in front of current drawn position
    if(playerDrawPositionLocal < playerLogicPosition ||
      (playerDrawPositionLocal == 299 && playerLogicPosition <= 3) ||
      (playerDrawPositionLocal == 298 && playerLogicPosition <= 3) ||
      (playerDrawPositionLocal == 297 && playerLogicPosition <= 3))
    {
      //Strip End/Beginning handling
      int sternmostPixel;
      if(playerDrawPositionLocal == 0 ){ sternmostPixel = NUMPIXELS - 2; }
      else if (playerDrawPositionLocal == 1 ){ sternmostPixel = NUMPIXELS - 1; }
      else { sternmostPixel = playerDrawPositionLocal - 2; }

      //unset last entity pixel
      //if the pixel to unset has not the player entity color
      if(pixels.getPixelColor(sternmostPixel) != pixels.Color(playerColorArrayRGB[0], playerColorArrayRGB[1], playerColorArrayRGB[2]))
      {
        //get pixelcolor
        uint32_t pixelColor = pixels.getPixelColor(sternmostPixel);

        //convert to byte array rgb values
        byte unmixedCol[3];
        unmixedCol[2] = pixelColor;
        unmixedCol[1] = pixelColor >> 8;
        unmixedCol[0] = pixelColor >> 16;

        //retract entity color
        unmixedCol[0]= (unmixedCol[0] - (playerColorArrayRGB[0] / 2)) * 2;
        unmixedCol[1]= (unmixedCol[1] - (playerColorArrayRGB[1] / 2)) * 2;
        unmixedCol[2]= (unmixedCol[2] - (playerColorArrayRGB[2] / 2)) * 2;

        //set "demerged" color
        pixels.setPixelColor(sternmostPixel, pixels.Color(unmixedCol[0], unmixedCol[1], unmixedCol[2]));
      }
      else //if the pixel to unset has the same color as the player entity
      {
        pixels.setPixelColor(sternmostPixel, pixels.Color(0, 0, 0));
      }

      //Strip End/Beginning handling
      int nextPixel;
      if(playerDrawPositionLocal == NUMPIXELS - 1 ){ nextPixel = 0; }
      else{ nextPixel = playerDrawPositionLocal + 1; }

       //set next pixel
       //check if next pixel empty
       if(pixels.getPixelColor(nextPixel) == 0)
       {
         //set playercolor
         pixels.setPixelColor(nextPixel, pixels.Color(playerColorArrayRGB[0], playerColorArrayRGB[1], playerColorArrayRGB[2]));
       }
       else //merge player color and pixel color if pixel already in use
       {
         //get pixelcolor
         uint32_t pixelColor = pixels.getPixelColor(nextPixel);

         //convert to byte array rgb values
         byte mixedCol[3];
         mixedCol[2] = pixelColor;
         mixedCol[1] = pixelColor >> 8;
         mixedCol[0] = pixelColor >> 16;

         //merge colors
         mixedCol[0] = (mixedCol[0] + playerColorArrayRGB[0]) / 2;
      	 mixedCol[1] = (mixedCol[1] + playerColorArrayRGB[1]) / 2;
         mixedCol[2] = (mixedCol[2] + playerColorArrayRGB[2]) / 2;

         //set mixed color
         pixels.setPixelColor(nextPixel, pixels.Color(mixedCol[0], mixedCol[1], mixedCol[2]));
       }


       pixels.show();
       //update playerDrawPosition
       if(playerDrawPositionLocal == 299) { playerDrawPositionLocal = 0; }
       else
       {
        //Serial.println("DRAW IS INCREMENTING PLAYERDRAWPOSITIONLOCAL");
         playerDrawPositionLocal++;
       }
    }
    //remove this line for fancy bug :p
    player1DrawPosition = playerDrawPositionLocal;
  }
}


void update()
{
  //Serial.println("debug update");
  Serial.print("update begin player1speed: ");
  Serial.println(player1Speed);
  //If player can still gain speed
    if(player1Speed < MAX_SPEED)
    {
      if( digitalRead(PLAYERONEBUTTONPIN) == 1 && buttonPlayer1IsDown == false)
      {
        //Set flag that button is down
        buttonPlayer1IsDown = true;
        player1Speed++;
      }
      else if(digitalRead(PLAYERONEBUTTONPIN) == 0 && buttonPlayer1IsDown == true)
      {
        buttonPlayer1IsDown = false;
      }

    }


  //evaluate speed and update player position
  if(player1Speed >= SPEED60PERCENT)
  {
    player1LogicPosition += 3;
    if(player1LogicPosition > 299)
    {
      if(player1LogicPosition == 300) { player1LogicPosition = 0;}
      else if(player1LogicPosition == 301) { player1LogicPosition = 1;}
      else if(player1LogicPosition == 302) { player1LogicPosition = 2;}
    }
  }
  else if(player1Speed >= SPEED30PERCENT)
  {
    player1LogicPosition += 2;
    if(player1LogicPosition > 299)
    {
      if(player1LogicPosition == 300) { player1LogicPosition = 0;}
      else if(player1LogicPosition == 301) { player1LogicPosition = 1;}
      else if(player1LogicPosition == 302) { player1LogicPosition = 2;}
    }
  }
  else if(player1Speed >= 1)
  {
    player1LogicPosition += 1;
    if(player1LogicPosition > 299)
    {
      if(player1LogicPosition == 300) { player1LogicPosition = 0;}
      else if(player1LogicPosition == 301) { player1LogicPosition = 1;}
      else if(player1LogicPosition == 302) { player1LogicPosition = 2;}
    }
  }
  //slowly loose speed stat
  if(millis() - lastSpeedDecay > speedDecayInterval)
  {
    //only decay when more than 0
    if(player1Speed > 0)
    {
      //standard decrease 10% (at least 1), 15% on 60% max speed, 25% on 90% max speed
      if(player1Speed <= SPEED30PERCENT)
      {
        int speedDecrease = player1Speed / 100 * 10;

        if(speedDecrease >= 1)
        {
          player1Speed -= speedDecrease;
        }
        else
        {
          player1Speed--;
        }
      }
      else if(player1Speed <= SPEED60PERCENT)
      {
        int speedDecrease = player1Speed / 100 * 15;
        player1Speed -= speedDecrease;
      }
      else if(player1Speed <= SPEED90PERCENT)
      {
        int speedDecrease = player1Speed / 100 * 25;
        player1Speed -= speedDecrease;
      }
      lastSpeedDecay = millis();
      //Serial.println("SPEED DECAY");
    }
  }

}

void loop()
{
  //Serial.println("debug loop");
 if(!gameInitDone)
 {
   initGame();
   delay(100);
 }
draw(player1DrawPosition, player1LogicPosition, player1ColorArrayRGB);
update();
  //int buttonvalue = digitalRead(PLAYERONEBUTTONPIN);
 //Serial.print("loop");
//  Serial.println(buttonvalue);
}
