#include "approach002.h"

int fileopen(image *,image *,char **);
int process(image *,image *,char **);
int imagecopy(image *,image *,char **);
/*int convgs(image*,image*);
int logtx(image*,image*);
int gammatx(image*,image*);*/




int fileopen(image* inImage,image* outImage,char **argv)
{
  /*if((inImage->imData=fopen(*argv+1,"rb"))==NULL)
    {
      return 1;
    }
  if((outImage->imData=fopen(*argv+2,"wb"))==NULL)
    {
      return 1;
      }*/
  printf("%c",(*argv[optind++]));
  return 0;
}

int process(image* inImage,image* outImage,char** argv)
{
  int choice;
  int err;

  do
    {
      printf("\n1.Copy Image\n2.Convert To Grey Scale\n3.Log Transformation\n4.Gamma Correction\n5.Exit\n");
      scanf("%d",&choice);
      switch(choice)
	{
	  case 1:
	    if((err=imagecopy(inImage,outImage,argv))==1)
	    {
	      return 1;
	    }
	  break;
	  /*case 2:
	  if((err=convgs(inImage,outImage))==1)
	    {
	      return 1;
	    }
	  break;
	case 3:
	  if((err=logtx(inImage,outImage))==1)
	    {
	      return 1;
	    }
	  break;
	case 4:
	  if((err=gammatx(inImage,outImage))==1)
	    {
	      return 1;
	    }
	    break;*/
	default:
	  printf("Re-enter Choice\n");
	}
    }while(choice!=5);
 
 return 0;
}

int imagecopy(image* inImage,image* outImage,char** argv)
{
  PBITMAPFILEHEADER bmfh;
  PBITMAPINFOHEADER bmih;
  PRGBTRIPLE pPixel;
  int i,j,err;

  if((err=fileopen(inImage,outImage,argv))!=0);
  {
    printf("\nFile: %s could not be opened\n",*argv+1);
    return 1;
  }

  fread(bmfh,sizeof(BITMAPFILEHEADER),1,inImage->imData);
  inImage->imType=outImage->imType=bmfh->bfSize;
  inImage->imOffset=outImage->imOffset=bmfh->bfOffsetBits;

  fread(bmih,sizeof(BITMAPINFOHEADER),1,inImage->imData);
  inImage->imSize=outImage->imSize=bmih->biSize;
  inImage->imWidth=outImage->imWidth=bmih->biWidth;
  inImage->imHeight=outImage->imHeight=bmih->biHeight;
  inImage->imBitsPerPixel=outImage->imBitsPerPixel=bmih->biBitCount;
  
  fwrite(bmfh,sizeof(BITMAPFILEHEADER),1,outImage->imData);
  fwrite(bmih,sizeof(BITMAPINFOHEADER),1,outImage->imData);

  for(i=0;i<inImage->imHeight;i++)
    {
      for(j=0;j<inImage->imWidth;j++)
	{
	  fread(pPixel,1,sizeof(RGBTRIPLE),inImage->imData);
	  fwrite(pPixel,1,sizeof(RGBTRIPLE),outImage->imData);
	}
    }
  return 0;
}
               


int main(int argc,char **argv)
{
  image* inImage;
  image* outImage;
  int err;

  if(argc==1 || argc<2)
    {
      printf("Insufficient arguments\nUsage:./progname infile outfile");
      exit(1);
    }

 

  if((err=process(inImage,outImage,argv))!=0);
  {
    printf("\nUnexpected Result\n");
    exit(1);
  }

  return 0;
}





      

