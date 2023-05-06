#include <FastLED.h>

#define NUM_LEDS  150
#define DATA_PIN  26
//#define CLOCK_PIN 3

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  //pinMode(DATA_PIN, OUTPUT); 
  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
  int i = 0;
  int j = 0;

  Serial.println("looping5!");
  
  for(j=0; j< NUM_LEDS / 10; j++)
  {
    for(i = 0; i < 10; i++)
    {
      Serial.println(i);
      Serial.println(j);
      Serial.println(j%3);
      Serial.println("---------");
      if(j % 3 == 0)
      {
        leds[i+j*10] = CRGB(255, 0, 0);
      }
      if(j % 3 == 1)
      {
        leds[i+j*10] = CRGB(0, 255, 0);
      }
      if(j % 3 == 2)
      {
        leds[i+j*10] = CRGB(0, 0, 255);
      }
    }
  }
  FastLED.show();

  // digitalWrite(DATA_PIN, HIGH); // sets the digital pin 13 on
  // delay(1000);            // waits for a second
  // digitalWrite(DATA_PIN, LOW);  // sets the digital pin 13 off
  
  delay(10000);         
}