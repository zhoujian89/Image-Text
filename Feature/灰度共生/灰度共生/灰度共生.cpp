// 灰度共生.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>

void doWork(IplImage * image, int n, FILE *fp) {                                                    
	IplImage *src = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U,1);
	cvCvtColor(image, src, CV_RGB2GRAY);
	int width = src->width;
	int height = src->height;
	uchar* data = (uchar*)src->imageData;
	int step = src->widthStep;

	for (int i=0;i<=height-1;i++)                                       //灰度级粗量化
		for (int j=0;j<=width-1;j++)
			data[i*step+j] = data[i*step+j]/n;

	int dy = 0,dx = 1;
	double s  =  0., lg[17][17];  //h[i][j]表示一个灰度为i而另一个灰度为 j的两个相距为( dx，dy)的像素对出现的次数，p[i][j]表示概率
	double CON = 0, ASM = 0, ENT = 0;
	double h[17][17] = {0.}, p[17][17] = {0.};


	for(int i=0;i<=height-1;i++)                                                         //求h[i][j]
		for(int j=0;j<=width-2;j++) {
				h[data[i*step+j]][data[(i+dy)*step+j+dx]] += 1;
				h[data[(i+dy)*step+j+dx]][data[i*step+j]] += 1;
		}
	for (int i=0;i<n;i++)
		for (int j=0;j<n;j++)
			s += h[i][j];
	
	for (int i=0;i<n;i++)                                                                        //求p[i][j]
		for (int j=0;j<n;j++)
			p[i][j] = h[i][j]/s;

	for(int i=0;i<n;i++)                                                                                //求特征向量
		for(int j=0;j<n;j++)
		{
			if( p[i][j]!=0 )  lg[i][j]= log( p[i][j] );
			else{ lg[i][j]=0; }
			CON+=(i-j)*(i-j)*p[i][j];
			ASM+=p[i][j]*p[i][j];
			ENT+=p[i][j]*lg[i][j];
			//COR+=i*j*p[i][j];
		}
		ENT = -ENT;
	//fprintf(fp, "\nCON(对比度)=%f    ASM(角二阶距【能量】)=%f    ENT(熵)=%f",CON,ASM,ENT);
		fprintf(fp, "%f\n",ENT);
}

void main(int argc, char * * argv[]) {
	int n = 16;//分为16个灰度级
	FILE * fp;
	fp = fopen( "out.txt", "a" );
	/*IplImage * image  = cvLoadImage( "test.jpg" );
	doWork( image, n, fp);*/
	
	CvCapture* capture = cvCreateFileCapture( "1.avi" );
	IplImage* frame;
	while(1) {
		frame = cvQueryFrame( capture );
		if( !frame )  break;
		doWork( frame, n, fp);
	}
	system( "pause" );
}