#include "SevSeg.h"
#include <Wire.h>
#include <DS3231.h>

DS3231 clock;
RTCDateTime dt;
SevSeg sevseg;  //Initiate a seven segment controller object

int sensorDigitalPin = 13;
int digitalValue;

void setup() {
  byte numDigits = 4;
  byte digitPins[] = { 2, 3, 4, 5 };
  byte segmentPins[] = { 6, 7, 8, 9, 10, 12, 11, 13 };
  bool resistorsOnSegments = 0;
  // variable above indicates that 4 resistors were placed on the digit pins.
  // set variable to 1 if you want to use 8 resistors on the segment pins.
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);
  clock.begin();
  Serial.begin(9600);
  pinMode(sensorDigitalPin, INPUT);

  // Manual (YYYY, MM, DD, HH, II, SS
  // clock.setDateTime(2016, 12, 9, 11, 46, 00);

  // Send sketch compiling time to Arduino
  clock.setDateTime(__DATE__, __TIME__);
}
bool on = false;
int prev = 1;
bool recording = false;
unsigned long timer;
void loop() {
  digitalValue = digitalRead(sensorDigitalPin);
  if (on) {
    dt = clock.getDateTime();
    sevseg.setNumber(dt.hour * 100 + dt.minute, 2);  // Must run repeatedly
  } else {
    sevseg.blank();
  }
  Serial.println("Value" + String(digitalValue));
  Serial.print(on);
  if (digitalValue == 0 && prev) {
    if (!recording) {
      Serial.println("HEREHEREHEREHEREHERER");
      recording = true;
      timer = micros();
      delay(100);
    } else {
      unsigned long currentTime = micros();
      Serial.println(currentTime - timer);
      if (currentTime - timer < 1000000) {
        on = !on;
      }
      recording = false;
    }
  }
  prev = digitalValue;
  sevseg.refreshDisplay();
}