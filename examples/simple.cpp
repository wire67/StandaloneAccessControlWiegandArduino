#include "Arduino.h"
#include "wiegandOutput.h"

#define ARRAY_COUNT(arr) ((int) (sizeof (arr) / sizeof (arr)[0])) // count elements in array

//Initialize at the pin 8 (Data 0) and 9 (Data 1)
WiegandOut wiegandOut(8,9);

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