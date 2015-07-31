//#include <avr/power.h>
#include <Adafruit_NeoPixel.h>
#include <avr/sleep.h>

#define PIN_DATA 0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(75, PIN_DATA, NEO_GRB + NEO_KHZ800);


int upModePin = 2;
int upModeButtonState = HIGH;
int upModeButtonCycles = 0;

int CYCLES_DEBOUNCE = 2; //check the button for X ticks to see if it is bouncing
int MAX_MODES = 19;

unsigned long tick = 0;

int mode = 0;

uint8_t i, j, x;
uint32_t c, d;

void setup() {
  //if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  
  // Setup the NeoPixel Strip
  strip.begin();
  strip.setBrightness(58); // Originally 128
  strip.show(); // Initialize all pixels to 'off'
  
  pinMode(upModePin, INPUT);    // declare pushbutton as input
  
  //attachInterrupt(2,triggerModeUp,LOW); //pin 2
  
  //triggerSleep();
}

void loop() {
  
  switch(mode%MAX_MODES) {
    case 0:
      solid(strip.Color(0, 255, 255)); // Red
      break;
    case 1:
      solid(strip.Color(0, 255, 0)); // Green
      break;
    case 2:
      solid(strip.Color(0, 0, 255)); // Blue
      break;
    case 3:
      solid(strip.Color(255, 255, 255)); // White
      break;
    case 4:
      rainbowCycle(20); // Rainbow
      break;
    case 5:
      whiteRainbow(20, false); // White with Rainbow, no flash
      break;
    case 6:
      whiteRainbow(20, true); // White with Rainbow and flash
      break;
    case 7:
      sectionWipeTwo(strip.Color(255, 255, 255), strip.Color(0, 0, 0), 50); // White section wipe
      break;
    case 8:
      sectionWipeTwo(strip.Color(255, 0, 255), strip.Color(0, 0, 0), 50); // Purple section wipe
      break;
    case 9:
      sectionWipeTwo(strip.Color(0, 255, 255), strip.Color(0, 0, 0), 50); // Teal section wipe
      break;
    case 10:
      sectionWipeTwo(strip.Color(0, 255, 255), strip.Color(255, 0, 255), 50); // 2 color section wipe, teal & purple
      break;
    case 11:
      sectionWipeTwo(strip.Color(0, 255, 0), strip.Color(255, 0, 255), 50); // 2 color section wipe, teal & purple
      break;
    case 12:
      solidTwo(strip.Color(0, 255, 0), strip.Color(255, 0, 255)); // 2 color solid, teal & purple
      break;
    case 13:
      //              green                  white                      bg: purple
      twinkleRand(5,strip.Color(0,255,0), strip.Color(255,255,255), strip.Color(200, 0, 255),90); // 
      break;
    case 14:
      //              yellow                  blue                      bg: white
      twinkleRand(5,strip.Color(255,255,0), strip.Color(0,0,255), strip.Color(255, 255, 255),90); // 
      break;
    case 15:
      //              yellow                  white                      bg: black
      twinkleRand(5,strip.Color(255,255,0), strip.Color(255,255,255), strip.Color(0, 0, 0),90); // 
      break;
    case 16:
      //              white                  white                      bg: black
      twinkleRand(5,strip.Color(255,255,255), strip.Color(255,255,255), strip.Color(0, 0, 0),90); // 
      break;
    case 17:
      //              random                  random                      bg: black
      twinkleRand(5, Wheel(random(255)), Wheel(random(255)), strip.Color(0, 0, 0),90); // 
      break;
    case 18:
      //              random                  random                      bg: white
      twinkleRand(5, Wheel(random(255)), Wheel(random(255)), strip.Color(255, 255, 255),90); // 
      break;
  }
  
  //strip.setPixelColor(strip.numPixels()-1, strip.Color(0,0,0)); //set that last LED off because it overlaps
  strip.show();  
  handleButtons();  
  tick++;
}

void triggerModeUp() {
  ++mode;
  solid(strip.Color(0, 0, 0));
}

