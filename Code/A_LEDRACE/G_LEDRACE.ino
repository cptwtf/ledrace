void light() {

    byte color1R = 0;
    byte color1G = 0;
    byte color1B = 254;
    
        byte color2R = 254;
        byte color2G = 0;
        byte color2B = 0;    

    pixels.setPixelColor(0, pixels.Color(color1R, color1G, color1B));
    pixels.setPixelColor(1, pixels.Color(color1R, color1G, color1B));
    pixels.setPixelColor(2, pixels.Color(color1R, color1G, color1B));

    pixels.setPixelColor(4, pixels.Color(color2R, color2G, color2B));
    pixels.setPixelColor(5, pixels.Color(color2R, color2G, color2B));
    pixels.setPixelColor(6, pixels.Color(color2R, color2G, color2B));
    
byte mixedCol[3];
 mixedCol[0] = (color1R + color2R) / 2;
 mixedCol[1] = (color1G + color2G) / 2;
 mixedCol[2] = (color1B + color2B) / 2;

 pixels.setPixelColor(8, pixels.Color(mixedCol[0], mixedCol[1], mixedCol[2]));
   pixels.setPixelColor(9, pixels.Color(mixedCol[0], mixedCol[1], mixedCol[2]));
   pixels.setPixelColor(10, pixels.Color(mixedCol[0], mixedCol[1], mixedCol[2]));
    pixels.show();
    
    delay(DELAYVAL);
  }
