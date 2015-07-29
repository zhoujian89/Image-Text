// histogram.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	IplImage * src= cvLoadImage("test.jpg",1);
 
	IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
	IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane };
 
	/** H ��������Ϊ16���ȼ���S��������Ϊ8���ȼ�*/
	int h_bins = 16, s_bins = 8;
	int hist_size[] = {h_bins, s_bins};
 
	/** H �����ı仯��Χ*/
	float h_ranges[] = { 0, 180 }; 
 
	/** S �����ı仯��Χ*/
	float s_ranges[] = { 0, 255 };
	float* ranges[] = { h_ranges, s_ranges };
 
	/** ����ͼ��ת����HSV��ɫ�ռ�*/
	cvCvtColor( src, hsv, CV_BGR2HSV );
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
 
	/** ����ֱ��ͼ����ά, ÿ��ά���Ͼ���*/
	CvHistogram * hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
	/** ����H,S����ƽ������ͳ��ֱ��ͼ*/
	cvCalcHist( planes, hist, 0, 0 );
 
	/** ��ȡֱ��ͼͳ�Ƶ����ֵ�����ڶ�̬��ʾֱ��ͼ*/
	float max_value;
	cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );
 
 
	/** ����ֱ��ͼ��ʾͼ��*/
	int height = 240;
	int width = (h_bins*s_bins*6);
	IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 3 );
	cvZero( hist_img );
 
	/** ��������HSV��RGB��ɫת������ʱ��λͼ��*/
	IplImage * hsv_color = cvCreateImage(cvSize(1,1),8,3);
	IplImage * rgb_color = cvCreateImage(cvSize(1,1),8,3);
	int bin_w = width / (h_bins * s_bins);
	for(int h = 0; h < h_bins; h++)
	{
		for(int s = 0; s < s_bins; s++)
		{
			int i = h*s_bins + s;
			/** ���ֱ��ͼ�е�ͳ�ƴ�����������ʾ��ͼ���еĸ߶�*/
			float bin_val = cvQueryHistValue_2D( hist, h, s );
			int intensity = cvRound(bin_val*height/max_value);
 
			/** ��õ�ǰֱ��ͼ��������ɫ��ת����RGB���ڻ���*/
			cvSet2D(hsv_color,0,0,cvScalar(h*180.f / h_bins,s*255.f/s_bins,255,0));
			cvCvtColor(hsv_color,rgb_color,CV_HSV2BGR);
			CvScalar color = cvGet2D(rgb_color,0,0);
 
			cvRectangle( hist_img, cvPoint(i*bin_w,height),
				cvPoint((i+1)*bin_w,height - intensity),
				color, -1, 8, 0 );
		}
	}
 
	cvNamedWindow( "Source", 1 );
	cvShowImage( "Source", src );
 
	cvNamedWindow( "H-S Histogram", 1 );
	cvShowImage( "H-S Histogram", hist_img );
 
	cvWaitKey(0);
	return 0;
}
