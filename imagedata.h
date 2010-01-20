#include "header_struct.h"




typedef struct imagefile
{
  DWORD imSize;
  LONG imWidth;
  LONG imHeight;
  WORD imType;
  WORD imBitsPerPixel;
  DWORD imOffset;
  BYTE * imName;
}image;
