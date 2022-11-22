/*
 * Written by Mason Watts, Dec 1 2021
 * Basic code for a rotating message display for a loved one/spouse. 
 * Uses a Adafruit Feather HUZZAH w/ ESP8266 (Product #2821) and Adafruit 2.9" Grayscale eInk Display FeatherWing.
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

const char* ssid     = "SSID";
const char* password = "PASSWORD";

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
    long seed = random();
    String myString = "";
    // "I"
    if(seed % 2 == 0)
    {
      myString += "I ";

      seed = random();
      // "love"
      if(seed % 2 == 0)
      {
        myString += "love ";

        int optionSize = 11;
        String options[optionSize] = 
        {
          "you",
          "your personality",
          "your eyes",
          "our cuddles",
          "movie nights",
          "our dates",
          "hiking together",
          "posing for photos",
          "travelling together",
          "our family",
          "our home"
        };
        
        seed = random(0, optionSize);
        myString += options[seed];
      }
      // "appreciate"
      else
      {
        myString += "appreciate ";

        int optionSize = 7;
        String options[optionSize] = 
        {
          "you",
          "your caring",
          "all you do",
          "your kindness",
          "your healthy help",
          "your strength",
          "your love"
        };
        
        seed = random(0, optionSize);
        myString += options[seed];
      }
    }
    // "You"
    else
    {
      myString += "You ";

      seed = random();
      // "make me"
      if(seed % 2 == 0)
      {
        myString += "make me ";

        int optionSize = 4;
        String options[optionSize] = 
        {
          "happy",
          "fulfilled",
          "proud",
          "feel loved"
        };
        
        seed = random(0, optionSize);
        myString += options[seed];
      }
      // "are"
      else
      {
        myString += "are ";

        int optionSize = 12;
        String options[optionSize] = 
        {
          "loved",
          "beautiful",
          "amazing",
          "strong",
          "kind",
          "my person",
          "the one for me",
          "sexy",
          "intelligent",
          "brave",
          "loyal",
          "valued"
        };
        
        seed = random(0, optionSize);
        myString += options[seed];
      }
    }

    seed = random();
    if(seed % 3 == 0)
    {
      myString += ".";
    }
    else if(seed % 3 == 1)
    {
       myString += "!";
    }
    else
    {
      myString += " <3";
    }

    myString.toCharArray(text, 80);
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

  display.setCursor(10, 40);
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

  randomSeed(analogRead(0));
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
