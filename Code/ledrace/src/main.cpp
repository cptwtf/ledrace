#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN               2
#define PLAYERONEBUTTONPIN   4
#define NUMPIXELS         300


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//start/finish line
//dont put to close to strip beginning/end or write extended crossing logic :p
int startFinishLine = 10;
byte startFinishLineColorArrayRGB[] = {254,254,254};
uint32_t startFinishLineColorInteger = pixels.Color(startFinishLineColorArrayRGB[0], startFinishLineColorArrayRGB[1], startFinishLineColorArrayRGB[2]);
//startposition
int player1LogicPosition = startFinishLine - 1;
int player1DrawPosition = player1LogicPosition;
byte player1ColorArrayRGB[] = {0,0,254};
uint32_t player1ColorInteger = pixels.Color(player1ColorArrayRGB[0], player1ColorArrayRGB[1], player1ColorArrayRGB[2]);
int player1Speed = 0;
int player1RollingPower = 0;
int player1RollNow = 0;
float player1DecelerationMultiplier = 1.0;
bool buttonPlayer1IsDown = false;
int player1LapCounter = 0;
unsigned long player1LapTimesArray[10];
bool player1Crossed = false;




const int MAX_SPEED = 100;
const int SPEED30PERCENT = MAX_SPEED / 100 * 30;
const int SPEED60PERCENT = MAX_SPEED / 100 * 60;
const int SPEED90PERCENT = MAX_SPEED / 100 * 90;


//"gravity" objects
//[objectNumber]
//            [start][topmost][end][intensity]

int gravityObjects[2][4] = {{72,126,198,10},{10,15,20,2}};



unsigned long lastSpeedDecay = 0;
unsigned long lastLoop = 0;
//interval between speed decays in ms
const long speedDecayInterval = 200;
const long loopInterval = 32;

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

  //draw "physics" test boundaries
  pixels.setPixelColor(gravityObjects[0][0], pixels.Color(0,254,0));
  pixels.setPixelColor(gravityObjects[0][1], pixels.Color(0,254,0));
  pixels.setPixelColor(gravityObjects[0][2], pixels.Color(0,254,0));

  pixels.show();
  gameInitDone = true;
}


//Draw players
void draw(int playerDrawPosition, int playerLogicPosition, byte playerColorArrayRGB[])
{
  //Serial.println("debug draw");
  int playerDrawPositionLocal = playerDrawPosition;
  //While player real location and player drawn location differ
  while(playerDrawPositionLocal != playerLogicPosition)
  {
    //Serial.println(playerDrawPositionLocal);
    //  Serial.println(playerLogicPosition);
    //Strip End/Beginning handling
    int sternmostPixel;
    if(playerDrawPositionLocal == 0 ){ sternmostPixel = NUMPIXELS - 2; }
    else if (playerDrawPositionLocal == 1 ){ sternmostPixel = NUMPIXELS - 1; }
    else { sternmostPixel = playerDrawPositionLocal - 2; }
    //Strip End/Beginning handling
    int nextPixel;
    if(playerDrawPositionLocal == NUMPIXELS - 1 ){ nextPixel = 0; }
    else{ nextPixel = playerDrawPositionLocal + 1; }

    //if player logic position is in front of current drawn position
    if(playerDrawPositionLocal < playerLogicPosition ||
      (playerDrawPositionLocal == 299 && playerLogicPosition <= 3) ||
      (playerDrawPositionLocal == 298 && playerLogicPosition <= 3) ||
      (playerDrawPositionLocal == 297 && playerLogicPosition <= 3))
    {

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
    } //if player entity has to be moved backwards
    else if(playerDrawPositionLocal > playerLogicPosition ||
            (playerDrawPositionLocal == 2 && playerLogicPosition >= 296) ||
            (playerDrawPositionLocal == 1 && playerLogicPosition >= 296) ||
            (playerDrawPositionLocal == 0 && playerLogicPosition >= 296)   )
    {

            //unset front entity pixel
            //if the pixel to unset has not the player entity color
            if(pixels.getPixelColor(nextPixel - 1) != pixels.Color(playerColorArrayRGB[0], playerColorArrayRGB[1], playerColorArrayRGB[2]))
            {
              //get pixelcolor
              uint32_t pixelColor = pixels.getPixelColor(nextPixel - 1);

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
              pixels.setPixelColor(nextPixel - 1, pixels.Color(unmixedCol[0], unmixedCol[1], unmixedCol[2]));
            }
            else //if the pixel to unset has the same color as the player entity
            {
              pixels.setPixelColor(nextPixel - 1, pixels.Color(0, 0, 0));
            }


             //set next pixel in opposite race direction
             //check if next pixel empty
             if(pixels.getPixelColor(sternmostPixel - 1) == 0)
             {
               //set playercolor
               pixels.setPixelColor(sternmostPixel - 1, pixels.Color(playerColorArrayRGB[0], playerColorArrayRGB[1], playerColorArrayRGB[2]));
             }
             else //merge player color and pixel color if pixel already in use
             {
               //get pixelcolor
               uint32_t pixelColor = pixels.getPixelColor(sternmostPixel - 1);

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
               pixels.setPixelColor(sternmostPixel - 1, pixels.Color(mixedCol[0], mixedCol[1], mixedCol[2]));
             }


             pixels.show();
             //update playerDrawPosition
             if(playerDrawPositionLocal == 0) { playerDrawPositionLocal = 299; }
             else
             {
              //Serial.println("DRAW IS DECREMENTING PLAYERDRAWPOSITIONLOCAL");
               playerDrawPositionLocal--;
             }
    }
    //remove this line for fancy bug :p
    player1DrawPosition = playerDrawPositionLocal;
  }
}


