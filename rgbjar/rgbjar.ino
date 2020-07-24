// http://colorschemedesigner.com/csd-3.5/
// https://www.mathsisfun.com/hexadecimal-decimal-colors.html

// programming attiny85 https://www.instructables.com/id/How-to-Program-an-Attiny85-From-an-Arduino-Uno/

// usbasp working on windows 10 https://electronics.stackexchange.com/questions/416714/avrdude-does-not-recognize-usbasp-device/417509#417509

/* attiny85 pinout
 * 1 - reset
 * 2 - a3 / 3
 * 3 - a2 / 4
 * 4 - ground
 * 5 - 0 (pwm)
 * 6 - 1 (pwm)
 * 7 - a1 / 2
 * 8 - 2.7-5.5V
 */
 
#include <Adafruit_NeoPixel.h>

// SETTINGS ////////////////////
#define PIXEL_BRIGHTNESS    40
#define COLOR_CHANGE_WAIT   20
#define LOOP_DELAY          10
#define NUMBER_OF_PIXELS    2
#define COLOR_CHANGE_SPEED  0.3

// PINS /////////////////////////
#define NEOPIXEL_PIN        3
#define PHOTO_RESISTOR_PIN  A1

float r1 = 0.0;
float g1 = 0.0;
float b1 = 0.0;
float r2 = 0.0;
float g2 = 0.0;
float b2 = 0.0;
float tr1 = 0.0;
float tg1 = 0.0;
float tb1 = 0.0;
float tr2 = 0.0;
float tg2 = 0.0;
float tb2 = 0.0;
float fade = 0.0;

unsigned long displayChangeTime = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMBER_OF_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void changeStyle(int forceStyle = 99);

void setup() {

  // pinMode(PHOTO_RESISTOR_PIN, INPUT); // max value 1023
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(PIXEL_BRIGHTNESS);
  randomSeed(analogRead(0));
  changeStyle(0);
}

void loop() {
/*
  if (analogRead(PHOTO_RESISTOR_PIN) > 650) {
    
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.show();
    return;
  }
  */
  
  if ((millis() - displayChangeTime) >= (COLOR_CHANGE_WAIT * 1000)) { // 60 s * m

    // change colors
    
    changeStyle();
    displayChangeTime = millis();
  }

  // fade colors towards targets

  r1 = moveTowards(r1, tr1, COLOR_CHANGE_SPEED);
  g1 = moveTowards(g1, tg1, COLOR_CHANGE_SPEED);
  b1 = moveTowards(b1, tb1, COLOR_CHANGE_SPEED);
  r2 = moveTowards(r2, tr2, COLOR_CHANGE_SPEED);
  g2 = moveTowards(g2, tg2, COLOR_CHANGE_SPEED);
  b2 = moveTowards(b2, tb2, COLOR_CHANGE_SPEED);

  // calculate fade
  fade = 0.3 + (((sin(millis() / 1100.0) + 1.0) + (sin(millis() / 1500.0) + 1.0) + (cos(millis() / 900.0) + 1.0)) / 15.0);

  // display them
  
  pixels.setPixelColor(0, pixels.Color(min(255, int(r1 * fade)), min(255, int(g1 * fade)), min(255, int(b1 * fade))));
  pixels.setPixelColor(1, pixels.Color(min(255, int(r2 * fade)), min(255, int(g2 * fade)), min(255, int(b2 * fade))));
  pixels.show();
      
  delay(LOOP_DELAY);
}

void changeStyle(int forceStyle = 99) {
  
    int style = int(random(6));

    if (forceStyle < 99)
      style = forceStyle;

    switch (style) {
      case 0:
        tr1 = random(255);
        tg1 = random(255);
        tb1 = random(255);
        tr2 = random(255);
        tg2 = random(255);
        tb2 = random(255);
        break;
      
      case 1:
        tr1 = random(255);
        tg1 = random(255);
        tb1 = random(255);
        break;
      
      case 2:
        tr2 = random(255);
        tg2 = random(255);
        tb2 = random(255);
        break;
      
      default:
        
        float tmpVal = tr1;
        tr1 = tr2;
        tr2 = tmpVal;
  
        tmpVal = tg1;
        tg1 = tg2;
        tg2 = tmpVal;
  
        tmpVal = tb1;
        tb1 = tb2;
        tb2 = tmpVal;
        break;
    }
}

float moveTowards(float current, float target, float changeSpeed) {

  if (current < target)
    return current += changeSpeed;
  else if (current > target)
    return current -= changeSpeed;

  return target;
}
