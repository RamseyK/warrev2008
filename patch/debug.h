#ifndef DEBUG_H
#define DEBUG_H

#include "Common.h"

class Debug
{
public:
void Debug::hex_print(char* data, int length)
{
for(int i = 0; i < length; i++)
{
printf("0x%02x,",(unsigned char)*(data+i));
}
printf("\n");
}

void Debug::ascii_print(char* data, int length)
{
     for(int i = 0; i <length; i++)
     {
             printf("%c ",(unsigned char)*(data+i));
     }
     printf("\n");
}

};

#endif
