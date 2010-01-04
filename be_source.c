#include "be_header.h"




#define INFILE "o.bmp"
#define OUTFILE "C:\p.bmp"
#define OUTFILE2 "C:\q.bmp"

int main()
{
  FILE *fp1=NULL;
  FILE *fp2;
  FILE *fp3;
  BITMAPFILEHEADER bmfh;
  BITMAPINFOHEADER bmih;
  RGBTRIPLE *pixel=(RGBTRIPLE *)malloc(sizeof(RGBTRIPLE));
  BYTE try;
  int i=0;
  int read=0;
  int j=0;
  BYTE avg[3][3];  
  RGBTRIPLE rgb[3][3];
  int errcode;
  int Blue;
  int Green;
  int Red,temp1,temp2;
  char buffer[10],buffer2[10];
  int rowoff,coloff; 
  LONG pos=0;
  LONG row=0;
  LONG col=0;

  printf("%d\n",sizeof(RGBTRIPLE));
  if((errcode=fopen_s(&fp1,INFILE,"rb"))!=0)
		printf("File Does Not Exist");
  if((errcode=fopen_s(&fp2,OUTFILE,"wb"))!=0)
	  printf("File Does Not Exist");
  if((errcode=fopen_s(&fp3,OUTFILE2,"wb"))!=0);
  { printf("File Does Not Exist");
  exit(0);}
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
  printf("\n%d\n",bmih.biWidth);
  printf("\n%d\n",bmih.biHeight);
  printf("\n%u\n",bmih.biPlanes);
  printf("\n%u\n",bmih.biBitCount);
  printf("\n%lu\n",bmih.biCompression);
  printf("\n%lu\n",bmih.biSizeImage);
  printf("\n%d\n",bmih.biXPelsPerMeter);
  printf("\n%d\n",bmih.biYPelsPerMeter);
  printf("\n%lu\n",bmih.biClrUsed);
  printf("\n%lu\n",bmih.biClrImportant);
  printf("%d\n",pos);
  fseek(fp1,bmfh.bfOffsetBits,SEEK_SET);





  for( j=0; j<bmih.biHeight; j++ )
    {
     
      read = 0;
      for( i=0; i<bmih.biWidth; i++ ) 
	{
	  fread(pixel, 1, sizeof(RGBTRIPLE),fp1);
	  fwrite(pixel,1,sizeof(RGBTRIPLE),fp2);
	  read += sizeof(RGBTRIPLE);
	 
	}
      
    }
  i=(int)(bmih.biHeight);
  j=(int)(bmih.biWidth);
  fseek(fp1,bmfh.bfOffsetBits,SEEK_SET);     /*Set Position to Start of Pixel Data*/
  row=0;      /*Offset for Row of Bitmap Array*/

  /*Begin Loop*/
  while(row<bmih.biHeight)
    {
      col=0;         /*Offset for Column of Bitmap Array*/
      while(col<bmih.biWidth)
	{
	  fseek(fp1,(bmfh.bfOffsetBits)+(row * (bmih.biWidth * 3))+(col),SEEK_SET);  /*Move to Next Pixel (central pixel on which mask coincides)*/
	  
	  pos=ftell(fp1);
	  pos=pos-((bmih.biWidth *3) + MASKSIZE);
	  if(row==0)
	    {
	      if(col==0)
		{
		  fseek(fp1,0,SEEK_CUR);      /*for poition (0,0) read pixel from (0,0)*/
		}
	      else
		{
		  fseek(fp1,ftell(fp1)-3,SEEK_CUR);  /*for position(0,col) read pixel into array from previous pixel( - 3Bytes)*/
		}
	    }
	  else
	    {
	      if(col==0)
		{
		  fseek(fp1,ftell(fp1)-(bmih.biWidth *3),SEEK_CUR);  /*for position(row,0) start reading into array from the pixel directly above current                                                                        position*/
		}
	      else
		{
		  if(col == (bmih.biWidth)-1)
		    {
		      fseek(fp1,ftell(fp1)-((bmih.biWidth+1)*3),SEEK_CUR); /*for poition(row,Width-1) read pixel data into array at position (-1,-1) in a                                                                               3x3 array*/
		    }
		  else
		    {
		      fseek(fp1,ftell(fp1)-((bmih.biWidth+1)*3),SEEK_CUR); /*for position(row,col)*/
		    }
		}
	    }


	  /*Begin Reading Pixel Data Into Array */
	  for(rowoff=-1;rowoff<2;rowoff++)
	    {
	      
	      for(coloff=-1;coloff<2;coloff++)
		{
		  if(row==0)
		    {
		      rgb[-1][-1].rgbtBlue=rgb[-1][0].rgbtBlue=rgb[-1][2].rgbtBlue=0;
		      rgb[-1][-1].rgbtGreen=rgb[-1][0].rgbtGreen=rgb[-1][2].rgbtGreen=0;
		      rgb[-1][-1].rgbtRed=rgb[-1][0].rgbtRed=rgb[-1][2].rgbtRed=0;
		      rowoff++;
		      
		    }
		  if(col==0)
		    {
		      rgb[-1][-1].rgbtBlue=rgb[0][-1].rgbtBlue=rgb[1][-1].rgbtBlue=0;
		      rgb[-1][-1].rgbtGreen=rgb[0][-1].rgbtGreen=rgb[1][-1].rgbtGreen=0;
		      rgb[-1][-1].rgbtRed=rgb[0][-1].rgbtRed=rgb[1][-1].rgbtRed=0;
		      coloff++;
		    }
		  
		  fread(pixel,1,sizeof(RGBTRIPLE),fp1);
		  rgb[rowoff][coloff].rgbtBlue=pixel->rgbtBlue;
		  rgb[rowoff][coloff].rgbtGreen=pixel->rgbtGreen;
		  rgb[rowoff][coloff].rgbtRed=pixel->rgbtRed;
		  
		  
		    if(col==(bmih.biWidth-1))
		    {
		      rgb[-1][1].rgbtBlue=rgb[0][1].rgbtBlue=rgb[1][1].rgbtBlue=0;
		      rgb[-1][1].rgbtGreen=rgb[0][1].rgbtGreen=rgb[1][1].rgbtGreen=0;
		      rgb[-1][1].rgbtRed=rgb[0][1].rgbtRed=rgb[1][1].rgbtRed=0;
		      coloff++;
		    }
		  if(row==(bmih.biHeight))
		    {
		      rgb[1][-1].rgbtBlue=rgb[1][0].rgbtBlue=rgb[1][1].rgbtBlue=0;
		      rgb[1][-1].rgbtGreen=rgb[1][0].rgbtGreen=rgb[1][1].rgbtGreen=0;
		      rgb[1][-1].rgbtRed=rgb[1][0].rgbtRed=rgb[1][1].rgbtRed=0;
		      rowoff++;
		    }
		}
	      fseek(fp1,((bmih.biWidth)*3)+(MASKSIZE),SEEK_CUR);

	    }
	  /*Finish Read*/

	  /*Begin Averaging Array color value*/ 
	  for(i=-1;i<2;i++)
	    {	
	      for(j=-1;j<2;j++)
		{
		  Blue=(Blue+(rgb[i][j].rgbtBlue))/9;
		  Green=(Green+(rgb[i][j].rgbtGreen))/9;
		  Red=(Red+(rgb[i][j].rgbtRed))/9;
		}
	    }
	  /*Averaging Finish*/


	  /*Write Averaged Values Into Fil*/
	  pixel->rgbtBlue=Blue;
	  pixel->rgbtGreen=Green;
	  pixel->rgbtRed=Red;
	  fwrite(pixel,1,sizeof(RGBTRIPLE),fp3);
	  /*Finish Writing*/

	  col++;
	}
      row++;
    }
    
  /*End Loop*/
    

	    
      

  


  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
  

  return 0;

}
