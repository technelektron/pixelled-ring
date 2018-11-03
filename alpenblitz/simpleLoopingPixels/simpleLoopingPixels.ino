// Arduino Duemilanove - Atmega38P

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
#define NUMRINGS        27

#define TOTALPIXELS     432 // = NUMPIXELS * NUMRINGS

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(TOTALPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

#define NUMCOLORS 8
#define RED 0
#define GREEN 1
#define BLUE 2
#define WHITE 3
#define YELLOW 4
#define MAGENTA 5
#define CYAN 6
#define ORANGE 7

uint32_t colors[NUMCOLORS];
int prevrandnum = 0;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.

  randomSeed(analogRead(0));

  // init colors
  colors[RED] = pixels.Color(255,0,0);
  colors[GREEN] = pixels.Color(0,255,0);
  colors[BLUE] = pixels.Color(0,0,255);
  colors[WHITE] = pixels.Color(255,255,255);
  colors[YELLOW] = pixels.Color(255,255,0);
  colors[MAGENTA] = pixels.Color(255,0,255);
  colors[CYAN] = pixels.Color(0,255,255);
  colors[ORANGE] = pixels.Color(255,128,0);
}

void loop() {

  // sinusoidal time delay
  int dt_sine[] = {50,52,57,65,75,85,93,98,100,98,93,85,75,65,57,52,50};
  // sinusoidal time delay fast
  int dt_sinef[] = {25,26,28,32,37,42,46,49,50,49,46,42,37,32,28,26,25};  
  // constant time delay
  int dt_const[] = {50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50};
  int dt_custom[] = {50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50};
  
  
  int i;
  uint32_t col3, col1, col2;

 // oneThird(dt_sinef, colors[RED],colors[ORANGE],colors[YELLOW], true);

  // pingPong Swipe
  for(i = 0; i < 10; i++) {
    col1  = randomColor();
    clearSwipe(dt_const, col1, 0, 0, true);
    col1  = randomColor();
    clearSwipe(dt_const, col1, 0, 0, false);
  }


  // double pingPong Swipe
  col2  = col1;
  for(i = 0; i < 10; i++) {
    col1  = randomColor();
    doubleSwipe(dt_const, col1, col2, 0, true);
    col2  = randomColor();
    doubleSwipe(dt_const, col1, col2, 0, false);
  }  

  // twoquarters
  col1 = randomColor();
  do{ col3 = randomColor(); } while( (col1 == col3) || (col2 == col3) );
  
  doubleSwipe(dt_const, col1, col2, 4, true);
  for(i = 0; i < 4; i++) {
    twoQuarters(dt_const, col1, col2, true);
  }
  clearSwipe(dt_const, col3,15, 16, true);
  clearSwipe(dt_const, col3,0, 15, true);

  // pixelCircle
  col2 = col3;
  col1 = randomColor();
  do{ col3 = randomColor(); } while( (col1 == col3) || (col2 == col3) );
  for(int i = 1; i<16; i++){
    createConstTimeDelay(50 - 3*i, dt_custom);
    pixelCircle(dt_custom, i, col1, col2);
  }
  clearSwipe(dt_const, 0,15, 16, true);
  clearSwipe(dt_const, 0,0, 15, true);

  for(i = 0; i < 10; i++) {
    createConstTimeDelay(20, dt_custom);
    oneThird(dt_custom, colors[RED],colors[ORANGE],colors[YELLOW], true);
  }

  


  /*
  doubleSwipe(dt_const, pixels.Color(0,255,0), pixels.Color(0,0,255), 4, true);
  twoQuarters(dt_const, pixels.Color(0,255,0), pixels.Color(0,0,255), true);
  clearSwipe(dt_const, pixels.Color(0,0,255),15, 16, true);
  clearSwipe(dt_const, pixels.Color(0,0,255),0, 3, true);
  clearSwipe(dt_const, pixels.Color(0,0,255),7, 10, true);
  
/*
  swipe(dt_const, pixels.Color(0,255,0), pixels.Color(0,0,255), true);
  swipe(dt_const, pixels.Color(0,0,255), pixels.Color(0,255,0), true);
  pixelCircle(dt_const, 1, pixels.Color(0,255,0), pixels.Color(0,0,255));
  pixelCircle(dt_const, 2, pixels.Color(0,255,0), pixels.Color(0,0,255));
  pixelCircle(dt_const, 3, pixels.Color(0,255,0), pixels.Color(0,0,255));
  twoQuarters(dt_const, pixels.Color(0,255,0), pixels.Color(0,0,255), true);
  twoQuarters(dt_const, pixels.Color(0,255,0), pixels.Color(0,0,255), true);
  /*
  for(int i = 1; i<16; i++){
    pixelCircle(dt_sine, i, pixels.Color(255,0,0), pixels.Color(15*i,15*i,15*i));
  }
  for(int i = 1; i<16; i++){
    pixelCircle(dt_sine, i, pixels.Color(255,255,255), pixels.Color(15*i,0,0));
  } 
  */ 
}

void createConstTimeDelay(int dt, int *dtArray) {
  for(int i = 0; i < NUMPIXELS; i++) {
    dtArray[i] = dt;
  }
}

uint32_t randomColor() {
  //return pixels.Color(random(0,255),random(0,255),random(0,255));

  int randnum;
  do {
    randnum = random(0,NUMCOLORS);
  } while(prevrandnum == randnum);
  prevrandnum = randnum;
  
  return colors[randnum];
}

