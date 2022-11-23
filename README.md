# Overview
RemoteNote is a simple system for displaying text on an E-Ink screen for loved ones. Use a Adafruit Feather HUZZAH with ESP8266, Adafruit 2.9" Grayscale eInk / ePaper Display FeatherWing, and any microSD card of your choosing. The ESP8266 is used to poll an NTP time server, which is then used to check the current date against notes saved on the SD card. If one matching the current date is there, it is displayed. If not, one is generated from a tree of sappy lines I came up with.

# Setup
Parts List:
1. Adafruit Feather HUZZAH with ESP8266 (P/N 2821), w/ headers
2. Adafruit 2.9" Grayscale eInk display (P/N 4777)
3. MicroSD card
4. (Optional) 3D printed stand (STL file is in repo)
5. (Optional) Four M2x20 screws, four M2 nuts to secure electronics to 3D printed stand

Process:
1. Solder male headers to MCU.
2. Load message text files on microSD card if you'd like specific messages on certain days. See my sample text files for a starting point. Skip this if you'd rather have all messages be generated on the fly. Insert the microSD card into eInk display's card expansion slot.
3. Gently seat MCU's male headers into display's female headers.
5. If you're using my 3D printed stand, line up the display/MCU with the mounting holes and secure with screws/nuts.
6. Edit Arduino sketch to add your WiFi network's SSID and password. If you want to change the message generation tree, do so now.
7. Connect MCU to PC, fire up Arduino IDE, and upload the sketch. (If you have trouble setting up Arduino IDE for the MCU, see [Adafruit's guide](https://learn.adafruit.com/adafruit-feather-328p-atmega328-atmega328p)). 
8. Watch your messages scroll by!
