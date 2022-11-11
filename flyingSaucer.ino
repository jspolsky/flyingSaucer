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
#define BRIGHTNESS 128
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

  static uint8_t cypos = 0;

  EVERY_N_MILLIS(10) {
     fadeToBlackBy(leds_rim, NUM_LEDS_RIM, 16);
  }
  
  EVERY_N_MILLIS(40) {

    if (cypos < NUM_LEDS_RIM) {
      leds_rim[NUM_LEDS_RIM - 1 - cypos] = CHSV(hue_cylon, 255, 255);
    }

    cypos = (cypos + 1) % (NUM_LEDS_RIM * 3 / 2);
   
  }

  EVERY_N_MILLIS(150) {
    hue_cylon++;
  }


  // OUTLINE

  fill_solid(leds_outline, NUM_LEDS_OUTLINE, CHSV(hue_cylon, 64, beatsin8(17, 50, 255)));


  EVERY_N_MILLIS(500) {

    blinky = !blinky;
    digitalWrite( 13, blinky );
    
  }

 FastLED.delay(1);

}
