#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define DATA_PIN_WINDOW    2
#define DATA_PIN_OUTLINE   14
#define DATA_PIN_RIM       7

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS_WINDOW    21
#define NUM_LEDS_OUTLINE   144
#define NUM_LEDS_RIM 15
#define BRIGHTNESS 255
CRGB leds_window[NUM_LEDS_WINDOW];
CRGB leds_outline[NUM_LEDS_OUTLINE];
CRGB leds_rim[NUM_LEDS_RIM];


void setup() {


  // delay(1000);
  FastLED.addLeds<LED_TYPE,DATA_PIN_WINDOW,COLOR_ORDER>(leds_window, NUM_LEDS_WINDOW).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN_OUTLINE,COLOR_ORDER>(leds_outline, NUM_LEDS_OUTLINE).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN_RIM,RGB>(leds_rim, NUM_LEDS_RIM).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(BRIGHTNESS);

  pinMode(13, OUTPUT);
  

}

void loop() {

  static uint8_t hue = 0;

  static uint8_t hue_cylon = 0;

  static uint8_t hue_outline = 0;

  static int blinky = HIGH;

  EVERY_N_MILLIS(10) {
    hue = (hue + 1) % 0xFF;



  }


  // WINDOW
  uint16_t  x;
  int       scale;
  uint16_t  t;

  x = 0;
  t = millis() / 5;
  scale = beatsin8(10, 10, 30);

  for (int i = 0; i < NUM_LEDS_WINDOW; i++) {
      uint8_t noise = inoise8(i * scale + x, t);
      uint8_t hue = map(noise, 50, 190, 0, 255);
      leds_window[i] = CHSV(hue, 255, 128);
  }
  

  // RIM (the 15 little windows)

  EVERY_N_MILLIS(10) {

    uint16_t cypos = scale8( cubicwave8( millis() / 10 ), NUM_LEDS_RIM - 1 );
    
    fadeToBlackBy(leds_rim, NUM_LEDS_RIM, 16);

    leds_rim[cypos] = CHSV(hue_cylon, 255, 255);
   
  }

  EVERY_N_MILLIS(60) {
    hue_cylon++;
  }


  // OUTLINE

  static uint16_t pos1 = 0;
  static uint16_t pos2 = NUM_LEDS_OUTLINE / 2;
  
  EVERY_N_MILLIS(2) {

    pos1 = (pos1 + 1) % (NUM_LEDS_OUTLINE-1);
    pos2 = (pos2 + 1) % (NUM_LEDS_OUTLINE-1);

    leds_outline[pos1+1] = 
    leds_outline[pos2+1] =
    CHSV(hue_outline, 255, 255);
       
    fadeToBlackBy(leds_outline, NUM_LEDS_OUTLINE, 16);
    
  }

  EVERY_N_MILLIS(50) {
    hue_outline++;
  }



  EVERY_N_MILLIS(500) {

    blinky = !blinky;
    digitalWrite( 13, blinky );
    
  }

 FastLED.delay(1);

}
