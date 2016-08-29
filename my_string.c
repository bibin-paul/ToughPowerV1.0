#include "my_string.h"
const unsigned char HEX_ARRAY[]={"0123456789ABCDEFG"};
 /****************************************************************************
FUNCTION: mymemset

DESCRIPTION:
    FILLS A BUFFER WITH VALUE.

ARGUMENTS:

        Argument                             Description                        Type
---------------------------------------------------------------------------------------------
        buffer                   Buffer where values are filled              * unsigned char
        val                      Value to be filled                           unsigned char
        len                      Number of bytes to be filled                 unsigned int

RETURN VALUE:
    NONE.

PRE-CONDITIONS:
    NONE.

POST-CONDITIONS:
    NONE.

IMPORTANT NOTES:
    Array length is not checked, caller should check boundaries
*****************************************************************************/
void mymemset(unsigned char *buffer, unsigned char val,unsigned int len){
/* fill each character of array with the value for len number of positions */
        unsigned int idx;
        for (idx = 0; idx < len; idx++){
                buffer[idx] = val;                //set the value to buffer
        }
}
 /****************************************************************************
FUNCTION: Long2DigitsPad

DESCRIPTION:
        THIS FUNCTION CONVERTS HEXADECIMAL VALUE TO DIGIT STRING WITH PADDING FOR CONSTANT LENGTH.

ARGUMENTS:

Argument                             Description                                                     Type
----------------------------------------------------------------------------------------------------------------------------
*buffer                               ATCOMMAND TO BE SENT.                                     const unsigned char *
val                                   LONG VALUE TO BE CONVERTED                                unsigned long
padlen                                STRING PADDING SIZE                                        unsigned char
padval                                CHARACTER TO BE PADDED ON LEADING                          unsigned char

RETURN VALUE:
        STR_FAIL :                    ON OVERFLOW OF VALUE
        STR_SUCCESS:                  SUCCESS FULL CONVERSION

PRE-CONDITIONS:
    NONE.

POST-CONDITIONS:
    NONE.

IMPORTANT NOTES:
    None.
*****************************************************************************/


unsigned char Long2DigitsPad(unsigned char *buff,unsigned long val,unsigned char padlen,unsigned char padval)
{
        unsigned char ctr;
        if(padlen == 0){
                return(STR_FAIL);
        }
        mymemset(buff,padval,padlen);
        buff[padlen--] = 0;
        buff[padlen] = 0x0A;
        for(ctr = 0;val > 0;ctr++) {
                buff[padlen] = (unsigned char)(val %10);
                val = val / 10;
                if((padlen-- == 0)&&(val > 0)){
                        return(STR_FAIL);
                }
        }
        return(STR_SUCCESS);
}

 /****************************************************************************
FUNCTION: Long2AsciiPad

DESCRIPTION:
        THIS FUNCTION CONVERTS HEXADECIMAL VALUE TO STRING WITH PADDING FOR CONSTANT LENGTH.

ARGUMENTS:

Argument                             Description                                                     Type
----------------------------------------------------------------------------------------------------------------------------
*buffer                               ATCOMMAND TO BE SENT.                                     const unsigned char *
val                                   LONG VALUE TO BE CONVERTED                                unsigned long
padlen                                STRING PADDING SIZE                                        unsigned char
padval                                CHARACTER TO BE PADDED ON LEADING                          unsigned char

RETURN VALUE:
        STR_FAIL :                    ON OVERFLOW OF VALUE
        STR_SUCCESS:                  SUCCESS FULL CONVERSION

PRE-CONDITIONS:
    NONE.

POST-CONDITIONS:
    NONE.

IMPORTANT NOTES:
    None.
*****************************************************************************/


unsigned char Long2AsciiPad(unsigned char *buff,unsigned long val,unsigned char padlen,unsigned char padval)
{
        unsigned char ctr;
        if(padlen == 0){
                return(STR_FAIL);
        }
        mymemset(buff,padval,padlen);
        buff[padlen--] = 0;
        buff[padlen] = '0';
        for(ctr = 0;val > 0;ctr++) {
                buff[padlen] = (unsigned char)(val %10) + 0x30;
                val = val / 10;
                if((padlen-- == 0)&&(val > 0)){
                        return(STR_FAIL);
                }
        }
        return(STR_SUCCESS);
}

/****************************************************************************
FUNCTION: Value2Parameter

DESCRIPTION:
        THIS FUNCTION CONVERTS HEXADECIMAL VALUE TO STRING WITH DECIMAL POINT AND UNIT.

ARGUMENTS:

Argument                             Description                                                     Type
----------------------------------------------------------------------------------------------------------------------------
*buffer                               ATCOMMAND TO BE SENT.                                     const unsigned char *
val                                   LONG VALUE TO BE CONVERTED                                unsigned long
decimal                               POSITION FOR DECIMAL POINT                                unsigned char
unit                                  CHARACTER TO BE PADDED FOR UNIT                           unsigned char

RETURN VALUE:
        STR_FAIL :                    ON OVERFLOW OF VALUE
        STR_SUCCESS:                  SUCCESS FULL CONVERSION

PRE-CONDITIONS:
    NONE.

POST-CONDITIONS:
    NONE.

IMPORTANT NOTES:
    None.
*****************************************************************************/


unsigned char Value2Parameter(unsigned char *buff,unsigned long val,unsigned char padlen, unsigned char unit){
        unsigned char ctr;
        unsigned char ptr;
        if(padlen == 0){
                return(STR_FAIL);
        }
        ptr=padlen;
        mymemset(buff,'0',padlen);
        buff[padlen] = 0;
        padlen--;
        buff[padlen]=unit;
        padlen--;
        ptr= padlen;
        padlen--;
       for(ctr = 0;val > 0;ctr++) {
                buff[padlen] = (unsigned char)(val %10) + 0x30;
                val = val / 10;
                if((padlen-- == 0)&&(val > 0)){
                        return(STR_FAIL);
                }
        }
        buff[ptr] = buff[ptr-1];
        
        buff[(ptr-1)] = '.';
        return(STR_SUCCESS);
}

unsigned char Value2Parameter2(unsigned char *buff,unsigned long val,unsigned char padlen, unsigned char unit){
        unsigned char ctr;
        unsigned char ptr;
        if(padlen == 0){
                return(STR_FAIL);
        }
        ptr=padlen;
        mymemset(buff,'0',padlen);
        buff[padlen] = 0;
        padlen--;
        buff[padlen]=unit;
        padlen--;
        ptr= padlen;
        padlen--;
       for(ctr = 0;val > 0;ctr++) {
                buff[padlen] = (unsigned char)(val %10) + 0x30;
                val = val / 10;
                if((padlen-- == 0)&&(val > 0)){
                        return(STR_FAIL);
                }
        }
        buff[ptr] = buff[ptr-1];
        buff[ptr-1] = buff[ptr-2];
        buff[(ptr-2)] = '.';
        return(STR_SUCCESS);
}


 unsigned char Hex2AsciiPad(unsigned char *buff,unsigned long val,unsigned char padlen,unsigned char padval){
        unsigned char ctr;
        unsigned char byte;
        if(padlen == 0){
                return(STR_FAIL);
        }
        mymemset(buff,padval,padlen);
        buff[padlen--] = 0;
        buff[padlen] = '0';
        for(ctr = 0;val > 0;ctr++) {
                byte=(unsigned char) val&0x000F;

                buff[padlen] = HEX_ARRAY[byte];
                val = val>>4;
                if((padlen-- == 0)&&(val > 0)){
                        return(STR_FAIL);
                }
        }
        return(STR_SUCCESS);
}