// clearSwipe
void clearSwipe(int dt[], uint32_t clearCol, uint32_t startAt, uint32_t stopAt, bool invert) {

  int pos;

  if( (stopAt == 0) || (stopAt > NUMPIXELS) ){
    stopAt = NUMPIXELS;
  }  

  if( startAt >= stopAt) {
    return;
  }

  for(int npos = startAt; npos < stopAt; npos++) {

    invert ? pos = npos : pos = NUMPIXELS - npos;

    for(int iPixel = pos; iPixel < TOTALPIXELS; iPixel+= NUMPIXELS) {
      pixels.setPixelColor(iPixel, clearCol);
    }

    pixels.show();
    delay(dt[pos]);
  }
  
}


// doubleSwipe
void doubleSwipe(int dt[], uint32_t pixelCol, uint32_t bgCol, uint32_t stopAt, bool invert) {

  int pos;
  uint32_t colmask[NUMPIXELS];
  uint32_t oneHalf = NUMPIXELS/2;

  if( (stopAt == 0) || (stopAt > oneHalf) ){
    stopAt = oneHalf;
  }

  for(int npos = 0; npos < stopAt; npos++) {

    invert ? pos = npos : pos = oneHalf - npos;

    for(int i = 0; i < oneHalf; i++) {
      (i < pos) ? colmask[i] = pixelCol : colmask[i] = bgCol;
    }

    for(int iPixel = 0; iPixel < TOTALPIXELS; iPixel++) {
      pixels.setPixelColor(iPixel, colmask[iPixel%oneHalf]);
    }

    pixels.show();
    delay(dt[pos]);
  }
  
}

// swipe
void swipe(int dt[], uint32_t pixelCol, uint32_t bgCol, uint32_t stopAt,  bool invert) {

  int pos;
  uint32_t colmask[NUMPIXELS];

  if( (stopAt == 0) || (stopAt > NUMPIXELS) ){
    stopAt = NUMPIXELS;
  }  

  for(int npos = 0; npos < stopAt; npos++) {

    invert ? pos = npos : pos = NUMPIXELS - npos;

    for(int i = 0; i < NUMPIXELS; i++) {
      (i < pos) ? colmask[i] = pixelCol : colmask[i] = bgCol;
    }

    for(int iPixel = 0; iPixel < TOTALPIXELS; iPixel++) {
      pixels.setPixelColor(iPixel, colmask[iPixel%NUMPIXELS]);
    }

    pixels.show();
    delay(dt[pos]);
  }
  
}


// oneThird
void oneThird(int dt[], uint32_t firstCol, uint32_t secondCol, uint32_t thirdCol, bool invert) {
  int pos;
  uint32_t oneThird = NUMPIXELS/3+1;
  uint32_t twoThirds = 2*oneThird;

  uint32_t colmask[NUMPIXELS];

  for(int npos = 0; npos < NUMPIXELS; npos++) {

    invert ? pos = npos : pos = NUMPIXELS - npos;

    for(int i = 0; i < oneThird; i++) {
      colmask[(i+pos)%NUMPIXELS] = firstCol;
      colmask[(i+pos+oneThird)%NUMPIXELS] = secondCol;
      colmask[(i+pos+twoThirds)%NUMPIXELS] = thirdCol;
    }
        
    for(int iRing = 0; iRing < NUMRINGS; iRing++){
      int iOffset = iRing * NUMPIXELS;
      for(int iPixel = 0; iPixel < oneThird; iPixel++){
        pixels.setPixelColor(iPixel + iOffset, colmask[iPixel]);
        pixels.setPixelColor(iPixel + oneThird + iOffset, colmask[iPixel+oneThird]);
        pixels.setPixelColor(iPixel + twoThirds + iOffset, colmask[iPixel+twoThirds]);
      }
    }

    pixels.show();
    delay(dt[pos]);
  } 
}

// two quarters
void twoQuarters(int dt[], uint32_t firstCol, uint32_t secondCol, bool invert) {

  int pos;
  uint32_t oneQuarter = NUMPIXELS/4;
  uint32_t twoQuarters = 2*oneQuarter;
  uint32_t threeQuarters = 3*oneQuarter;

  uint32_t colmask[NUMPIXELS];

  for(int npos = 0; npos < NUMPIXELS; npos++) {

    invert ? pos = npos : pos = NUMPIXELS - npos;

    for(int i = 0; i < oneQuarter; i++) {
      colmask[(i+pos)%NUMPIXELS] = firstCol;
      colmask[(i+pos+oneQuarter)%NUMPIXELS] = secondCol;
      colmask[(i+pos+twoQuarters)%NUMPIXELS] = firstCol;
      colmask[(i+pos+threeQuarters)%NUMPIXELS] = secondCol;
    }
        
    for(int iRing = 0; iRing < NUMRINGS; iRing++){
      int iOffset = iRing * NUMPIXELS;
      for(int iPixel = 0; iPixel < oneQuarter; iPixel++){
        pixels.setPixelColor(iPixel + iOffset, colmask[iPixel]);
        pixels.setPixelColor(iPixel + oneQuarter + iOffset, colmask[iPixel+oneQuarter]);
        pixels.setPixelColor(iPixel + twoQuarters + iOffset, colmask[iPixel+twoQuarters]);
        pixels.setPixelColor(iPixel + threeQuarters + iOffset, colmask[iPixel+threeQuarters]);
      }
    }

    pixels.show();
    delay(dt[pos]);
  }
  
}

// one or more pixels circle around
void pixelCircle(int dt[], int nPixels, uint32_t pixelCol, uint32_t bgCol) {

  // initialize picutre
  for(int iPixel = 0; iPixel < TOTALPIXELS; iPixel++) {
    if((iPixel%NUMPIXELS) < nPixels)
      pixels.setPixelColor(iPixel, pixelCol);
    else if((iPixel%NUMPIXELS - 15) == 00 )
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
