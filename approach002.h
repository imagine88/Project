#include "be_header.h"


struct image
{
  DWORD imSize;
  LONG imWidth;
  LONG imHeight;
  WORD imType;
  WORD imBitsPerPixel;
  DWORD imOffset;
  FILE *imData;
};
