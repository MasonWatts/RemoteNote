/*
 * Written by Mason Watts, Dec 1 2021
 * Basic code for a dynamic name tag. 
 * Uses a Adafruit Feather HUZZAH w/ ESP8266 (Product #2821) and Adafruit 2.9" Grayscale eInk Display FeatherWing.
 * Iterates over images (.bmp) in the root directory of the loaded SD card, waiting a short period before displaying the next image.
 * This display supports a maximum image resolution of 296 x 128.
 */

#include <ESP8266WiFi.h>
//#include <Arduino.h>
#include <AceTimeClock.h>
#include <AceTime.h>
#include "Adafruit_ThinkInk.h"
#include "SdFat.h"
//using namespace sdfat;
//#include <SDFS.h>

using namespace ace_time;
using namespace ace_time::clock;

#define REFRESH_TIME_MS (1000 * 60)

// Pin assignments - ESP8266
#define SD_CS        2
#define SRAM_CS     16
#define EPD_CS       0
#define EPD_DC      15  
#define EPD_RESET   -1
#define EPD_BUSY    -1

SdFat sd;

const char* ssid     = "Fios-w3XKz";
const char* password = "shout63fuel67bar";

static BasicZoneProcessor timeZoneProcessor;
static NtpClock ntpClock, ntpClock2;
static TimeZone timezone;

// 2.9" Grayscale Featherwing or Breakout:
ThinkInk_290_Grayscale4_T5 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

void printScreen(ZonedDateTime localTime)
{
  char fileName[40];
  char text[80];

  for(int c = 0; c < sizeof(text); ++c)
  {
    text[c] = '\0';
  }
  
  snprintf(fileName, sizeof(fileName), "%i_%i.txt", localTime.month(), localTime.day());
  Serial.println(fileName);

  if(sd.exists(fileName))
  {
    SdFile myFile;
    myFile.open(fileName);
    myFile.read((uint8_t*)&text[0], sizeof(text));
    myFile.close();
  }
  else
  {
    snprintf(text, sizeof(text), "I LOVE YOU! Wait for a note.");
  }
  
  Serial.println(text);
  
  display.clearBuffer();
  display.setTextSize(3);
  display.setTextColor(EPD_BLACK);
  display.setCursor(20, 5);
  display.print(localTime.month());
  display.print("/");
  display.print(localTime.day());
  display.print("/");
  display.print(localTime.year());

  display.setCursor(20, 40);
  display.print(text);
  
  display.display();
}

void setup(void) 
{
  Serial.begin(9600);

  timezone = TimeZone::forZoneInfo(&zonedb::kZoneAmerica_New_York, &timeZoneProcessor);
  acetime_t nowSeconds;

  ntpClock.setup(ssid, password);
  if (!ntpClock.isSetup()) 
  {
    Serial.println(F("WiFi connection failed... try again."));
    return;
  }

  display.begin(THINKINK_GRAYSCALE4);

  sd.begin(SD_CS, SD_SCK_MHZ(10));
}

void loop() 
{
  auto nowSeconds = ntpClock.getNow();
  ZonedDateTime localTime = ZonedDateTime::forEpochSeconds(nowSeconds, timezone);
  
  Serial.print("Time: ");
  localTime.printTo(Serial);
  Serial.println();

  unsigned long currentDay = localTime.day();
  unsigned long currentMonth = localTime.month();

  printScreen(localTime);

  delay(REFRESH_TIME_MS);
}
