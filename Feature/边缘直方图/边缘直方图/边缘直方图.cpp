// 边缘直方图.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <ctype.h>
#define PI 3.14


int _tmain(int argc, _TCHAR* argv[])
{
	 IplImage *src = 0;  // source imagre
    IplImage *histimg = 0; // histogram image 
    CvHistogram *hist = 0; // define multi_demention histogram
    IplImage* canny;
    CvMat* canny_m;
    IplImage* dx; // the sobel x difference 
    IplImage* dy; // the sobel y difference 
    CvMat* gradient; // value of gradient
    CvMat* gradient_dir; // direction of gradient
    CvMat* dx_m; // format transform to matrix
    CvMat* dy_m;
    CvMat* mask;
    CvSize  size;
    IplImage* gradient_im;
    int i,j;
    float theta;
    
    int hdims = 8;     // 划分HIST的个数，越高越精确
    float hranges_arr[] = {-PI/2,PI/2}; // 直方图的上界和下界
    float* hranges = hranges_arr;
                                                                                                                                                                                                                                                              
    float max_val;  // 
    int bin_w;
    
    src=cvLoadImage("t.jpg", 0); // force to gray image
       if(src==0) return -1;
    
    cvNamedWindow( "Histogram", 0 );
    //cvNamedWindow( "src", 0);
    size=cvGetSize(src);
    canny=cvCreateImage(cvGetSize(src),8,1);//边缘图像
    dx=cvCreateImage(cvGetSize(src),32,1);//x方向上的差分//此处的数据类型为U 不怕溢出吗？
    dy=cvCreateImage(cvGetSize(src),32,1);
    gradient_im=cvCreateImage(cvGetSize(src),32,1);//梯度图像
    canny_m=cvCreateMat(size.height,size.width,CV_32FC1);//边缘矩阵
    dx_m=cvCreateMat(size.height,size.width,CV_32FC1);
    dy_m=cvCreateMat(size.height,size.width,CV_32FC1);
    gradient=cvCreateMat(size.height,size.width,CV_32FC1);//梯度矩阵
    gradient_dir=cvCreateMat(size.height,size.width,CV_32FC1);//梯度方向矩阵
    mask=cvCreateMat(size.height,size.width,CV_32FC1);//掩码

    cvCanny(src,canny,60,180,3);//边缘检测
    cvConvert(canny,canny_m);//把图像转换为矩阵
    cvSobel(src,dx,1,0,3);// 一阶X方向的图像差分:dx
    cvSobel(src,dy,0,1,3);// 一阶Y方向的图像差分:dy
    cvConvert(dx,dx_m);
    cvConvert(dy,dy_m);
    cvAdd(dx_m,dy_m,gradient); // value of gradient//梯度不是等于根号下x的导数的平方加上y导数的平方吗？
    cvDiv(dx_m,dy_m,gradient_dir); // direction
    for(i=0;i<size.height;i++)
    for(j=0;j<size.width;j++)
    {
      if(cvmGet(canny_m,i,j)!=0 && cvmGet(dx_m,i,j)!=0)//只看边缘上的方向
      {
		  theta=cvmGet(gradient_dir,i,j);
		  theta=atan(theta);
		  cvmSet(gradient_dir,i,j,theta);   
      }
      else
      {
         cvmSet(gradient_dir,i,j,0);
      }
         
    }
   hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );  
   // 创建一个指定尺寸的直方图，并返回创建的直方图指针
    histimg = cvCreateImage( cvSize(320,200), 8, 3 ); // 创建一个图像，通道
	cvZero( histimg ); // 清零
	 cvConvert(gradient_dir,gradient_im);//把梯度方向矩阵转化为图像
	  cvCalcHist( &gradient_im, hist, 0, canny ); // 计算直方图
	   cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );  // 只找最大值
	   // 缩放bin 到区间[0,255] ，比例系数
	   cvConvertScale( hist->bins, hist->bins, max_val ? 255/ max_val : 0, 0 );
	   cvZero( histimg );
	   bin_w = histimg->width /16;  // hdims: 条的个数，则bin_w 为条的宽度

	     // 画直方图
    for( i = 0; i < hdims; i++ )
    {
       double val = ( cvGetReal1D(hist->bins,i)*histimg->height/255 );
     // 返回单通道数组的指定元素， 返回直方图第i条的大小，val为histimg中的i条的高度
        CvScalar color = CV_RGB(255,255,0); //(hsv2rgb(i*180.f/hdims);//直方图颜色
        cvRectangle( histimg, cvPoint(100+i*bin_w,histimg->height),cvPoint(100+(i+1)*bin_w,(int)(histimg->height - val)), color, 1, 8, 0 ); 
    //画直方图——画矩形，左下角，右上角坐标
     }
    cvShowImage( "src", src);
    cvShowImage( "Histogram", histimg );
    cvWaitKey(0);
	cvDestroyWindow("src");
    cvDestroyWindow("Histogram");
    cvReleaseImage( &src );
    cvReleaseImage( &histimg );
    cvReleaseHist ( &hist );
	return 0;
}

