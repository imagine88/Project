#include "imagedata.h"
#define SUCCESS 0
#define EXIT 5
#define masksize 3


/*********************************************
 *         Function Prototypes               *
 *                                           *
 *                                           *
 *********************************************/

/*--------------------------------------------------------*/

int process(image *,image *,char **);                      
int readpixel(RGBTRIPLE** ,image* ,FILE* );
int copyheader(image* ,image* ,FILE* ,FILE* );
int median(image* ,image* ,char** );
RGBTRIPLE medianprocess(RGBTRIPLE** ,int ,int );
RGBTRIPLE mediansort(RGBTRIPLE** ,RGBTRIPLE* );
int averaging(image*,image*,char**);
RGBTRIPLE averagingprocess(RGBTRIPLE** ,int ,int );
int sharpening(image*,image*,char**);
RGBTRIPLE sharpeningprocess(RGBTRIPLE** ,int ,int );
int convgs(image*,image*,char**);

/*--------------------------------------------------------*/



/*********************************************
 *                   Menu                    *
 *                                           *
 *                                           *
 *********************************************/

/*--------------------------------------------------------*/

int process(image* inImage,image* outImage,char** argv)
{
  int choice;
  int err;

  do
    {
      printf("\n1.Convert To Grey Scale\n2.Median Filter\n3.Averaging Filter\n4.Sharpening\n5.Exit\n");
      printf("Choice:");
      scanf("%d",&choice);
      switch(choice)
	{
	  
	case 1:
	    if((err=convgs(inImage,outImage,argv))!=SUCCESS)
	      {
		printf("Error Encountered\n");
		exit(1);
	      }
	    break;
	case 2:
	  if((err=median(inImage,outImage,argv))!=SUCCESS)
	    {
	      printf("Error Encountered\n");	      
	      exit(1);
	    }
	  break;
	case 3:
	  if((err=averaging(inImage,outImage,argv))!=SUCCESS)
	    {
	      printf("Error Encountered\n");
	      exit(1);
	    }
	  break;
	case 4:
	  if((err=sharpening(inImage,outImage,argv))!=SUCCESS)
	    {
	      printf("Error Encountered\n");
	      exit(1);
	    }
	  break;
	default:
	  if(choice==EXIT)
	    return EXIT;
	  else
	    printf("Re-enter Choice\n");
	}
    }while(choice!=EXIT);
 
 return SUCCESS;
}

/*--------------------------------------------------------*/




/*********************************************
 *         Copy Header Information           *
 *                                           *
 *                                           *
 *********************************************/

/*--------------------------------------------------------*/

