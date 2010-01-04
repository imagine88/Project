#include "be_header.h"
#include<getopt.h>


typedef struct imagefile
{
  DWORD imSize;
  LONG imWidth;
  LONG imHeight;
  WORD imType;
  WORD imBitsPerPixel;
  DWORD imOffset;
  FILE *imData;
}image;
