#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MASKSIZE 3

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef long LONG;
typedef unsigned char BYTE;

#pragma pack(push)
#pragma pack(1)

typedef struct tagBITMAPFILEHEADER
{
 
  WORD bfType;
  
  DWORD bfSize;
  
  WORD bfReserved1;

  WORD bfReserved2;

  DWORD bfOffsetBits;

}BITMAPFILEHEADER,*PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
  DWORD biSize;
  LONG biWidth;
  LONG biHeight;
  WORD biPlanes;
  WORD biBitCount;
  DWORD biCompression;
  DWORD biSizeImage;
  LONG biXPelsPerMeter;
  LONG biYPelsPerMeter;
  DWORD biClrUsed;
  DWORD biClrImportant;
}
  BITMAPINFOHEADER,*PBITMAPINFOHEADER;


typedef struct tagRGBTRIPLE
{
  BYTE rgbtBlue ;
  BYTE rgbtGreen ;
  BYTE rgbtRed ;
}
  RGBTRIPLE,*PRGBTRIPLE;

typedef struct tagRGBQUAD
{
  BYTE rgbBlue;
  BYTE rgbGreen;
  BYTE rgbRed;
  BYTE rgbReserved;
}
  RGBQUAD;

#pragma pack(pop)
