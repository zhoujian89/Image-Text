// corner_cvGoodFeaturesToTrack.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"iostream"
#include "cv.h"
#include "highgui.h"
using namespace std;
#define MAX_CORNERS 100

int _tmain(int argc, _TCHAR* argv[])
{
	int cornersCount=MAX_CORNERS;//得到的角点数目
CvPoint2D32f corners[MAX_CORNERS];//输出角点集合
IplImage *srcImage = 0,*grayImage = 0,*corners1 = 0,*corners2 = 0;
int i;
CvScalar color = CV_RGB(255,0,0);
cvNamedWindow("image",1);

//Load the image to be processed
srcImage = cvLoadImage("dd.jpg",1);
grayImage = cvCreateImage(cvGetSize(srcImage),IPL_DEPTH_8U,1);

//copy the source image to copy image after converting the format
//复制并转为灰度图像
cvCvtColor(srcImage,grayImage,CV_BGR2GRAY);

//create empty images os same size as the copied images
//两幅临时位浮点图像，cvGoodFeaturesToTrack会用到
corners1 = cvCreateImage(cvGetSize(srcImage),IPL_DEPTH_32F,1);
corners2 = cvCreateImage(cvGetSize(srcImage),IPL_DEPTH_32F,1);

cvGoodFeaturesToTrack(grayImage,corners1,corners2,corners,&cornersCount,0.05,
30,//角点的最小距离是
0,//整个图像
3,0,0.4);
printf("num corners found: %d\n",cornersCount);

//开始画出每个点
if(cornersCount>0){
	for(i=0;i<cornersCount;i++)
	{
		cvCircle(srcImage,cvPoint((int)(corners[i].x),(int)(corners[i].y)),2,color,2,CV_AA,0);
	}
}
cvShowImage("image",srcImage);
cvSaveImage("imagedst.png",srcImage);

cvReleaseImage(&srcImage);
cvReleaseImage(&grayImage);
cvReleaseImage(&corners1);
cvReleaseImage(&corners2);

cvWaitKey(0);
	return 0;
}

