#include "Arduino.h"
#include "wiegandOutput.h"

#define ARRAY_COUNT(arr) ((int) (sizeof (arr) / sizeof (arr)[0])) // count elements in array

//Initialize at the pin 8 (Data 0) and 9 (Data 1)
WiegandOut wiegandOut(8,9);
#define MASTER_CODE  "999999" // default master code 999999

void demo1(void)
{
   // enter programming mode
   wiegandOut.enterProgrammingMode(MASTER_CODE);

   // change master code
   wiegandOut.changeMasterCode(MASTER_CODE);

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

// -------

void setup() {
   demo1();
}

void loop(){
    delay(10000);
}

/* EOF */
