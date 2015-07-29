// candy_margin.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h" 

int _tmain(int argc, _TCHAR* argv[])
{
	  //声明IplImage指针
         IplImage* pImg = NULL; 
	    IplImage* pCannyImg = NULL; 
	    //载入图像，强制转化为Gray
		pImg = cvLoadImage( "1.bmp", 0); 
		//为canny边缘图像申请空间
		pCannyImg = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
		//canny边缘检测
		cvCanny(pImg, pCannyImg, 50, 150, 3); 
		//创建窗口
		cvNamedWindow("src", 1);
		cvNamedWindow("canny",1);
		//显示图像
		cvShowImage( "src", pImg ); 
		cvShowImage( "canny", pCannyImg ); 
		//等待按键
		cvWaitKey(0);
		//销毁窗口
		cvDestroyWindow( "src" ); 
		cvDestroyWindow( "canny" );
		//释放图像
		cvReleaseImage( &pImg ); 
		cvReleaseImage( &pCannyImg );
		return 0;
}

