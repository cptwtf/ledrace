#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN               2
#define PLAYERONEBUTTONPIN   15
#define PLAYERTWOBUTTONPIN 4
#define NUMPIXELS         300


Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


//start/finish line
//dont put to close to strip beginning/end or write extended crossing logic :p
int startFinishLine = 10;
byte startFinishLineColorArrayRGB[] = {254,254,254};
uint32_t startFinishLineColorInteger = pixels.Color(startFinishLineColorArrayRGB[0], startFinishLineColorArrayRGB[1], startFinishLineColorArrayRGB[2]);


//game options
int playerCount = 2;
int maxLaps = 10;



class Player {
  public:
    int LogicPosition = startFinishLine - 1;
    int DrawPosition = this->LogicPosition;
    byte ColorArrayRGB[3] = {0,0,0};
    uint32_t ColorInteger;
    float Speed = 0.0;
    float DecelerationMultiplier = 1.0;
    int buttonPin;
    bool buttonIsDown = false;
    int LapCounter = 0;
    unsigned long LapTimesArray[10];
    bool crossedLine = false;
    unsigned long lastSpeedDecay = 0;
    bool skipSpeedDecayOnce = false;

    void setColor(int R, int G, int B)
    {
      this->ColorArrayRGB[0] = R;
      this->ColorArrayRGB[1] = G;
      this->ColorArrayRGB[2] = B;
      this->ColorInteger = pixels.Color(this->ColorArrayRGB[0], this->ColorArrayRGB[1], this->ColorArrayRGB[2]);
    }
};

Player playerInstances[2];

const int MAX_SPEED = 100;
const int SPEED30PERCENT = MAX_SPEED / 100 * 30;
const int SPEED60PERCENT = MAX_SPEED / 100 * 60;
const int SPEED90PERCENT = MAX_SPEED / 100 * 90;


//"gravity" objects
//[objectNumber]
//            [start][topmost][end][intensity]
const int gravityObjectsCount = 5;
int gravityObjects[gravityObjectsCount][4] = {{19,41,42,6},{42,43,69,5},{82,126,162,8}, {163,174,175,3}, {167, 168, 204, 6}};



unsigned long lastLoop = 0;
//interval between speed decays in ms
const long speedDecayInterval = 32;
const long loopInterval = 32;

bool gameInitDone = false;

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  Serial.begin(9600);
  Serial.println("debug setup");
  pinMode(PLAYERONEBUTTONPIN, INPUT);
  pinMode(PLAYERTWOBUTTONPIN, INPUT);

  pixels.begin();
  pixels.clear();

}

void initGame(int playerCount)
{
  byte mixedColorsArray[3];

  if(playerCount == 1)
  {
    //create player object and put in array
    Player player1;
    player1.setColor(0, 0, 254);
    player1.buttonPin = PLAYERONEBUTTONPIN;
    playerInstances[0] = player1;

  }
  else if(playerCount == 2)
  {
    //create player1 object and put in array
    Player player1;
    player1.setColor(0, 0, 254);
    player1.buttonPin = PLAYERONEBUTTONPIN;
    playerInstances[0] = player1;

    //create player2 object and put in array
    Player player2;
    player2.setColor(254, 0, 0);
    player2.buttonPin = PLAYERTWOBUTTONPIN;
    playerInstances[1] = player2;

    //mix player colors
    mixedColorsArray[0] = (playerInstances[0].ColorArrayRGB[0] + playerInstances[1].ColorArrayRGB[0]) / 2;
    mixedColorsArray[1] = (playerInstances[0].ColorArrayRGB[1] + playerInstances[1].ColorArrayRGB[1]) / 2;
    mixedColorsArray[2] = (playerInstances[0].ColorArrayRGB[2] + playerInstances[1].ColorArrayRGB[2]) / 2;

  }
  else
  {
    //something wrent wrong :p
  }

  //draw player
  if(playerCount == 1)
  {
    for(int j = 2; j >= 0; j--)
    {
      pixels.setPixelColor(playerInstances[0].DrawPosition - j,
                pixels.Color(playerInstances[0].ColorArrayRGB[0],
                             playerInstances[0].ColorArrayRGB[1],
                             playerInstances[0].ColorArrayRGB[2]));
    }
  }
  else if(playerCount == 2) //draw "both" players by mixing their color
  {
    for(int j = 2; j >= 0; j--)
    {
      pixels.setPixelColor(playerInstances[0].DrawPosition - j,
                pixels.Color(mixedColorsArray[0],
                             mixedColorsArray[1],
                             mixedColorsArray[2]));
    }
  }



  //draw start/finish line
  pixels.setPixelColor(startFinishLine,
                        pixels.Color(startFinishLineColorArrayRGB[0], startFinishLineColorArrayRGB[1], startFinishLineColorArrayRGB[2]));

  //draw color merge test //DEACTIVATED
  for(int i = NUMPIXELS; i < NUMPIXELS; i++)
  {
    if(i % 6 == 0) { pixels.setPixelColor(i, pixels.Color(254,0,0)); }
  }
  pixels.setPixelColor(0, pixels.Color(0,0,0));

  //draw "physics" test boundaries
  for(int i = gravityObjectsCount; i > 0; i--)
  {
    pixels.setPixelColor(gravityObjects[i-1][0], pixels.Color(0,254,0));
    pixels.setPixelColor(gravityObjects[i-1][1], pixels.Color(0,254,0));
    pixels.setPixelColor(gravityObjects[i-1][2], pixels.Color(0,254,0));
  }

  pixels.show();
  gameInitDone = true;
}