void handleButtons() {
  // software debounce
  if(digitalRead(upModePin) != upModeButtonState) {
    upModeButtonCycles++;
    if(upModeButtonCycles > CYCLES_DEBOUNCE) {
      upModeButtonCycles = 0;
      upModeButtonState = digitalRead(upModePin);
      if(upModeButtonState == LOW) {
        triggerModeUp();
      }
    }
  }
}

void solid(uint32_t c) {
  for(i=0; i < strip.numPixels()+1; i++) {
      strip.setPixelColor(i, c);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  //uint16_t i, j;
  j = tick % 256;

  //for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  //}
}


//Theatre-style crawling lights with rainbow effect
void whiteRainbow(uint8_t wait, bool flash) {
  j = tick % 256;
  for (i=0; i < strip.numPixels(); i=i+4) {
    strip.setPixelColor(i, Wheel((j*2)%255) ); // Rainbow pixel
    strip.setPixelColor(i+1, strip.Color(255, 255, 255)); // White
    if (j%4 == 0 || !flash) {
      strip.setPixelColor(i+2, strip.Color(255, 255, 255));
    } else {
      strip.setPixelColor(i+2, 0);
    }
    strip.setPixelColor(i+3, strip.Color(255, 255, 255)); //White
   }
  strip.show();
  delay(wait);
}

/*
void sectionWipe(uint32_t c, uint8_t wait) {
  // 15 lights per half meter = 1 section
  j = tick % 15;  
  
  for (int i=0; i<strip.numPixels(); i++)
  {
    strip.setPixelColor(i, 0); // blackout first
  }
  
  for (int i=0; i<strip.numPixels(); i=i+15)
  {
    //strip.setPixelColor(i+j, c);
    for(int b=0; b<3; b++) {
      strip.setPixelColor(i+b+j, c); // Light up section
    }
  }

  strip.show();
  delay(wait);  
}
*/

void sectionWipeTwo(uint32_t c1, uint32_t c2, uint8_t wait) {
  // 15 lights per half meter = 1 section
  j = tick % 15;  
  
  solid(0); // blackout first
  
  for (i=0; i<strip.numPixels(); i=i+15)
  {
    //strip.setPixelColor(i+j, c);
    for(x=0; x<5; x++) {
      //if ((i+b+j) >= 0) {
        strip.setPixelColor(i+x+j, c1); // Light up section
      //}
    }
    //if ((i+j+9) >= 0)
    //{
      strip.setPixelColor(i+j+9, c2);
      strip.setPixelColor(i+j+10, c2);
    //}
  }

  strip.show();
  delay(wait);  
}


void solidTwo(uint32_t c1, uint32_t c2) {
  for (i=0; i<strip.numPixels(); i++)
  {
    if (i%25 < 17) {
      strip.setPixelColor(i, c1);
    } else {
      strip.setPixelColor(i, c2);
    }
  }
  strip.show();
}

void twinkleRand(uint8_t num, uint32_t c1, uint32_t c2, uint32_t bg, uint8_t wait) {
  // set background
  solid(bg);
  // for each num
  for (i=0; i<num; i++) {
    strip.setPixelColor(random(strip.numPixels()),c1);
    strip.setPixelColor(random(strip.numPixels()),c2);
  }
  strip.show();
  delay(wait);
}




















/*





//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  //for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
  int   j = tick % 256;
//    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i, Wheel( (i+j) % 255));    //turn every third pixel on
          strip.setPixelColor(i+1, strip.Color(255, 255, 255));        //turn every third pixel off
          strip.setPixelColor(i+2, strip.Color(255, 255, 255));        //turn every third pixel off
        }
//        strip.show();
       
//        delay(wait);
       
        //for (int i=0; i < strip.numPixels(); i=i+3) {
          //strip.setPixelColor(i+q, 0);        //turn every third pixel off
        //}
  //  }
  //}
  strip.show();
  delay(wait);
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}
*/

/*

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;
  
  j = tick % 384;

  //for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  //}
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  j = tick % 384;

  //for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  //}
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  int j = tick % 384;
  for (j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}
*/

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

