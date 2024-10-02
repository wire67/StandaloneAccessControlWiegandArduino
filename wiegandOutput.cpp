#include <Arduino.h>
#include "wiegandOutput.h"
#if (defined(__AVR__))
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

const int DELAY_PULSE_SHORT = 100;  //100us
const int DELAY_PULSE_LONG = 2000; //2000us


//Class constructor. Determine the pins D0 and D1
WiegandOut::WiegandOut(int data0, int data1, bool enableDebug){
    _pinData0 = 0;
    _pinData1 = 0;
    _oddParity = 1;
    _evenParity = 0;
    _enableDebug = enableDebug;
    begin(data0,data1);
}

/*Initialize the pins used to transmit wiegand data*/
void WiegandOut::begin(int data0 , int data1)
{
    pinMode(data0, OUTPUT);					// Set D0 pin as output
	pinMode(data1, OUTPUT);					// Set D1 pin as output
    digitalWrite(data0,HIGH);
    digitalWrite(data1,HIGH);
    _pinData0 = data0;                      //set as global
    _pinData1 = data1;                      //set as global
}

/*Create wiegand parity*/
void WiegandOut::createParity(unsigned long data, unsigned int bits, bool useFacilityCode){
    _evenParity = 0;
    _oddParity = 1;
    unsigned long int tempData = 0;

    if(!useFacilityCode){
        if(bits==34){
            data = data & 0x00FFFFFF; //force facility (first 8 msb as 0) as 0x00
        }
        else if(bits==26){
            data = data & 0x00FFFF; //force facility (first 8 msb as 0) as 0x00
        }
    }

    //34 bits -> use 32 to check parity ((34-2)/2) = 16 even / 16 odd
    //26 bits -> use 24 to check parity ((26-2)/2) = 12 even / 12 odd
    int b = (bits-2)/2;
    for (int i = 0; i < b; i++)
    {

        tempData = data>>i;
        if(tempData & 0x01){        //this count the qty of 1 and do a xor to complete the even parity;
            _oddParity = _oddParity ^ 1;
        }
    }

    //34 bits -> use 32 to check parity ((34-2)/2) = 16 even / 16 odd
    //26 bits -> use 24 to check parity ((26-2)/2) = 12 even / 12 odd
    tempData=0;
    for (int i = b; i < bits-2; i++)
    {
        tempData = data>>i;
        if(tempData & 0x01){        //this count the qty of 1 and do a xor to complete the odd parity;
            _evenParity = _evenParity ^ 1;
        }
    }

}

/*Send D0 pin pulse*/
void WiegandOut::sendD0(){
    digitalWrite(_pinData0,LOW);
    #if(defined(__AVR__))
         _delay_us(DELAY_PULSE_SHORT);
    #elif defined(ESP32) || defined(ESP8266)
        delayMicroseconds(DELAY_PULSE_SHORT);
    #endif
    digitalWrite(_pinData0,HIGH);
    #if(defined(__AVR__))
         _delay_us(DELAY_PULSE_LONG);
    #elif defined(ESP32) || defined(ESP8266)
        delayMicroseconds(DELAY_PULSE_LONG);
    #endif
    if(_enableDebug){
        Serial.print("0");
    }

}

/*Send D1 pin pulse*/
void WiegandOut::sendD1(){
    digitalWrite(_pinData1,LOW);
    #if(defined(__AVR__))
         _delay_us(DELAY_PULSE_SHORT);
    #elif defined(ESP32) || defined(ESP8266)
        delayMicroseconds(DELAY_PULSE_SHORT);
    #endif
    digitalWrite(_pinData1,HIGH);
    #if(defined(__AVR__))
         _delay_us(DELAY_PULSE_LONG);
    #elif defined(ESP32) || defined(ESP8266)
        delayMicroseconds(DELAY_PULSE_LONG);
    #endif
    if(_enableDebug){
        Serial.print("1");
    }

}

/*Send wiegand data
@data = wiegand number
@bits = qty of bits (only 26 and 34 was implemented)
@useFacilityCode = If false, it will erase facility code from @data
*/
void WiegandOut::send(unsigned long data, unsigned int bits, bool useFacilityCode){
    unsigned long tempData =0;

    if(bits==4){  //send 4 bits data wiegand
        for (int i = 0; i < (bits); i++)
            {
                tempData = data << i;
                if(tempData & 0x08){
                    sendD1();
                }else{
                    sendD0();
                }
            }
            return;
    }

    if(!useFacilityCode){
        if(bits==34){
            data = data & 0x00FFFFFF; //force facility (first 8 msb as 0) as 0x00
        }
        else if(bits==26){
            data = data & 0x00FFFF; //force facility (first 8 msb as 0) as 0x00
        }
    }

    createParity(data,bits,useFacilityCode);
    tempData = data;
    //send even parity
    if(_evenParity){sendD1();}else{sendD0();}

    if(bits==26){  //send 24 bits data wiegand
        for (int i = 0; i < (bits-2); i++)
            {
                tempData = data << i;
                if(tempData & 0x00800000){
                    sendD1();
                }else{
                    sendD0();
                }
            }
    }
    else if(bits==34){      //send 32 bits data wiegand
        for (int i = 0; i < (bits-2); i++)
            {
                tempData = data << i;
                if(tempData & 0x80000000){
                    sendD1();
                }else{
                    sendD0();
                }
            }
    }
    //send odd parity bit
    if(_oddParity){sendD1();}else{sendD0();}
    if(_enableDebug){
        Serial.println();
    }

}


void WiegandOut::enterProgrammingMode(const char* masterCode)
{
   writeString("*");
   writeString(masterCode);
   writeString("#");
}

void WiegandOut::exitProgrammingMode(void)
{
   writeString("*");
}

void WiegandOut::changeMasterCode(const char* newMasterCode)
{
   writeString("0");
   writeString(newMasterCode);
   writeString("#");
   writeString(newMasterCode);
   writeString("#");
}

void WiegandOut::addPinUser(const char* userId, const char* userPin)
{
   writeString("1");
   writeString(userId);
   writeString("#");
   writeString(userPin);
   writeString("#");
}

void WiegandOut::deletePinUser(const char* userId)
{
   writeString("2");
   writeString(userId);
   writeString("#");
}

void WiegandOut::deleteAllUsers(void)
{
   writeString("20000#");
}

/**
 * Note: changePinUser will exit programming mode
 */
void WiegandOut::changePinUser(const char* userId, const char* oldUserPin, const char* newUserPin)
{
   exitProgrammingMode();
   writeString("*");
   writeString(userId);
   writeString("#");
   writeString(oldUserPin);
   writeString("#");
   writeString(newUserPin);
   writeString("#");
   writeString(newUserPin);
   writeString("#");
}

void WiegandOut::writeArray(const uint8_t* myDigits, const uint8_t length)
{
   for(uint8_t i=0;i<length;i++)
   {
      send(myDigits[i],4,false);
      delay(500);
   }
}

void WiegandOut::writeString(const char* digitString)
{
   for(uint8_t i=0;i<strlen(digitString);i++)
   {
      writeChar(digitString[i]);
      delay(500);
   }
}

void WiegandOut::writeChar(const char digitChar)
{
   uint8_t myDigit;
   switch(digitChar)
   {
      case '*':
         myDigit=10;
         break;
      case '#':
         myDigit=11;
         break;
      default:
         myDigit=digitChar-'0';
         break;
   }
   send(myDigit,4,false);
}