//Draw players
//void draw(int playerDrawPosition, int playerLogicPosition, byte playerColorArrayRGB[])
void draw(Player &player)
{
  //Serial.println("debug draw");
  int playerDrawPositionLocal = player.DrawPosition;
  int playerLogicPosition = player.LogicPosition;
  byte playerColorArrayRGB[3];
  playerColorArrayRGB[0] = player.ColorArrayRGB[0];
  playerColorArrayRGB[1] = player.ColorArrayRGB[1];
  playerColorArrayRGB[2] = player.ColorArrayRGB[2];

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
    if((playerDrawPositionLocal < playerLogicPosition && !(playerLogicPosition == 299 && playerDrawPositionLocal == 0)) ||
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
    player.DrawPosition = playerDrawPositionLocal;
  }
}


void update(Player &player)
{
  //Serial.println("debug update");
  //Serial.print("update player1speed: ");
  //Serial.println(player1Speed);

  //If player can still gain speed
    if(player.Speed < MAX_SPEED)
    {
      if( digitalRead(player.buttonPin) == 1 && player.buttonIsDown == false)
      {
        //Set flag that button is down
        player.buttonIsDown = true;
        player.Speed++;
        // Just for debugging: player.LogicPosition++;

        //Reset deceleration multiplier if player uses button
        if(player.DecelerationMultiplier > 1) {player.DecelerationMultiplier = 1.0;}
      }
      else if(digitalRead(player.buttonPin) == 0 && player.buttonIsDown == true)
      {
        player.buttonIsDown = false;
      }
      else if(digitalRead(player.buttonPin) == 0 && player.buttonIsDown == false && player.DecelerationMultiplier < 10) //User stopped pressing the button
      {
        //add to deceleration multiplier so car gets slow quicker the longer user doesnt press the button
        //should help with long "roll out" on high velocity
        player.DecelerationMultiplier += 0.25;
        //Serial.print("decel: ");
        //Serial.println(player.DecelerationMultiplier);
      }
    }

    //evaluate speed and update player position
    //checks if player moves and how strong in either direction
    if(player.Speed >= SPEED90PERCENT || -(player.Speed) >= SPEED90PERCENT)
    {
      if(player.Speed > 0) //player moves forward
      {
        player.LogicPosition += 4;

        if(player.LogicPosition > 299)
        {
          if(player.LogicPosition == 300) { player.LogicPosition = 0;}
          else if(player.LogicPosition == 301) { player.LogicPosition = 1;}
          else if(player.LogicPosition == 302) { player.LogicPosition = 2;}
          else if(player.LogicPosition == 303) { player.LogicPosition = 3;}
        }
      }
      else //player moves backward
      {
        player.LogicPosition -= 4;

        if(player.LogicPosition < 0)
        {
          if(player.LogicPosition == -1) { player.LogicPosition = 299;}
          else if(player.LogicPosition == -2) { player.LogicPosition = 298;}
          else if(player.LogicPosition == -3) { player.LogicPosition = 297;}
          else if(player.LogicPosition == -4) { player.LogicPosition = 296;}
        }
      }
    }
    else if(player.Speed >= SPEED60PERCENT || -(player.Speed) >= SPEED60PERCENT)
    {
      if(player.Speed > 0)
      {
        player.LogicPosition += 3;

        if(player.LogicPosition > 299)
        {
          if(player.LogicPosition == 300) { player.LogicPosition = 0;}
          else if(player.LogicPosition == 301) { player.LogicPosition = 1;}
          else if(player.LogicPosition == 302) { player.LogicPosition = 2;}
        }
      }
      else //player moves backward
      {
        player.LogicPosition -= 3;


        if(player.LogicPosition < 0)
        {
          if(player.LogicPosition == -1) { player.LogicPosition = 299;}
          else if(player.LogicPosition == -2) { player.LogicPosition = 298;}
          else if(player.LogicPosition == -3) { player.LogicPosition = 297;}
        }
      }
    }
    else if(player.Speed >= SPEED30PERCENT || -(player.Speed) >= SPEED30PERCENT)
    {
      if(player.Speed > 0)
      {
        player.LogicPosition += 2;

        if(player.LogicPosition > 299)
        {
          if(player.LogicPosition == 300) { player.LogicPosition = 0;}
          else if(player.LogicPosition == 301) { player.LogicPosition = 1;}
        }
      }
      else //player moves backward
      {
        player.LogicPosition -= 2;


        if(player.LogicPosition < 0)
        {
          if(player.LogicPosition == -1) { player.LogicPosition = 299;}
          else if(player.LogicPosition == -2) { player.LogicPosition = 298;}
        }
      }
    }
    else if(player.Speed >= 1 || -(player.Speed) >= 1)
    {
      if(player.Speed > 0)
      {
        player.LogicPosition += 1;

        if(player.LogicPosition > 299)
        {
          if(player.LogicPosition == 300) { player.LogicPosition = 0;}
        }
      }
      else //if player moving backwards
      {
        player.LogicPosition -= 1;

        if(player.LogicPosition < 0)
        {
          if(player.LogicPosition == -1) { player.LogicPosition = 299;}
        }
      }
    }


  //slowly loose speed stat
  if(millis() - player.lastSpeedDecay > speedDecayInterval && !player.skipSpeedDecayOnce)
  {

    if (player.Speed >= 1)
    {
      float speedDecrease = (float)player.Speed * player.Speed *  0.00004 + 0.15;

      speedDecrease = speedDecrease * player.DecelerationMultiplier;

      player.Speed -= speedDecrease;
      Serial.print("speed decay player.speed: ");
      Serial.println(player.Speed);
    }
    else if(player.Speed <= -1)
    {
      float speedDecrease = (float)player.Speed * player.Speed *  0.00004 + 0.15;

      speedDecrease = speedDecrease * player.DecelerationMultiplier;

      player.Speed -= -(speedDecrease);

      Serial.print("speed decay player.speed: ");
      Serial.println(player.Speed);
    }
    else
    {
    //  if(player.Speed > 0) {player.Speed = 0.4;}
    //else{player.Speed = -0.4;}
    }
    player.lastSpeedDecay = millis();
  }
  if(player.skipSpeedDecayOnce){player.skipSpeedDecayOnce = false;}



    //check for crossing finish line
    if(player.LogicPosition >= startFinishLine && player.crossedLine == false)
    {
      player.crossedLine = true;
      //Serial.print("Lap #");
      //Serial.println(player.LapCounter);

      player.LapTimesArray[player.LapCounter] = millis();

      /*
      if(player.LapCounter > 0)
      {
        long laptimeMillis = player.LapTimesArray[player.LapCounter] - player.LapTimesArray[player.LapCounter - 1];
        float laptimeSeconds = float(laptimeMillis) / 1000;
        Serial.print("Lap Time #");
        Serial.print(player.LapCounter);
        Serial.print(": ");
        Serial.println(laptimeSeconds);
      }
      */

      player.LapCounter++;
    }
    else if(player.LogicPosition < startFinishLine && player.crossedLine == true)
    {
      player.crossedLine = false;
    }

    /*//////////////Gravity Objects\\\\\\\\\\\\\\\\
    //decreases speed stat while going uphill and increases speed stat while going downhill
    //not manipulating speed stat while exactly on top of the rise
    //if the player is between rise start point and highest point of the rise (uphill)*/

    //if player doesnt have max speed (positiv or negative)
    if(!(player.Speed >= MAX_SPEED) && !(-(player.Speed) >= MAX_SPEED))
    {
      //loop through gravity objects
      for(int i = gravityObjectsCount; i > 0; i--)
      {
        if(player.LogicPosition >= gravityObjects[i - 1][0] && player.LogicPosition < gravityObjects[i - 1][1])
        {
          float speedDecrease = (float)0.05 * gravityObjects[i - 1][3];

        //  Serial.print("gravity objects decreasing speed by");
        //  Serial.println(speedDecrease);

          player.Speed -= speedDecrease;

          if(player.Speed < 0 && player.Speed > -1.0){player.Speed = -1;}
          if(player.DecelerationMultiplier > 1) {player.DecelerationMultiplier = 1.0;}

          player.skipSpeedDecayOnce = true;

       //    Serial.print("new speed");
      //    Serial.println(player.Speed);

        }//else if the player is between highest point and rise ending (downhill)
        else if(player.LogicPosition > gravityObjects[i - 1][1] && player.LogicPosition <= gravityObjects[i - 1][2])
        {
          float speedIncrease = (float)0.05 * gravityObjects[i - 1][3];

        //  Serial.print("gravity objects increasing speed by");
        //  Serial.println(speedIncrease);


          player.Speed += speedIncrease;

          if(player.Speed > 0 && player.Speed < 1.0){player.Speed = 1;}
          if(player.DecelerationMultiplier > 1) {player.DecelerationMultiplier = 1.0;}

          player.skipSpeedDecayOnce = true;

        //  Serial.print("new speed ");
        //  Serial.println(player.Speed);
        }
      }
    }





}

void loop()
{
  //Serial.println("debug loop");
 if(!gameInitDone)
 {
   initGame(playerCount);
   delay(100);
 }

if(millis() - lastLoop > loopInterval)
{
  for(int i = playerCount; i > 0; i--)
  {
    draw(playerInstances[i-1]);
    update(playerInstances[i-1]);
  }
  lastLoop = millis();
}
  //int buttonvalue = digitalRead(PLAYERONEBUTTONPIN);
 //Serial.print("loop");
//  Serial.println(buttonvalue);
}
