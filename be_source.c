#include "be_header.h"





#define INFILE "o4.bmp"
#define OUTFILE "p.bmp"
#define OUTFILE2 "q.bmp"

int main()
{
  FILE *fp1;
  FILE *fp2;
  FILE *fp3;
  BITMAPFILEHEADER bmfh;
  BITMAPINFOHEADER bmih;
  RGBTRIPLE *pixel=(RGBTRIPLE *)malloc(sizeof(RGBTRIPLE));
  int i=0,j=0;
  BYTE buffer1[3],buffer2; 
  RGBTRIPLE rgb[3][3];
  int errcode,pos,read;
  char buffer[10];
  

  printf("%d\n",sizeof(RGBTRIPLE));
  fp1=fopen(INFILE,"rb");
  fp2=fopen(OUTFILE,"wb");
  fp3=fopen(OUTFILE2,"wb");
  
  fseek(fp1,0,SEEK_SET);
  
 

  fread((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp1);

  fread((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp1);

  fwrite((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp2);

  fwrite((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp2);

  fwrite((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp3);

  fwrite((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp3);

  pos=ftell(fp1);

  printf("\n%x\n",bmfh.bfType);

  printf("\n%lu\n",bmfh.bfSize);
  printf("\n%u\n",bmfh.bfReserved1);
  printf("\n%u\n",bmfh.bfReserved2);
  printf("\n%lu\n",bmfh.bfOffsetBits);
  printf("------");
  printf("\n%lu\n",bmih.biSize);
  printf("\n%ld\n",bmih.biWidth);
  printf("\n%ld\n",bmih.biHeight);
  printf("\n%u\n",bmih.biPlanes);
  printf("\n%u\n",bmih.biBitCount);
  printf("\n%lu\n",bmih.biCompression);
  printf("\n%lu\n",bmih.biSizeImage);
  printf("\n%ld\n",bmih.biXPelsPerMeter);
  printf("\n%ld\n",bmih.biYPelsPerMeter);
  printf("\n%lu\n",bmih.biClrUsed);
  printf("\n%lu\n",bmih.biClrImportant);
  printf("%d\n",pos);
  fseek(fp1,bmfh.bfOffsetBits,SEEK_SET);


  for( j=0; j<bmih.biHeight; j++ )
    {
     
      read = 0;
      for( i=0; i<bmih.biWidth; i++ ) 
	{
	  
	  fread(buffer1, 1, (sizeof(RGBTRIPLE)),fp1);
	  buffer1[0]=(BYTE) ( ( ( pow( ( (float)buffer1[0]),2))  ) );
	  buffer1[1]=(BYTE) ( ( ( pow( ( (float)buffer1[1]),2))  ) );
	  buffer1[2]=(BYTE) ( ( ( pow( ( (float)buffer1[2]),2))  ) );
	  fwrite(buffer1,1,(sizeof(RGBTRIPLE)),fp2);
	  read += sizeof(RGBTRIPLE);
	    

	}
      
    }    
  printf("%f",log10(100));
  printf("\t%d\t",sizeof(LONG));

	    
      

  


  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  

  return 0;

}
