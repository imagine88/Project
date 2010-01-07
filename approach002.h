#include "be_header.h"




typedef struct imagefile
{
  DWORD imSize;
  LONG imWidth;
  LONG imHeight;
  WORD imType;
  WORD imBitsPerPixel;
  DWORD imOffset;
  char* imRawData;
  char * imName;
}image;
