#include "approach002.h"


int process(image *,image *,char **);
int imagecopy(image *,image *,char **);
int median(image* ,image* ,char** );
BYTE proc(BYTE** ,int ,int );
BYTE mediansort(BYTE** ,char* ,int );
int convgs(image*,image*,char**);

int logtx(image*,image*,char**);
int gammatx(image*,image*,char**);
int media(image*,image*,char**);


int process(image* inImage,image* outImage,char** argv)
{
  int choice;
  int err;

  do
    {
      printf("\n1.Copy Image\n2.Convert To Grey Scale\n3.Log Transformation\n4.Gamma Correction\n5.Median Filter\n6.Exit\n");
      scanf("%d",&choice);
      switch(choice)
	{
	  case 1:
	    if((err=imagecopy(inImage,outImage,argv))==1)
	    {
	      return 1;
	    }
	  break;
	  case 2:
	    if((err=convgs(inImage,outImage,argv))==1)
	    {
	      return 1;
	    }
	  break;
	  case 3:
	  if((err=logtx(inImage,outImage,argv))==1)
	    {
	      return 1;
	    }
	  break;
	  case 4:
	    if((err=gammatx(inImage,outImage,argv))==1)
	    {
	      return 1;
	    }
	  break;
	case 5:
	  if((err=median(inImage,outImage,argv))==1)
	    return 1;
	  break;
	default:
	  if(choice==6)
	    return 6;
	  else
	    printf("Re-enter Choice\n");

	}
    }while(choice!=5);
 
 return 0;
}

