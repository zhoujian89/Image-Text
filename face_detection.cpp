// face_detection.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>
#include"iostream"
using namespace std;
 
#ifdef _EiC
#define WIN32
#endif
 
static CvMemStorage* storage = 0;
static CvHaarClassifierCascade* cascade = 0;
 
void detect_and_draw( IplImage* image );
 
const char* cascade_name =
    "haarcascade_frontalface_alt.xml";
/*    "haarcascade_profileface.xml";*/


int _tmain(int argc, _TCHAR* argv[])
{
	 CvCapture* capture = 0;
    IplImage *frame=0, *frame_copy = 0;
	int flag=3;//1代表对视频处理，0代表对图像处理,3代表对多幅图像处理
  // int optlen = strlen("--cascade=");
   // const char* input_name;
 
  /*  if( argc > 1 && strncmp( argv[1], "--cascade=", optlen ) == 0 )
    {
        cascade_name = argv[1] + optlen;
        input_name = argc > 2 ? argv[2] : 0;
    }
    else
    {
        cascade_name = "../../data/haarcascades/haarcascade_frontalface_alt2.xml";
        //opencv装好后haarcascade_frontalface_alt2.xml的路径,
       //也可以把这个文件拷到你的工程文件夹下然后不用写路径名cascade_name= "haarcascade_frontalface_alt2.xml";  
       //或者cascade_name ="C:\\Program Files\\OpenCV\\data\\haarcascades\\haarcascade_frontalface_alt2.xml"
        input_name = argc > 1 ? argv[1] : 0;
    }*/
   cascade_name="D:\\Program Files\\OpenCV2.1\\data\\haarcascades\\haarcascade_frontalface_alt2.xml";
	//cascade_name="D:\\Program Files\\OpenCV2.1\\data\\haarcascades\\haarcascade_lefteye_2splits.xml";
	//cascade_name="D:\\Program Files\\OpenCV2.1\\data\\haarcascades\\haarcascade_mcs_mouth.xml";
	//frontalface_alt2
 
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name, 0, 0, 0 );
 
    if( !cascade )
    {
        fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
        fprintf( stderr,
        "Usage: facedetect --cascade=\"<cascade_path>\" [filename|camera_index]\n" );
        return -1;
    }
    storage = cvCreateMemStorage(0);
	//....................................................对图像进行处理.........................................
	if(flag==0){
	IplImage* image = cvLoadImage("test.jpg", 1 );
	cvNamedWindow( "result", 1 );
	 detect_and_draw( image );
      cvWaitKey(0);
      cvReleaseImage( &image );
	}
//.......................................................对视频进行处理...........................................
	if(flag==1){
		capture=cvCaptureFromAVI("tt.mkv");
		cvNamedWindow( "result", 1 );
		 if( capture )
    {
        for(;;)
        {
            if( !cvGrabFrame( capture ))
                break;
            frame = cvRetrieveFrame( capture );
            if( !frame )
                break;
            if( !frame_copy )
                frame_copy = cvCreateImage( cvSize(frame->width,frame->height),
                                            IPL_DEPTH_8U, frame->nChannels );
            if( frame->origin == IPL_ORIGIN_TL )
                cvCopy( frame, frame_copy, 0 );
            else
                cvFlip( frame, frame_copy, 0 );
 
            detect_and_draw( frame_copy );
 
            if( cvWaitKey( 10 ) >= 0 )
                break;
        }
 
        cvReleaseImage( &frame_copy );
        cvReleaseCapture( &capture );
    }
	}
	/*if(flag==1){
		cvNamedWindow("E",1);
		//capture=cvCaptureFromAVI("tt.mkv");
		capture=cvCreateFileCapture("test1.avi");
		while(1){
			frame=cvQueryFrame(capture);
			if(!frame) break;
			cvShowImage("E",frame);
			char c=cvWaitKey(33);
			if(c==27) break;
		}

	}*/
	//...........................................对多幅图像进行处理........................................................
            /* assume it is a text file containing the
               list of the image filenames to be processed - one per line */
	if(flag==3){
           FILE* f = fopen( "face.txt", "rt" );
            if( f )
            {
                char buf[1000+1];
                while( fgets( buf, 1000, f ) )
                {
				   int len = (int)strlen(buf);
                    while( len > 0 && isspace(buf[len-1]) )
                        len--;
                    buf[len] = '\0';
					cout<<buf<<endl;
				 IplImage * image = cvLoadImage( buf, 1 );
                    if( image )
                    {
                        detect_and_draw( image );
                        cvWaitKey(0);
                        cvReleaseImage( &image );
                    }
                }
                fclose(f);
            }
	}
 
   
   
   // cvDestroyWindow("result");
 
    return 0;
}
 
void detect_and_draw( IplImage* img )
{
    static CvScalar colors[] = 
    {
        {{0,0,255}},
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}},
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}}
    };
 
    double scale = 1.3;
    IplImage* gray = cvCreateImage( cvSize(img->width,img->height), 8, 1 );
    IplImage* small_img = cvCreateImage( cvSize( cvRound (img->width/scale),
                         cvRound (img->height/scale)),
                     8, 1 );
    int i;
 
    cvCvtColor( img, gray, CV_BGR2GRAY );
    cvResize( gray, small_img, CV_INTER_LINEAR );
    cvEqualizeHist( small_img, small_img );
    cvClearMemStorage( storage );
 
    if( cascade )
    {
        double t = (double)cvGetTickCount();
        CvSeq* faces = cvHaarDetectObjects( small_img, cascade, storage,
                                            1.1, 2, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
                                            cvSize(30, 30) );
        t = (double)cvGetTickCount() - t;
        printf( "detection time = %gms\n", t/((double)cvGetTickFrequency()*1000.) );
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
            CvPoint center;
            int radius;
            center.x = cvRound((r->x + r->width*0.5)*scale);
            center.y = cvRound((r->y + r->height*0.5)*scale);
            radius = cvRound((r->width + r->height)*0.25*scale);
            cvCircle( img, center, radius, colors[i%8], 3, 8, 0 );
        }
    }
 
    cvShowImage( "result", img );
	cvSaveImage("aaaa.jpg",img);
    cvReleaseImage( &gray );
    cvReleaseImage( &small_img );
}

