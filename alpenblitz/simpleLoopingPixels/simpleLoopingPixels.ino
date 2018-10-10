// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16

// How many Pixel Rings are connected in series
#define NUMRINGS        51

#define TOTALPIXELS     816 // = NUMPIXELS * NUMRINGS

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
  //pixelCircle(50, 8, pixels.Color(255,0,0), pixels.Color(255,255,255));
  int dt[] = {50,52,57,65,75,85,93,98,100,98,93,85,75,65,57,52,50};
  for(int i = 1; i<16; i++){
    pixelCircleVariableDelay(dt, i, pixels.Color(255,0,0), pixels.Color(15*i,15*i,15*i));
  }
  for(int i = 1; i<16; i++){
    pixelCircleVariableDelay(dt, i, pixels.Color(255,255,255), pixels.Color(15*i,0,0));
  }  
}

void pixelCircle(int dt, int nPixels, uint32_t pixelCol, uint32_t bgCol) {

  // initialize picutre
  for(int iPixel = 0; iPixel < TOTALPIXELS; iPixel++) {
    if((iPixel%NUMPIXELS) < nPixels)
      pixels.setPixelColor(iPixel, pixelCol);
    else
      pixels.setPixelColor(iPixel, bgCol);
  }
  pixels.show();
  delay(dt);

  // loop once trough circle
  for(int pos = 1; pos < NUMPIXELS; pos++) {
    for(int offset = 0; offset < TOTALPIXELS; offset += NUMPIXELS) {
      pixels.setPixelColor((pos + offset - 1)%TOTALPIXELS, bgCol);
      pixels.setPixelColor((pos + nPixels + offset - 1)%TOTALPIXELS, pixelCol);
    }
    pixels.show();
    delay(dt);
  }
}

void pixelCircleVariableDelay(int dt[], int nPixels, uint32_t pixelCol, uint32_t bgCol) {

  // initialize picutre
  for(int iPixel = 0; iPixel < TOTALPIXELS; iPixel++) {
    if((iPixel%NUMPIXELS) < nPixels)
      pixels.setPixelColor(iPixel, pixelCol);
    else if((iPixel%(NUMPIXELS-1)) == 00 )
      pixels.setPixelColor(iPixel, bgCol);
  }
  pixels.show();
  delay(dt[0]);

  // loop once trough circle
  for(int pos = 1; pos < NUMPIXELS; pos++) {
    for(int offset = 0; offset < TOTALPIXELS; offset += NUMPIXELS) {
      pixels.setPixelColor((pos + offset - 1)%TOTALPIXELS, bgCol);
      pixels.setPixelColor((pos + nPixels + offset - 1)%TOTALPIXELS, pixelCol);
    }
    pixels.show();
    delay(dt[pos]);
  }
}