void update()
{
  //Serial.println("debug update");
  //Serial.print("update player1speed: ");
  //Serial.println(player1Speed);

  //If player can still gain speed
    if(player1Speed < MAX_SPEED)
    {
      if( digitalRead(PLAYERONEBUTTONPIN) == 1 && buttonPlayer1IsDown == false)
      {
        //Set flag that button is down
        buttonPlayer1IsDown = true;
        player1Speed++;

        //Reset deceleration multiplier if player uses button
        if(player1DecelerationMultiplier > 1) {player1DecelerationMultiplier = 1.0;}
      }
      else if(digitalRead(PLAYERONEBUTTONPIN) == 0 && buttonPlayer1IsDown == true)
      {
        buttonPlayer1IsDown = false;
      }
      else if(digitalRead(PLAYERONEBUTTONPIN) == 0 && buttonPlayer1IsDown == false && player1DecelerationMultiplier < 6) //User stopped pressing the button
      {
        //add to deceleration multiplier so car gets slow quicker the longer user doesnt press the button
        //should help with long "roll out" on high velocity
        player1DecelerationMultiplier += 0.6;
      }
    }


  //evaluate speed and update player position
  //checks if player moves and how strong in either direction
  if(player1Speed >= SPEED90PERCENT || -(player1Speed) >= SPEED90PERCENT)
  {
    if(player1Speed > 0) //player moves forward
    {
      player1LogicPosition += 4;

      if(player1RollingPower < 8) { player1RollingPower = 8; }

      if(player1LogicPosition > 299)
      {
        if(player1LogicPosition == 300) { player1LogicPosition = 0;}
        else if(player1LogicPosition == 301) { player1LogicPosition = 1;}
        else if(player1LogicPosition == 302) { player1LogicPosition = 2;}
        else if(player1LogicPosition == 303) { player1LogicPosition = 3;}
      }
    }
    else //player moves backward
    {
      player1LogicPosition -= 4;

      //if(player1RollingPower < 8) { player1RollingPower = 8; }

      if(player1LogicPosition < 0)
      {
        if(player1LogicPosition == -1) { player1LogicPosition = 299;}
        else if(player1LogicPosition == -2) { player1LogicPosition = 298;}
        else if(player1LogicPosition == -3) { player1LogicPosition = 297;}
        else if(player1LogicPosition == -4) { player1LogicPosition = 296;}
      }
    }
  }
  else if(player1Speed >= SPEED60PERCENT || -(player1Speed) >= SPEED60PERCENT)
  {
    if(player1Speed > 0)
    {
      player1LogicPosition += 3;

      if(player1RollingPower < 6) { player1RollingPower = 6; }

      if(player1LogicPosition > 299)
      {
        if(player1LogicPosition == 300) { player1LogicPosition = 0;}
        else if(player1LogicPosition == 301) { player1LogicPosition = 1;}
        else if(player1LogicPosition == 302) { player1LogicPosition = 2;}
      }
    }
    else //player moves backward
    {
      player1LogicPosition -= 3;

      //if(player1RollingPower < 6) { player1RollingPower = 6; }

      if(player1LogicPosition < 0)
      {
        if(player1LogicPosition == -1) { player1LogicPosition = 299;}
        else if(player1LogicPosition == -2) { player1LogicPosition = 298;}
        else if(player1LogicPosition == -3) { player1LogicPosition = 297;}
      }
    }
  }
  else if(player1Speed >= SPEED30PERCENT || -(player1Speed) >= SPEED30PERCENT)
  {
    if(player1Speed > 0)
    {
      player1LogicPosition += 2;

      if(player1RollingPower < 4) { player1RollingPower = 4; }

      if(player1LogicPosition > 299)
      {
        if(player1LogicPosition == 300) { player1LogicPosition = 0;}
        else if(player1LogicPosition == 301) { player1LogicPosition = 1;}
      }
    }
    else //player moves backward
    {
      player1LogicPosition -= 2;

      //if(player1RollingPower < 4) { player1RollingPower = 4; }

      if(player1LogicPosition < 0)
      {
        if(player1LogicPosition == -1) { player1LogicPosition = 299;}
        else if(player1LogicPosition == -2) { player1LogicPosition = 298;}
      }
    }
  }
  else if(player1Speed >= 1 || -(player1Speed) >= 1)
  {
    if(player1Speed > 0)
    {
      player1LogicPosition += 1;

      if(player1Speed > 5 && player1RollingPower < 2) { player1RollingPower = 2;}
      if(player1LogicPosition > 299)
      {
        if(player1LogicPosition == 300) { player1LogicPosition = 0;}
      }
    }
    else //if player moving backwards
    {
      player1LogicPosition -= 1;

      //if(player1Speed < 5 && player1RollingPower < 2) { player1RollingPower = 2;}
      if(player1LogicPosition < 0)
      {
        if(player1LogicPosition == -1) { player1LogicPosition = 299;}
      }
    }
  }

  //check for crossing finish line
  if(player1LogicPosition >= startFinishLine && player1Crossed == false)
  {
    player1Crossed = true;
    //Serial.print("Lap #");
    //Serial.println(player1LapCounter);

    player1LapTimesArray[player1LapCounter] = millis();

    /*
    if(player1LapCounter > 0)
    {
      long laptimeMillis = player1LapTimesArray[player1LapCounter] - player1LapTimesArray[player1LapCounter - 1];
      float laptimeSeconds = float(laptimeMillis) / 1000;
      Serial.print("Lap Time #");
      Serial.print(player1LapCounter);
      Serial.print(": ");
      Serial.println(laptimeSeconds);
    }
    */

    player1LapCounter++;
  }
  else if(player1LogicPosition < startFinishLine && player1Crossed == true)
  {
    player1Crossed = false;
  }

  //slowly loose speed stat
  if(millis() - lastSpeedDecay > speedDecayInterval)
  {
    //only decay when more than 0
    if(player1Speed != 0)
    {
      //standard decrease  (at least 1), decreasing faster on higher speeds
      if(player1Speed <= SPEED30PERCENT || -(player1Speed) <= SPEED30PERCENT)
      {
        int speedDecrease = player1Speed / (float)100 * 2 * player1DecelerationMultiplier;

        if(player1Speed > 0)
        {
          if(speedDecrease > 1)
          {
            player1Speed -= speedDecrease;
          }
          else
          {
            player1Speed--;
          }
        }
        else
        {
          if(speedDecrease < -1)
          {
            player1Speed -= speedDecrease;
          }
          else
          {
            player1Speed++;
          }
        }
      }
      else if(player1Speed <= SPEED60PERCENT  || -(player1Speed) <= SPEED60PERCENT)
      {
        //Serial.println("ELSE IF SPEED60PERCENT");
        int speedDecrease = (float)player1Speed / 100 * 2 * player1DecelerationMultiplier;
        //Serial.println(speedDecrease);

        player1Speed -= speedDecrease;
      }
      else if(player1Speed <= SPEED90PERCENT  || -(player1Speed) <= SPEED90PERCENT)
      {
        int speedDecrease = (float)player1Speed / 100 * 3  * player1DecelerationMultiplier;
        player1Speed -= speedDecrease;
      }
      else if(player1Speed > SPEED90PERCENT  || -(player1Speed) > SPEED90PERCENT)
      {
        int speedDecrease = (float)player1Speed / 100 * 4  * player1DecelerationMultiplier;
        player1Speed -= speedDecrease;
      }
    }
    ////////////////Gravity Objects\\\\\\\\\\\\\\\\
    //decreases speed stat while going uphill and increases speed stat while going downhill
    //not manipulating speed stat while exactly on top of the rise
    //if the player is between rise start point and highest point of the rise (uphill)
    if(player1LogicPosition >= gravityObjects[0][0] && player1LogicPosition < gravityObjects[0][1])
    {
      int speedDecrease = (float)0.1 * gravityObjects[0][3] * player1DecelerationMultiplier;
      player1Speed -= speedDecrease;

      Serial.print("gravity handling player1speed: ");
      Serial.println(player1Speed);
    }//else if the player is between highest point and rise ending (downhill)
    else if(player1LogicPosition > gravityObjects[0][1] && player1LogicPosition <= gravityObjects[0][2])
    {
      int speedIncrease = (float)0.1 * gravityObjects[0][3] * player1DecelerationMultiplier;
      player1Speed += speedIncrease;

      Serial.print("gravity handling player1speed: ");
      Serial.println(player1Speed);
    }
    lastSpeedDecay = millis();
    //Serial.println("SPEED DECAY");
  }


  ////////////////"Momentum"\\\\\\\\\\\\\\\\\\\\\\
  //if player gets to 0 speed but entity is marked as moving previously
  if(player1Speed == 0 && player1RollingPower != 0)
  {
    //forward rolling power
    if(player1RollingPower > 0)
    {
      //Serial.println("PLAYER ROLLING POWER MORE THAN ZERO");
      //skip every second speedcheck
      if(player1RollNow == 0)
      {
        player1RollNow = player1RollingPower;
      }
      else if(player1RollNow < 10)
      {
        player1RollNow++;
      }
      else
      {
        //add some speed
        player1Speed++;

        //decrease rolling power counter
        player1RollingPower--;

        //prepare for next check
        player1RollNow = 0;
      }
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

if(millis() - lastLoop > loopInterval)
{
  draw(player1DrawPosition, player1LogicPosition, player1ColorArrayRGB);
  update();
  lastLoop = millis();
}
  //int buttonvalue = digitalRead(PLAYERONEBUTTONPIN);
 //Serial.print("loop");
//  Serial.println(buttonvalue);
}
