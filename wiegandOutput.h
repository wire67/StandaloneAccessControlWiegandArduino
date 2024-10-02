#ifndef _WIEGANDOUT_H
#define _WIEGANDOUT_H

class WiegandOut{
  public:
    WiegandOut(int data0, int data1, bool enableDebug = false);
    void begin(int pinD0, int pinD1);
    void send(unsigned long data, unsigned int bits, bool useFacilityCode);
    void enterProgrammingMode(const char* masterCode);
    void exitProgrammingMode(void);
    void changeMasterCode(const char* newMasterCode);
    void addPinUser(const char* userId, const char* userPin);
    void deletePinUser(const char* userId);
    void deleteAllUsers(void);
    void changePinUser(const char* userId, const char* oldUserPin, const char* newUserPin);
    void writeArray(const uint8_t* myDigits, const uint8_t length);
    void writeString(const char* digitString);
    void writeChar(const char digitChar);

  private:
    void sendD1();
    void sendD0();
    void createParity(unsigned long data, unsigned int bits, bool useFacilityCode);
    int _pinData0;
    int _pinData1;
    int _evenParity;
    int _oddParity;
    bool _enableDebug;
};

#endif // _WIEGANDOUT_H