#define STR_SUCCESS     0
#define STR_FAIL        1



 void mymemset(unsigned char *buffer, unsigned char val,unsigned int len);
 unsigned char Long2AsciiPad(unsigned char *buff,unsigned long val,unsigned char padlen,unsigned char padval);
 unsigned char Long2DigitsPad(unsigned char *buff,unsigned long val,unsigned char padlen,unsigned char padval);
 unsigned char Value2Parameter(unsigned char *buff,unsigned long val,unsigned char padlen, unsigned char unit);
 unsigned char Value2Parameter2(unsigned char *buff,unsigned long val,unsigned char padlen, unsigned char unit);
  unsigned char Hex2AsciiPad(unsigned char *buff,unsigned long val,unsigned char padlen,unsigned char padval);