//#include <avr/power.h>
#include <Adafruit_NeoPixel.h>
#include <avr/sleep.h>

#define PIN_DATA 0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN_DATA, NEO_GRB + NEO_KHZ800);


int upModePin = 2;
int upModeButtonState = HIGH;
int upModeButtonCycles = 0;

int CYCLES_DEBOUNCE = 2; //check the button for X ticks to see if it is bouncing
int MAX_MODES = 6;

unsigned long tick = 0;

int mode = 0;

uint16_t i, j, x, y ;
uint32_t c, d;

void setup() {
  //if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  
  // Setup the NeoPixel Strip
  strip.begin();
  strip.setBrightness(128);
  strip.show(); // Initialize all pixels to 'off'
  
  pinMode(upModePin, INPUT);    // declare pushbutton as input
  
  //attachInterrupt(2,triggerModeUp,LOW); //pin 2
  
  //triggerSleep();
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  /*
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  */
  // Send a theater pixel chase in...
  /*
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127,   0,   0), 50); // Red
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue
  */
  
  // Not evenly distributed rainbow, fine with strips reversed
  //rainbow(20);
  
  // I like this one, smooth scrolling rainbow
  //rainbowCycle(20);
  
  // Flashing rainbow.. looks alright... would need strips to be reversed
  //theaterChaseRainbow(50);
  
  

  
  
  
  switch(mode%MAX_MODES) {
    case 0:
      solid(strip.Color(255, 0, 0)); // Red
      break;
    case 1:
      solid(strip.Color(0, 255, 0)); // Greem
      break;
    case 2:
      solid(strip.Color(0, 0, 255)); // Blue
      break;
    case 3:
      solid(strip.Color(255, 255, 255)); // White
      break;
    case 4:
      rainbowCycle(20);
      break;
    case 5:
      theaterChaseRainbow(50);
      break;
    case 6:
      colorWipe(strip.Color(255, 255, 255), 50); // Red
      break;
/*
    case 0: //solid
      rainbow(20);
      break;
    case 1:
      rainbowCycle(20);
      break;
    case 2:
      theaterChaseRainbow(50);
      break;
    case 3:
      colorWipe(strip.Color(255, 0, 0), 50); // Red
      break;
    case 4:
      colorWipe(strip.Color(0, 255, 0), 50); // Green
      break;
    case 5:
      colorWipe(strip.Color(0, 0, 255), 50); // Blue
      break;
*/
  }
  
  //strip.setPixelColor(strip.numPixels()-1, strip.Color(0,0,0)); //set that last LED off because it overlaps
  strip.show();
  
  handleButtons();
  
  tick++;
}


void triggerModeUp() {
  ++mode;
  //blackout();
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
  for(int i=0; i < strip.numPixels()+1; i++) {
      strip.setPixelColor(i, c);
  }
  //strip.show();
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
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
void theaterChaseRainbow(uint8_t wait) {
  //for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
  int   j = tick % 256;
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
  //}
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}


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

