// contour.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include<stdlib.h>
#include<string.h>
#include"iostream"
using namespace std;



int _tmain(int argc, _TCHAR* argv[])
{
		IplImage * src=cvLoadImage("new.jpg",0);
	IplImage * imgTemp;
	char * name=0;
	name=new  char[13];
	CvMemStorage* storage = cvCreateMemStorage( 0 );
   CvSeq* contours = NULL;
    int contour_area_tmp = 0; //定义目标对象面积临时寄存
   cvThreshold( src, src,128 , 255,   CV_THRESH_OTSU | CV_THRESH_BINARY_INV);

   imgTemp = cvCloneImage(src);

    // int mode = CV_RETR_EXTERNAL;
//mode = CV_RETR_CCOMP; //内外轮廓都检测
   cvFindContours( imgTemp, storage, &contours, sizeof( CvContour ), CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
   int i=0;
   for( ; contours != NULL; contours = contours -> h_next )
   {
      CvRect rect = cvBoundingRect( contours, 0 );
      cvRectangle(src, cvPoint( rect.x, rect.y ),cvPoint( rect.x + rect.width, rect.y + rect.height ), cvScalar(0,0,0), 0 );
	  cout<<"width"<<rect.width<<"  height"<<rect.height<<endl;
	  //分割图像
	  contour_area_tmp = fabs(cvContourArea( contours, CV_WHOLE_SEQ )); //获取当前轮廓面积
	  cout<<contour_area_tmp<<endl;
	 if(contour_area_tmp>1000&&contour_area_tmp<2500){
	  IplImage * tmp=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	  uchar* data = (uchar*)tmp->imageData;
	  cvSetImageROI(src,cvRect(rect.x,rect.y,rect.width,rect.height));
	  cvCopy(src,tmp,0);
	  cvResetImageROI(src); 
	  IplImage * dst=0;
	  dst=cvCreateImage(cvSize(18,27),tmp->depth,tmp->nChannels);
	  cvResize(tmp,dst,CV_INTER_LINEAR);
	  cvNamedWindow("object"+i,1);
	  cvShowImage("object"+i,dst);
	  itoa(i+1,name,10);
	  string s(name);
	  s.append(".jpg");
	  cvSaveImage(s.c_str(),dst);
	  i++;
	 }
   }
  
   cvSaveImage("tep.jpg",src);
	cvNamedWindow("src",1);
	cvShowImage("src",src);
	cvWaitKey(0);
	return 0;
}

