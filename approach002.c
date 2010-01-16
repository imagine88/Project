#include "approach002.h"


int process(image *,image *,char **);
int imagecopy(image *,image *,char **);
int median(image* ,image* ,char** );
RGBTRIPLE proc(RGBTRIPLE** ,int ,int ,int);
RGBTRIPLE mediansort(RGBTRIPLE** ,RGBTRIPLE* ,int );
int convgs(image*,image*,char**);
int averaging(image*,image*,char**);
RGBTRIPLE proc2(RGBTRIPLE** ,int ,int ,int );
int logtx(image*,image*,char**);
int gammatx(image*,image*,char**);
int media(image*,image*,char**);


int process(image* inImage,image* outImage,char** argv)
{
  int choice;
  int err;

  do
    {
      printf("\n1.Copy Image\n2.Convert To Grey Scale\n3.Log Transformation\n4.Gamma Correction\n5.Median Filter\n6.Averaging Filter\n7.Exit\n");
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
	case 6:
	  if((err=averaging(inImage,outImage,argv))==1)
	    return 1;
	  break;
	default:
	  if(choice==7)
	    return 7;
	  else
	    printf("Re-enter Choice\n");

	}
    }while(choice!=7);
 
 return 0;
}

int averaging(image* inImage,image* outImage,char** argv)
{
  BITMAPFILEHEADER bmfh;
  BITMAPINFOHEADER bmih;
  RGBTRIPLE pixel;
  int i,j,err,masksize;
  FILE* fp1;
  FILE* fp2;
  RGBTRIPLE** imRawData;

  fp1=fopen(argv[1],"rb");
  fp2=fopen(argv[2],"wb");

  printf("Enter mask size:");
  scanf("%d",&masksize);

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

  imRawData=(RGBTRIPLE **)malloc(((inImage->imHeight)+2) * sizeof(RGBTRIPLE *));
  
  
  
  for(i=0;i<=( (inImage->imHeight) +2);i++)
    {
      imRawData[i]=(RGBTRIPLE *)malloc(((inImage->imWidth)+2) * sizeof(RGBTRIPLE));
    }
 

  for(i=0;i<=( (inImage->imHeight) + 2 );i++)
    {
      for(j=0;j<=((inImage->imWidth)+2);j++)
	{    
	  if(((i==0 || i==((inImage->imHeight)+2))))
	    {
	      imRawData[i][j].rgbtRed=imRawData[i][j].rgbtGreen=imRawData[i][j].rgbtBlue=0;
	    }
	  else if(((j==0 || j==((inImage->imWidth)+2))))
	    {
	      imRawData[i][j].rgbtRed=imRawData[i][j].rgbtGreen=imRawData[i][j].rgbtBlue=0;
	    }
	  else
	    {
	      fread((BYTE *)&pixel,1,sizeof(RGBTRIPLE),fp1);
	      (imRawData)[i][j]=pixel;
	    }

	}
    }
  for(i=1;i<=(inImage->imHeight)+1;i++)
    {
      for(j=1;j<=(inImage->imWidth)+1;j++)
	{
	  pixel= proc2(imRawData,i,j,masksize);
	  fwrite((BYTE *)&pixel,1,sizeof(RGBTRIPLE),fp2);
	}
    }
  for(i=0;i<((inImage->imHeight) + 2);i++)
    {
      free((imRawData)[i]);
    }
  free((imRawData));
  fclose(fp1);
  fclose(fp2);
  return 0;
}



RGBTRIPLE proc2(RGBTRIPLE** imData,int rowNo,int colNo,int masksize)
{
 
  RGBTRIPLE** MASK;
  int x,y,count;
  RGBTRIPLE average;
  int Red=0;
  int Green=0;
  int Blue=0;
  
  MASK=(RGBTRIPLE **)malloc(masksize * sizeof(RGBTRIPLE *));

  for(x=0;x<=(masksize-1);x++)
    {
      MASK[x]=(RGBTRIPLE *)malloc(masksize * sizeof(RGBTRIPLE));
    }
  for(x = 0 ; x <= ( masksize -1 ) ; x++)
    {
      for(y = 0 ; y <= ( masksize - 1 ) ; y++)
	{
	  MASK[x][y]=imData[rowNo+(x-(int)(masksize/2))][colNo+(y-(int)(masksize/2))];
	  Red=Red + (int)MASK[x][y].rgbtRed;
	  Green=Green + (int)MASK[x][y].rgbtGreen;
	  Blue=Blue + (int)MASK[x][y].rgbtBlue;
	}
    }
  Red=Red / (pow(masksize,2));
  Green=Green / (pow(masksize,2));
  Blue=Blue / (pow(masksize,2)); 
  average.rgbtRed=(BYTE)(Red);
  average.rgbtGreen=(BYTE)(Green);
  average.rgbtBlue=(BYTE)(Blue);

  for(x=0;x<(masksize);x++)
    free(MASK[x]);

  free(MASK);
  return average;
}