int copyheader(image* inImage,image* outImage,FILE* fp1,FILE* fp2)
{
  BITMAPFILEHEADER bmfh;
  BITMAPINFOHEADER bmih;

  fread((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp1);
  inImage->imType=outImage->imType=bmfh.bfSize;
  inImage->imOffset=outImage->imOffset=bmfh.bfOffsetBits;
 
  fread((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp1);
  inImage->imSize=bmfh.bfSize;
  inImage->imWidth=outImage->imWidth=bmih.biWidth;
  inImage->imHeight=outImage->imHeight=bmih.biHeight;
  inImage->imBitsPerPixel=outImage->imBitsPerPixel=bmih.biBitCount;
  outImage->imSize=inImage->imWidth * inImage->imHeight ;
  
  fwrite((BYTE *)&bmfh,sizeof(BITMAPFILEHEADER),1,fp2);
  fwrite((BYTE *)&bmih,sizeof(BITMAPINFOHEADER),1,fp2);

  return SUCCESS;
}

/*--------------------------------------------------------*/



/*********************************************
 *         Read Pixel Data                   *
 *         Into Image Array(0-Padded)        *
 *                                           *
 *********************************************/

/*--------------------------------------------------------*/

int readpixel(RGBTRIPLE** imRawData,image* inImage,FILE* fp1)
{
  int i,j;
  RGBTRIPLE pixel;

  for(i=0;i<( (inImage->imHeight) + (masksize -1));i++)
    {
      for(j=0;j<((inImage->imWidth)+(masksize -1));j++)
	{    
	  if(((i<((int)(masksize/2)) || i>=((inImage->imHeight)+((int)(masksize/2))))))
	    {
	      imRawData[i][j].rgbtRed=imRawData[i][j].rgbtGreen=imRawData[i][j].rgbtBlue=0;
	    }
	  else if(((j<((int)(masksize/2)) || j>=((inImage->imWidth)+((int)(masksize/2))))))
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

  return SUCCESS;
}

/*--------------------------------------------------------*/



/*********************************************
 *         Function:Averaging Filter         *
 *             (SPATIAL - With Mask)         *
 *                                           *
 *********************************************/

/*--------------------------------------------------------*/

int averaging(image* inImage,image* outImage,char** argv)
{
  RGBTRIPLE pixel;
  int i,j,err;
  FILE* fp1;
  FILE* fp2;
  RGBTRIPLE** imRawData;

  fp1=fopen(argv[1],"rb");
  fp2=fopen(argv[2],"wb");

 
  inImage->imName=(BYTE *)argv[1];
  outImage->imName=(BYTE *)argv[2];
  
  if((err=copyheader(inImage,outImage,fp1,fp2)!=0))
    {
      printf("Error Copying Header\n");
      exit(1);
    }

  fseek(fp1,0,SEEK_SET+(inImage->imOffset));
  fseek(fp2,0,SEEK_SET+(outImage->imOffset));

  imRawData=(RGBTRIPLE **)malloc(((inImage->imHeight)+(masksize - 1)) * sizeof(RGBTRIPLE *));
  
  for(i=0;i<( (inImage->imHeight) +(masksize - 1));i++)
    {
      imRawData[i]=(RGBTRIPLE *)malloc(((inImage->imWidth)+(masksize - 1)) * sizeof(RGBTRIPLE));
    }
  
  if((err=readpixel(imRawData,inImage,fp1))!=0)
    {
      printf("Error Reading Pixel Data\n");
      exit(1);
    }

  
  for(i=((int)(masksize/2));i<((inImage->imHeight)+((int)(masksize/2)));i++)
    {
      for(j=((int)(masksize/2));j<((inImage->imWidth)+((int)(masksize/2)));j++)
	{
	  pixel= averagingprocess(imRawData,i,j);
	  fwrite((BYTE *)&pixel,1,sizeof(RGBTRIPLE),fp2);
	}
    }

  for(i=0;i<((inImage->imHeight) + (masksize - 1));i++)
    {
      free((imRawData)[i]);
    }

  free((imRawData));
  fclose(fp1);
  fclose(fp2);

  return SUCCESS;
}

RGBTRIPLE averagingprocess(RGBTRIPLE** imData,int rowNo,int colNo)
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
	  Red=Red + MASK[x][y].rgbtRed;
	  Green=Green + MASK[x][y].rgbtGreen;
	  Blue=Blue + MASK[x][y].rgbtBlue;
	}
    }
 
  Red=Red / (pow(masksize,2));
  Green=Green / (pow(masksize,2));
  Blue=Blue / (pow(masksize,2));

  if(Red>=255)
    Red=255;
 
  if(Green>=255)
    Green=255;

  if(Blue>=255)
    Blue=255;

  average.rgbtRed=(BYTE)(Red);
  average.rgbtGreen=(BYTE)(Green);
  average.rgbtBlue=(BYTE)(Blue);

  for(x=0;x<(masksize);x++)
    free(MASK[x]);

  free(MASK);
  return average;
}

/*--------------------------------------------------------*/


/*********************************************
 *         Function:Median Filter            *
 *         SPATIAL(Non-Liner)                *
 *                                           *
 *********************************************/

/*--------------------------------------------------------*/

int median(image* inImage,image* outImage,char** argv)
{
 
  RGBTRIPLE pixel;
  int i,j,err;
  FILE* fp1;
  FILE* fp2;
  RGBTRIPLE** imRawData;

  fp1=fopen(argv[1],"rb");
  fp2=fopen(argv[2],"wb");

  if((err=copyheader(inImage,outImage,fp1,fp2)!=0))
    {
      printf("Error Copying Header\n");
      exit(1);
    }

  inImage->imName=(BYTE *)argv[1];
  outImage->imName=(BYTE *)argv[2];

  fseek(fp1,0,SEEK_SET+(inImage->imOffset));
  fseek(fp2,0,SEEK_SET+(outImage->imOffset));

  imRawData=(RGBTRIPLE **)malloc(((inImage->imHeight)+(masksize - 1)) * sizeof(RGBTRIPLE *));
  
  for(i=0;i<( (inImage->imHeight) +(masksize - 1));i++)
    {
      imRawData[i]=(RGBTRIPLE *)malloc(((inImage->imWidth)+(masksize - 1)) * sizeof(RGBTRIPLE));
    }
 
  if((err=readpixel(imRawData,inImage,fp1))!=0)
    {
      printf("Error Reading Pixel Data\n");
      exit(1);
    }

  for(i=((int)(masksize/2));i<((inImage->imHeight)+((int)(masksize/2)));i++)
    {
      for(j=((int)(masksize/2));j<((inImage->imWidth)+((int)(masksize/2)));j++)
	{
	  pixel= medianprocess(imRawData,i,j);
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
  return SUCCESS;
}

RGBTRIPLE medianprocess(RGBTRIPLE** imData,int rowNo,int colNo)
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
  pixel=mediansort(MASK,sort);
  for(x=0;x<(masksize);x++)
    free(MASK[x]);

  free(MASK);
  free(sort);

  return pixel;
}

RGBTRIPLE mediansort(RGBTRIPLE** MASK,RGBTRIPLE* sort)
{
  RGBTRIPLE medianvalue;
  RGBTRIPLE temp;
  int x,y,count;
  count =0;
  
  
  for(x=0;x<(masksize);x++)
    {
      for(y=0;y<(masksize);y++)
	{
	  sort[count++]=MASK[x][y];
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
  
  medianvalue=sort[(int)((masksize*masksize)/2)];
  
  return medianvalue;
}

/*--------------------------------------------------------*/


/*********************************************
 *         Convert Image                     *
 *         To Greyscale                      *
 *                                           *
 *********************************************/  

/*--------------------------------------------------------*/

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
  

  inImage->imName=(BYTE *)argv[1];
  outImage->imName=(BYTE *)argv[2];
  
  if((err=copyheader(inImage,outImage,fp1,fp2)!=0))
    {
      printf("Error Copying Header\n");
      exit(1);
    }

  fseek(fp1,0,SEEK_SET+(inImage->imOffset));
  fseek(fp2,0,SEEK_SET+(outImage->imOffset));

  for(i=0;i<inImage->imHeight;i++)
    {
      for(j=0;j<inImage->imWidth;j++)
	{
	  fread((BYTE *)&pixel,1,sizeof(RGBTRIPLE),fp1);
	  pixel.rgbtBlue=pixel.rgbtRed=pixel.rgbtGreen=(BYTE)(((0.30 * (int)pixel.rgbtRed) + (0.59 * (int)pixel.rgbtGreen) + (0.11 * (int)pixel.rgbtBlue))/3);
	  fwrite((BYTE *)&pixel,1,sizeof(RGBTRIPLE),fp2);
	}
    }
  fclose(fp1);
  fclose(fp2);
  return SUCCESS;
}

/*--------------------------------------------------------*/



/*********************************************
 *         Function:Sharpening Filter        *
 *             (SPATIAL - With Laplacian     *
 *                       Mask)               *
 *********************************************/

/*--------------------------------------------------------*/

int sharpening(image* inImage,image* outImage,char** argv)
{
  RGBTRIPLE pixel;
  int i,j,err;
  FILE* fp1;
  FILE* fp2;
  RGBTRIPLE** imRawData;

  fp1=fopen(argv[1],"rb");
  fp2=fopen(argv[2],"wb");

 
  inImage->imName=(BYTE *)argv[1];
  outImage->imName=(BYTE *)argv[2];
  
  if((err=copyheader(inImage,outImage,fp1,fp2)!=0))
    {
      printf("Error Copying Header\n");
      exit(1);
    }

  fseek(fp1,0,SEEK_SET+(inImage->imOffset));
  fseek(fp2,0,SEEK_SET+(outImage->imOffset));

  imRawData=(RGBTRIPLE **)malloc(((inImage->imHeight)+(masksize - 1)) * sizeof(RGBTRIPLE *));
  
  for(i=0;i<( (inImage->imHeight) +(masksize - 1));i++)
    {
      imRawData[i]=(RGBTRIPLE *)malloc(((inImage->imWidth)+(masksize - 1)) * sizeof(RGBTRIPLE));
    }
  
  if((err=readpixel(imRawData,inImage,fp1))!=0)
    {
      printf("Error Reading Pixel Data\n");
      exit(1);
    }

  
  for(i=((int)(masksize/2));i<((inImage->imHeight)+((int)(masksize/2)));i++)
    {
      for(j=((int)(masksize/2));j<((inImage->imWidth)+((int)(masksize/2)));j++)
	{
	  pixel= sharpeningprocess(imRawData,i,j);
	  fwrite((BYTE *)&pixel,1,sizeof(RGBTRIPLE),fp2);
	}
    }

  for(i=0;i<((inImage->imHeight) + (masksize - 1));i++)
    {
      free((imRawData)[i]);
    }

  free((imRawData));
  fclose(fp1);
  fclose(fp2);

  return SUCCESS;
}

RGBTRIPLE sharpeningprocess(RGBTRIPLE** imData,int rowNo,int colNo)
{
 
  RGBTRIPLE** MASK;
  int x,y,count;
  RGBTRIPLE weighted;
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
	  if(x==(int)(masksize/2) && y==(int)(masksize/2))
	    {
	      Red=Red + ( 8 * MASK[x][y].rgbtRed );
	      Green=Green + ( 8 * MASK[x][y].rgbtGreen );
	      Blue=Blue + ( 8 * MASK[x][y].rgbtBlue );
	    }
	  else
	    {
	      Red=Red -  MASK[x][y].rgbtRed ;
	      Green=Green - MASK[x][y].rgbtGreen ;
	      Blue=Blue  - MASK[x][y].rgbtBlue ;
	    }
	}
    }
 
  if(Red>=255)
    Red=255;
 
  if(Green>=255)
    Green=255;

  if(Blue>=255)
    Blue=255;

  if(Red<=0)
    Red=0;

  if(Green<=0)
    Green=0;

  if(Blue<=0)
    Blue=0;

  weighted.rgbtRed=(BYTE)(Red);
  weighted.rgbtGreen=(BYTE)(Green);
  weighted.rgbtBlue=(BYTE)(Blue);

  for(x=0;x<(masksize);x++)
    free(MASK[x]);

  free(MASK);
  return weighted;
}

/*--------------------------------------------------------*/

