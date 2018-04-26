
#include "FastLED.h"

#include <Wire.h>
#include <DS3231.h>
#include <SoftwareSerial.h>

#define TIME_HEADER  "T"  
#define LED_PIN     9
#define NUM_LEDS    60
#define CHIPSET     WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
DS3231  rtc(SDA, SCL);
Time  t;

#define BRIGHTNESS  255
unsigned long timer;
boolean show_rainbow = LOW;
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup()  {
  Serial.begin(115200);
  mySerial.begin(9600);
  rtc.begin();

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness( BRIGHTNESS );
}

void loop()  {
  if (mySerial.available() > 0) {
    if (mySerial.find(TIME_HEADER)) {
      int h = mySerial.parseInt();
      int m = mySerial.parseInt();
      int s = mySerial.parseInt();
      
      mySerial.print(h);
      mySerial.print(',');
      mySerial.print(m);
      mySerial.print(',');
      mySerial.println(s);
      rtc.setTime(h,m,s);
    }
    mySerial.println("OK");
  }

  if (millis() - timer >= 1000) {
    t = rtc.getTime();
    to_analog_clock();
    timer = millis();
  }

  static uint8_t starthue = 0;
  if (show_rainbow == HIGH) {
    fill_rainbow( leds, NUM_LEDS , --starthue, 5);
    FastLED.show();
  }

}

void to_analog_clock() {
  int _hour = t.hour>=12? t.hour-12: t.hour;

  for (byte i = 0; i <= 59; i++) leds[i] = CRGB::Black;
  for (byte i = 0; i <= 59; i += 5) leds[i] = CRGB::White;
  leds[_hour * 5 + t.min / 12] = CRGB::Red;

  leds[t.min] = CRGB::Green;
  if (_hour * 5 + t.min / 12 == t.min) leds[t.min] = CRGB::Yellow;

  leds[t.sec] = CRGB::Blue;
  if (_hour * 5 + t.min / 12 == t.sec) leds[t.sec] = CRGB(255, 0, 255);
  if (t.min == t.sec) leds[t.sec] = CRGB(0, 255, 255);
  if (_hour * 5 + t.min / 12 == t.min && t.min == t.sec) leds[t.sec] = CRGB(255, 255, 255);

  if (t.sec == 0 && t.min%5 == 0) show_rainbow = HIGH;
  else show_rainbow = LOW;

  FastLED.show();
}