int median(image* inImage,image* outImage,char** argv)
{
  BITMAPFILEHEADER bmfh;
  BITMAPINFOHEADER bmih;
  RGBTRIPLE pixel;
  int i,j,err,masksize;
  FILE* fp1;
  FILE* fp2;
  RGBTRIPLE** imRawData;

  fp1=fopen(argv[1],"rb");
  fp2=fopen(argv[2],"wb");

  printf("Enter mask size:");
  scanf("%d",&masksize);

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

  imRawData=(RGBTRIPLE **)malloc(((inImage->imHeight)+2) * sizeof(RGBTRIPLE *));
  
  
  
  for(i=0;i<=( (inImage->imHeight) +2);i++)
    {
      imRawData[i]=(RGBTRIPLE *)malloc(((inImage->imWidth)+2) * sizeof(RGBTRIPLE));
    }
 

  for(i=0;i<=( (inImage->imHeight) + 2 );i++)
    {
      for(j=0;j<=((inImage->imWidth)+2);j++)
	{    
	  if(((i==0 || i==((inImage->imHeight)+2))))
	    {
	      imRawData[i][j].rgbtRed=imRawData[i][j].rgbtGreen=imRawData[i][j].rgbtBlue=0;
	    }
	  else if(((j==0 || j==((inImage->imWidth)+2))))
	    {
	      imRawData[i][j].rgbtRed=imRawData[i][j].rgbtGreen=imRawData[i][j].rgbtBlue=0;
	    }
	  else
	    {
	      fread((BYTE *)&pixel,1,sizeof(RGBTRIPLE),fp1);
	      (imRawData)[i][j]=pixel;
	    }

	}
    }
  for(i=1;i<=(inImage->imHeight)+1;i++)
    {
      for(j=1;j<=(inImage->imWidth)+1;j++)
	{
	  pixel= proc(imRawData,i,j,masksize);
	  fwrite((BYTE *)&pixel,1,sizeof(RGBTRIPLE),fp2);
	}
    }
  for(i=0;i<((inImage->imHeight) + 2);i++)
    {
      free((imRawData)[i]);
    }
  free((imRawData));
  fclose(fp1);
  fclose(fp2);
  return 0;
}

RGBTRIPLE proc(RGBTRIPLE** imData,int rowNo,int colNo,int masksize)
{
 
  RGBTRIPLE** MASK;
  RGBTRIPLE* sort;
  RGBTRIPLE pixel;
  int x,y,count;


  
  MASK=(RGBTRIPLE **)malloc(masksize * sizeof(RGBTRIPLE *));

  sort=(RGBTRIPLE *)malloc( (masksize*masksize) * sizeof(RGBTRIPLE));

  for(x=0;x<=(masksize-1);x++)
    {
      MASK[x]=(RGBTRIPLE *)malloc(masksize * sizeof(RGBTRIPLE));
    }
  for(x = 0 ; x <= ( masksize -1 ) ; x++)
    {
      for(y = 0 ; y <= ( masksize - 1 ) ; y++)
	{
	  MASK[x][y]=imData[rowNo+(x-(int)(masksize/2))][colNo+(y-(int)(masksize/2))];
	}
    }
  pixel=mediansort(MASK,sort,masksize);
  for(x=0;x<(masksize);x++)
    free(MASK[x]);

  free(MASK);
  free(sort);
  return pixel;
}

RGBTRIPLE mediansort(RGBTRIPLE** MASK,RGBTRIPLE* sort,int masksize)
{
  RGBTRIPLE medianvalue;
  RGBTRIPLE temp;
  int x,y,count;
  static int flag=0;
  count =0;
  flag++;
  
  for(x=0;x<(masksize);x++)
    {
      for(y=0;y<(masksize);y++)
	{
	  sort[count++]=MASK[x][y];
	}
    }
  if(flag==100)
    {
      printf("Unsoted");
      for(x=0;x<9;x++)
	{
	  printf("%x\t",sort[x].rgbtGreen);
	}
    }
  for(x=0;x<(masksize*masksize);x++)
    {
      for(y=((masksize * masksize)-1);y>x;y--)
	{
	  if(sort[y-1].rgbtRed>sort[y].rgbtRed)
	    {
	      temp.rgbtRed=sort[y-1].rgbtRed;
	      sort[y-1].rgbtRed=sort[y].rgbtRed;
	      sort[y].rgbtRed=temp.rgbtRed;
	    }
	  if(sort[y-1].rgbtGreen>sort[y].rgbtGreen)
	    {
	      temp.rgbtGreen=sort[y-1].rgbtGreen;
	      sort[y-1].rgbtGreen=sort[y].rgbtGreen;
	      sort[y].rgbtGreen=temp.rgbtGreen;
	    }
	  if(sort[y-1].rgbtBlue>sort[y].rgbtBlue)
	    {
	      temp.rgbtBlue=sort[y-1].rgbtBlue;
	      sort[y-1].rgbtBlue=sort[y].rgbtBlue;
	      sort[y].rgbtBlue=temp.rgbtBlue;
	    }
	}
    }
  if(flag==100)
    {
      printf("Sorted");
      for(x=0;x<9;x++)
	{
	  printf("%x\t",sort[x].rgbtGreen);
	}
    }
  medianvalue=sort[(int)((masksize*masksize)/2)];
  if(flag==100)
    printf("\n%x\n",medianvalue.rgbtGreen);
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
	  pixel.rgbtBlue=pixel.rgbtRed=pixel.rgbtGreen=(BYTE)(1/(log(1+(int)(((0.30 * (int)pixel.rgbtRed) + (0.59 * (int)pixel.rgbtGreen) + (0.11 * (int)pixel.rgbtBlue))/3))));
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
  

  
  if(argc!=3)
    {
      printf("Insufficient arguments\nUsage:./progname infile\n");
      exit(1);
    }

 
  
  if((err=process(&inImage,&outImage,argv))!=0);
  {
    if(err!=7)
      printf("\nUnexpected Result\n");
    exit(1);
  }
  
  return 0;
}





      

