# Standalone Access Control Wiegand Arduino

![Standalone Access Control](https://images.tokopedia.net/img/cache/700/VqbcmM/2023/3/22/a900c778-e943-4c33-a7d1-ccd1fcf67578.jpg)

# Description

Arduino library to remotely configure a `Standalone Access Control` over its Wiegand input.

It will simulate button presses over Wiegand, as if an admin was manually typing them on the keypad.

Enter programming mode, change master code, add user PIN, delete user pin, change user PIN, and any custom command.

Main advantage of `Standalone Access Control` is that they are very cheap, they work autonomously and offline. Disavantage is that it needs to be manually configured using the keypad, this is where this library comes in, allowing for remove administration via Wiegand Input.

# Tested hardwares

ESP32 / ESP8266

# How to use

1. Include this library wiegandOutput.h
2. Create a WiegandOut object: WiegandOut wiegandOut(8,9);  -> Wiegand object initialized with Data0 at pin 8 and Data 1 at pin 9
3. Use the method send to send your card number.

# Simple example

```
#include "Arduino.h"
#include "wiegandOutput.h"

#define ARRAY_COUNT(arr) ((int) (sizeof (arr) / sizeof (arr)[0])) // count elements in array

WiegandOut wiegandOut(8,9);
//WiegandOut wiegandOut(8,9,true); -> it will enable the serial output with the binary wiegand data as ascii to debug;

long count = 0;

void setup() {
   // enter programming mode (master code 999999)
   wiegandOut.enterProgrammingMode("999999");

   // change master code (new master code 999999)
   wiegandOut.changeMasterCode("999999");

   // add a PIN user ID 7 pin 1234
   wiegandOut.addPinUser("7", "1234");

   // delete a PIN user ID 7
   wiegandOut.deletePinUser("7");

   /*** custom 1 ***/

   // delete a PIN user ID 7
   wiegandOut.writeString("27#");

   /*** custom 2 ***/

   // delete a PIN user ID 7
   const uint8_t arDelUsr[]={2,7,12};
   wiegandOut.writeArray(arDelUsr, ARRAY_COUNT(arDelUsr));

   /*** custom 3 ***/

   // delete a PIN user ID 7
   wiegandOut.writeChar('2');
   delay(500);
   wiegandOut.writeChar('7');
   delay(500);
   wiegandOut.writeChar('#');
   delay(500);
}

void loop(){
delay(1000);
}
```
