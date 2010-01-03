#include "be_header.h"

#define INFILE "o.bmp"
#define OUTFILE "p.bmp"

int main()
{
  FILE *fp1;
  FILE *fp2;
  PBITMAPFILEHEADER pbmfh;
  PBITMAPINFOHEADER pbmih;
  int errcode;


  fp1=fopen(INFILE,"rb");
  fp2=fopen(OUTFILE,"wb");


  errcode=fread(pbmfh->bfType,sizeof(pbmfh->bfType),1,fp1);
  errcode=fread(pbmfh->bfSize,sizeof(pbmfh->bfSize),1,fp1);
  errcode=fread(pbmfh->bfReserved1,sizeof(pbmfh->bfReserved1),1,fp1);
  errcode=fread(pbmfh->bfReserved2,sizeof(pbmfh->bfReserved2),1,fp1);
  errcode=fread(pbmfh->bfOffsetBits,sizeof(pbmfh->bfOffsetBits),1,fp1);

  printf("%d",pbmfh->bfType);


  fclose(fp1);
  fclose(fp2);
  

  return 0;

}
