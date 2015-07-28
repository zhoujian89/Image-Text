// hough_circle.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include "iostream"
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	
    IplImage * img=cvLoadImage("circle.jpg", 1);
    IplImage * gray = cvCreateImage( cvGetSize(img), 8, 1 );
     CvMemStorage* storage = cvCreateMemStorage(0);
     cvCvtColor( img, gray, CV_BGR2GRAY );

	 // smooth it, otherwise a lot of false circles may be detected
     cvSmooth( gray, gray, CV_GAUSSIAN, 5, 15 );
	 CvSeq* circles = cvHoughCircles( gray, storage, CV_HOUGH_GRADIENT, 2,10, 200, 100 );
	  int i;
	  int num=0;
     for( i = 0; i < circles->total; i++ )
     {
          float* p = (float*)cvGetSeqElem( circles, i );
		  if(p[2]>100){
         cvCircle( img, cvPoint(cvRound(p[0]),cvRound(p[1])), 3, CV_RGB(0,255,0), -1, 8, 0 );
          cvCircle( img, cvPoint(cvRound(p[0]),cvRound(p[1])), cvRound(p[2]), CV_RGB(255,0,0), 3, 8, 0 );
          cout<<"圆心坐标x= "<<cvRound(p[0])<<endl<<"圆心坐标y= "<<cvRound(p[1])<<endl;
          cout<<"半径="<<cvRound(p[2])<<endl; 
		  num++;
		  }
     }
     cout<<"圆数量="<<num<<endl;
	 cvSaveImage("aaa.jpg",img);
     cvNamedWindow( "circles", 1 );
     cvShowImage( "circles", img );
	 cvWaitKey(0);
	 




	return 0;
}

