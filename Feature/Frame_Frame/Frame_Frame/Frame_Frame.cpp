// Frame_Frame.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"highgui.h"
#include"cv.h"

void hist_show(IplImage * src){

	//Compute the HSV image and decompose it into seperate planes
	IplImage * hsv=cvCreateImage(cvGetSize(src),8,3);
	cvCvtColor(src,hsv,CV_BGR2HSV);

	IplImage * h_plane=cvCreateImage(cvGetSize(src),8,1);
	IplImage * s_plane=cvCreateImage(cvGetSize(src),8,1);
	IplImage * v_plane=cvCreateImage(cvGetSize(src),8,1);
	IplImage * planes[]={h_plane,s_plane};
	cvCvtPixToPlane(hsv,h_plane,s_plane,v_plane,0);
//分割多通道数组成几个单通道数组或者从数组中提取一个通道 　　
//一般用法是cvCvtPixToPlane（IplImage * src,IplImage * dst1,IplImage *dst2,IplImage * dst3,IplImage *dst4) 　　
//第一个参数是源图像，后面是分离出来每个通道的目标图像，如果原图像是3通道的，可以把最后一个参数设置为空。
//例如cvCvtPixToPlane（IplImage * src,IplImage * dst1,IplImage *dst2,IplImage * dst3,NULL)，NULL也可以写为0.

	//Build the histogram and compute its contents.
	int h_bins=16,s_bins=8;
	CvHistogram * hist;
		int hist_size[]={h_bins,s_bins};
		float h_ranges[]={0,180};//   hue varies from 0 (~0°red) to 180 (~360°red again) 
		float s_ranges[]={0,255};//saturation varies from 0 (black-gray-white) to 255 (pure spectrum color)
		float * ranges[]={h_ranges,s_ranges};
		hist=cvCreateHist(
			2,
			hist_size,
			CV_HIST_ARRAY,
			ranges,
			1		
			);

	cvCalcHist(planes,hist,0,0);//. Compute histogra
	cvNormalizeHist(hist,1.0);//Normalize it
	//Create an image to use to visualize[显现] our histogram
	int scale=10;
    int height =240 ;
	int width =768;
     
	IplImage * hist_img=cvCreateImage(cvSize(width,height),8,3);//最后在这幅图像显示结果
	cvZero(hist_img);

	//populate our visualization with little gray squares.
	float max_value=0;
	cvGetMinMaxHistValue(hist,0,& max_value,0,0);

 
	/** 用来进行HSV到RGB颜色转换的临时单位图像 */
	IplImage * hsv_color = cvCreateImage(cvSize(1,1),8,3);
	IplImage * rgb_color = cvCreateImage(cvSize(1,1),8,3);
    int bin_w = scale;


	for(int h=0;h<h_bins;h++){
		for(int s=0;s<s_bins;s++){
           int i = h*s_bins + s;

               
			float bin_val=cvQueryHistValue_2D(hist,h,s);//每个函数返回相应bin中的值的浮点数
			int intensity=cvRound(bin_val*height/max_value);//归一化

			/** 获得当前直方图代表的颜色，转换成RGB用于绘制 */
			cvSet2D(hsv_color,0,0,cvScalar(h*180.f / h_bins,s*255.f/s_bins,255,0));
			cvCvtColor(hsv_color,rgb_color,CV_HSV2BGR);
			CvScalar color = cvGet2D(rgb_color,0,0);


cvRectangle( hist_img, cvPoint(i*bin_w,height),
				cvPoint((i+1)*bin_w, height - intensity),
				color, -1, 8, 0 );

		}
	}
	

	
	cvShowImage("H-S Histogram",hist_img);

}

int _tmain(int argc, _TCHAR* argv[])
{

IplImage * src=cvLoadImage("test.jpg",1);
cvNamedWindow("H-S Histogram",1);
hist_show(src);
cvWaitKey(0);
return 0;
}

