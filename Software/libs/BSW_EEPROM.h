#include <eep.h>
#define EEPROM_MBS_ID 0x0



void EepromWrite(unsigned int address, unsigned char data);
unsigned char EepromRead(unsigned int address);