int median(image* inImage,image* outImage,char** argv)
{
  BITMAPFILEHEADER bmfh;
  BITMAPINFOHEADER bmih;
  RGBTRIPLE pixel;
  int i,j,err;
  FILE* fp1;
  FILE* fp2;
  BYTE** imRedData;
  BYTE** imGreenData;
  BYTE** imBlueData;

  fp1=fopen(argv[1],"rb");
  fp2=fopen(argv[2],"wb");

  

  inImage->imName=argv[1];
  outImage->imName=argv[2];
  
  fread((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp1);
  inImage->imType=outImage->imType=bmfh.bfSize;
  inImage->imOffset=outImage->imOffset=bmfh.bfOffsetBits;
  printf("%x\n",bmfh.bfType);

  fread((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp1);
  inImage->imSize=bmfh.bfSize;
  inImage->imWidth=outImage->imWidth=bmih.biWidth;
  inImage->imHeight=outImage->imHeight=bmih.biHeight;
  inImage->imBitsPerPixel=outImage->imBitsPerPixel=bmih.biBitCount;
  outImage->imSize=inImage->imWidth * inImage->imHeight ;
  

  printf("%ld\n",bmih.biWidth);

  fwrite((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp2);
  fwrite((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp2);

  fseek(fp1,0,SEEK_SET+(inImage->imOffset));
  fseek(fp2,0,SEEK_SET+(outImage->imOffset));

  imRedData=imGreenData=imBlueData=(BYTE **)calloc((inImage->imHeight) + 2,sizeof(BYTE));

  for(i=-1;i<(inImage->imHeight +1);i++)
    {
      imRedData[i]=imGreenData[i]=imBlueData[i]=(BYTE *)calloc((inImage->imWidth) +2,sizeof(BYTE));
    }


  for(i=-1;i<=inImage->imHeight+1;i++)
    {
      for(j=-1;j<=inImage->imWidth+1;j++)
	{    
	  if((i==-1 || i==inImage->imHeight+1 || j==-1 || j==inImage->imWidth+1))
	    {
	      (imRedData)[i][j]=0x00;
	      (imGreenData)[i][j]=0x00;
	      (imBlueData)[i][j]=0x00;
	    }
	  else
	    {
	      fread((BYTE *)&pixel,1,sizeof(RGBTRIPLE),fp1);
	      (imRedData)[i][j]=pixel.rgbtRed;
	      (imGreenData)[i][j]=pixel.rgbtGreen;
	      (imBlueData)[i][j]=pixel.rgbtBlue;
	    }

	}
    }
  for(i=0;i<=inImage->imHeight;i++)
    {
      for(j=0;j<inImage->imWidth;j++)
	{
	  pixel.rgbtRed= proc(imRedData,i,j);
	  pixel.rgbtGreen=proc(imGreenData,i,j);
	  pixel.rgbtBlue=proc(imBlueData,i,j);
	  fwrite((BYTE *)&pixel,1,sizeof(RGBTRIPLE),fp2);
	}
    }
  for(i=0;i<((inImage->imHeight) + 2);i++)
    {
      free((imRedData)[i]);
      free((imGreenData)[i]);
      free((imBlueData)[i]);
    }
  free((imRedData));
  free((imGreenData));
  free((imBlueData));
  fclose(fp1);
  fclose(fp2);
  return 0;
}

BYTE proc(BYTE** imData,int rowNo,int colNo)
{
  int masksize;
  BYTE** MASK;
  BYTE* sort;
  BYTE pixel;
  int x,y,count;

  printf("Enter mask size:");
  scanf("%d",&masksize);
  
  MASK=(BYTE **)calloc(masksize,sizeof(BYTE));

  sort=(BYTE *)calloc(masksize*masksize,sizeof(BYTE));

  for(x=(int)(-(masksize/2));x<=(masksize/2);x++)
    {
      MASK[x]=(BYTE *)calloc(masksize,sizeof(BYTE));
    }
  for(x=(int)(-(masksize/2));x<=(masksize/2);x++)
    {
      for(y=(int)(-(masksize/2));y<=(masksize/2);y++)
	{
	  MASK[x][y]=imData[rowNo+x][colNo+y];
	}
    }
  pixel=mediansort(MASK,sort,masksize);
  for(x=0;x<(masksize);x++)
    free(MASK[x]);

  free(MASK);
  free(sort);
  return pixel;
}

BYTE mediansort(BYTE** MASK,char* sort,int masksize)
{
  BYTE medianvalue;
  BYTE temp;
  int x,y,count;

  for(x=(int)(-(masksize/2));x<=(masksize/2);x++)
    {
      for(y=(int)(-(masksize/2));y<=(masksize/2);y++)
	{
	  sort[count++]=MASK[x][y];
	}
    }
  for(x=0;x<(masksize*masksize);x++)
    {
      for(y=1;y<=((masksize*masksize)-1);y++)
	{
	  if(sort[y]<=sort[x])
	    {
	      sort[y]=temp;
	      sort[y]=sort[x];
	      sort[x]=temp;
	    }
	}
    }
  medianvalue=sort[(masksize*masksize)/2];
  return medianvalue;
}

  
int convgs(image* inImage,image* outImage,char** argv)
{
  BITMAPFILEHEADER bmfh;
  BITMAPINFOHEADER bmih;
  RGBTRIPLE pixel;
  int i,j,err;
  FILE* fp1;
  FILE* fp2;
  fp1=fopen(argv[1],"rb");
  fp2=fopen(argv[2],"wb");
  

  inImage->imName=argv[1];
  outImage->imName=argv[2];
  
  fread((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp1);
  inImage->imType=outImage->imType=bmfh.bfSize;
  inImage->imOffset=outImage->imOffset=bmfh.bfOffsetBits;
  printf("%x\n",bmfh.bfType);

  fread((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp1);
  inImage->imSize=bmfh.bfSize;
  inImage->imWidth=outImage->imWidth=bmih.biWidth;
  inImage->imHeight=outImage->imHeight=bmih.biHeight;
  inImage->imBitsPerPixel=outImage->imBitsPerPixel=bmih.biBitCount;
  outImage->imSize=inImage->imWidth * inImage->imHeight ;
  

  printf("%ld\n",bmih.biWidth);

  fwrite((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp2);
  fwrite((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp2);

  fseek(fp1,0,SEEK_SET+(inImage->imOffset));
  fseek(fp2,0,SEEK_SET+(outImage->imOffset));
  for(i=0;i<inImage->imHeight;i++)
    {
      for(j=0;j<inImage->imWidth;j++)
	{
	  fread((BYTE *)&pixel,1,sizeof(RGBTRIPLE),fp1);
	  pixel.rgbtBlue=pixel.rgbtRed=pixel.rgbtGreen=((0.30 * pixel.rgbtRed) + (0.59 * pixel.rgbtGreen) + (0.11 * pixel.rgbtBlue))/3;
	  fwrite((BYTE *)&pixel,1,sizeof(RGBTRIPLE),fp2);
	}
    }
  fclose(fp1);
  fclose(fp2);
  return 0;
}

	  

  
int imagecopy(image* inImage,image* outImage,char** argv)
{
  BITMAPFILEHEADER bmfh;
  BITMAPINFOHEADER bmih;
  RGBTRIPLE pixel;
  int i,j,err;
  FILE* fp1;
  FILE* fp2;
  fp1=fopen(argv[1],"rb");
  fp2=fopen(argv[2],"wb");
  

  inImage->imName=argv[1];
  outImage->imName=argv[2];
  
  fread((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp1);
  inImage->imType=outImage->imType=bmfh.bfSize;
  inImage->imOffset=outImage->imOffset=bmfh.bfOffsetBits;
  printf("%x\n",bmfh.bfType);

  fread((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp1);
  inImage->imSize=outImage->imSize=bmih.biSize;
  inImage->imWidth=outImage->imWidth=bmih.biWidth;
  inImage->imHeight=outImage->imHeight=bmih.biHeight;
  inImage->imBitsPerPixel=outImage->imBitsPerPixel=bmih.biBitCount;
  printf("%ld\n",bmih.biWidth);

  fwrite((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp2);
  fwrite((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp2);

  fseek(fp1,0,SEEK_SET+(inImage->imOffset));
  fseek(fp2,0,SEEK_SET+(outImage->imOffset));


  for(i=0;i<inImage->imHeight;i++)
    {
      
      for(j=0;j<inImage->imWidth;j++)
	{
	  fread((BYTE *)&pixel,1,(sizeof(RGBTRIPLE)),fp1);
	  fwrite((BYTE *)&pixel,1,(sizeof(RGBTRIPLE)),fp2);
	}
    }
  
  fclose(fp1);
  fclose(fp2);

  return 0;
}


int logtx(image* inImage,image* outImage,char** argv)
{
  BITMAPFILEHEADER bmfh;
  BITMAPINFOHEADER bmih;
  RGBTRIPLE pixel;
  int i,j,err;
  FILE* fp1;
  FILE* fp2;
  fp1=fopen(argv[1],"rb");
  fp2=fopen(argv[2],"wb");
  

  inImage->imName=argv[1];
  outImage->imName=argv[2];
  
  fread((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp1);
  inImage->imType=outImage->imType=bmfh.bfSize;
  inImage->imOffset=outImage->imOffset=bmfh.bfOffsetBits;
  printf("%x\n",bmfh.bfType);

  fread((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp1);
  inImage->imSize=outImage->imSize=bmih.biSize;
  inImage->imWidth=outImage->imWidth=bmih.biWidth;
  inImage->imHeight=outImage->imHeight=bmih.biHeight;
  inImage->imBitsPerPixel=outImage->imBitsPerPixel=bmih.biBitCount;
  printf("%ld\n",bmih.biWidth);

  fwrite((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp2);
  fwrite((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp2);

  fseek(fp1,0,SEEK_SET+(inImage->imOffset));
  fseek(fp2,0,SEEK_SET+(outImage->imOffset));


  for(i=0;i<inImage->imHeight;i++)
    {
      
      for(j=0;j<inImage->imWidth;j++)
	{
	  fread((BYTE *)&pixel,1,(sizeof(RGBTRIPLE)),fp1);
	  pixel.rgbtBlue=pixel.rgbtRed=pixel.rgbtGreen=1/(log(1+(((0.30 * pixel.rgbtRed) + (0.59 * pixel.rgbtGreen) + (0.11 * pixel.rgbtBlue))/3)));
	  if(pixel.rgbtBlue < 0x00)
	    pixel.rgbtBlue=pixel.rgbtGreen=pixel.rgbtRed=0x00;
	  if(pixel.rgbtBlue>0xFF)
	    pixel.rgbtBlue=pixel.rgbtRed=pixel.rgbtRed=0xFF;

	  fwrite((BYTE *)&pixel,1,(sizeof(RGBTRIPLE)),fp2);
	}
    }
  
  fclose(fp1);
  fclose(fp2);

  return 0;
}              

int gammatx(image* inImage,image* outImage,char** argv)
{
  BITMAPFILEHEADER bmfh;
  BITMAPINFOHEADER bmih;
  RGBTRIPLE pixel;
  int i,j,err;
  FILE* fp1;
  FILE* fp2;
  float power;

  fp1=fopen(argv[1],"rb");
  fp2=fopen(argv[2],"wb");
  

  inImage->imName=argv[1];
  outImage->imName=argv[2];
  
  fread((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp1);
  inImage->imType=outImage->imType=bmfh.bfSize;
  inImage->imOffset=outImage->imOffset=bmfh.bfOffsetBits;
  printf("%x\n",bmfh.bfType);

  fread((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp1);
  inImage->imSize=outImage->imSize=bmih.biSize;
  inImage->imWidth=outImage->imWidth=bmih.biWidth;
  inImage->imHeight=outImage->imHeight=bmih.biHeight;
  inImage->imBitsPerPixel=outImage->imBitsPerPixel=bmih.biBitCount;
  printf("%ld\n",bmih.biWidth);

  fwrite((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp2);
  fwrite((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp2);

  fseek(fp1,0,SEEK_SET+(inImage->imOffset));
  fseek(fp2,0,SEEK_SET+(outImage->imOffset));

  printf("Enter the exponent:");
  scanf("%f",&power);
  for(i=0;i<inImage->imHeight;i++)
    {
      
      for(j=0;j<inImage->imWidth;j++)
	{
	  fread((BYTE *)&pixel,1,(sizeof(RGBTRIPLE)),fp1);
	  pixel.rgbtBlue=pow(pixel.rgbtBlue,power);
	  pixel.rgbtRed=pow(pixel.rgbtRed,power);
	  pixel.rgbtGreen=pow(pixel.rgbtGreen,power);
	  fwrite((BYTE *)&pixel,1,(sizeof(RGBTRIPLE)),fp2);
	}
    }
  
  fclose(fp1);
  fclose(fp2);

  return 0;
}  


int main(int argc,char **argv)
{
  image inImage;
  image outImage;
  int err;
  FILE* fp1;
  FILE* fp2;

  
  if(argc!=3)
    {
      printf("Insufficient arguments\nUsage:./progname infile\n");
      exit(1);
    }

 
  
  if((err=process(&inImage,&outImage,argv))!=0);
  {
    if(err!=5)
      printf("\nUnexpected Result\n");
    exit(1);
  }
  
  return 0;
}





      

