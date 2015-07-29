// hough_rect.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

int thresh = 50;
IplImage* img = 0;
IplImage* img0 = 0;
CvMemStorage* storage = 0;
CvPoint pt[4];const char* wndname = "Square Detection Demo"; 
double angle( CvPoint* pt1, CvPoint* pt2, CvPoint* pt0 )
{    
	double dx1 = pt1->x - pt0->x; 
	double dy1 = pt1->y - pt0->y;  
	double dx2 = pt2->x - pt0->x;  
	double dy2 = pt2->y - pt0->y;    
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
} 
CvSeq* findSquares4( IplImage* img, CvMemStorage* storage )
{  
	CvSeq* contours;
	int i, c, l, N = 11;  
	CvSize sz = cvSize( img->width & -2, img->height & -2 );
	IplImage* timg = cvCloneImage( img );
	IplImage* gray = cvCreateImage( sz, 8, 1 ); 
	IplImage* pyr = cvCreateImage( cvSize(sz.width/2, sz.height/2), 8, 3 );  
	IplImage* tgray;   
	CvSeq* result;  
	double s, t;  
	CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), storage );   
	cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));   
	// down-scale and upscale the image to filter out the noise 
	cvPyrDown( timg, pyr, 7 );  
	cvPyrUp( pyr, timg, 7 );   
	tgray = cvCreateImage( sz, 8, 1 );  
	// find squares in every color plane of the image 
	for( c = 0; c < 3; c++ )  
	{       
		cvSetImageCOI( timg, c+1 );     
		cvCopy( timg, tgray, 0 );           
		for( l = 0; l < N; l++ )     
		{          
			if( l == 0 )     
			{               
				cvCanny( tgray, gray, 0, thresh, 5 );        
				cvDilate( gray, gray, 0, 1 );      
			}           
			else       
			{             
				cvThreshold( tgray, gray, (l+1)*255/N, 255, CV_THRESH_BINARY );
			}                       
		cvFindContours( gray, storage, &contours, sizeof(CvContour),CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );          
			while( contours )    
			{              
  result = cvApproxPoly( contours, sizeof(CvContour), storage,CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 ); 
if( result->total == 4 && fabs(cvContourArea(result,CV_WHOLE_SEQ)) > 1000 &&  cvCheckContourConvexity(result) )  
				{               
					s = 0;      
					for( i = 0; i < 5; i++ )  
					{                   
						if( i >= 2 )           
						{               
							t = fabs(angle( (CvPoint*)cvGetSeqElem( result, i ),(CvPoint*)cvGetSeqElem( result, i-2 ),(CvPoint*)cvGetSeqElem( result, i-1 )));   
							s = s > t ? s : t;     
						}         
					}                                            
					if( s < 0.3 )                      
						for( i = 0; i < 4; i++ )              
							cvSeqPush( squares,                    
							(CvPoint*)cvGetSeqElem( result, i ));     
				}                                      
				contours = contours->h_next;      
			}   
		} 
	}
	cvReleaseImage( &gray );   
	cvReleaseImage( &pyr );  
	cvReleaseImage( &tgray );  
	cvReleaseImage( &timg );   
	return squares;
}  
// the function draws all the squares in the image
void drawSquares( IplImage* img, CvSeq* squares )
{   
	CvSeqReader reader;   
	IplImage* cpy = cvCloneImage( img );   
	int i;       
	cvStartReadSeq( squares, &reader, 0 );     
	for( i = 0; i < squares->total; i += 4 )  
	{       
		CvPoint* rect = pt;    
		int count = 4;      
		memcpy( pt, reader.ptr, squares->elem_size ); 
		CV_NEXT_SEQ_ELEM( squares->elem_size, reader ); 
		memcpy( pt + 1, reader.ptr, squares->elem_size );     
		CV_NEXT_SEQ_ELEM( squares->elem_size, reader );   
		memcpy( pt + 2, reader.ptr, squares->elem_size );   
		CV_NEXT_SEQ_ELEM( squares->elem_size, reader );     
		memcpy( pt + 3, reader.ptr, squares->elem_size );  
		CV_NEXT_SEQ_ELEM( squares->elem_size, reader );         
		cvPolyLine( cpy, &rect, &count, 1, 1, CV_RGB(0,255,0), 1, CV_AA, 0 );
	}        
	cvShowImage( wndname, cpy );  
	cvReleaseImage( &cpy );
}
void on_trackbar( int a )
{   
	if( img )    
		drawSquares( img, findSquares4( img, storage ) );
}
char* names[] = { "t.jpg", 0 };


int _tmain(int argc, _TCHAR* argv[])
{
	int i, c; 
	storage = cvCreateMemStorage(0);    
	for( i = 0; names[i] != 0; i++ )   
	{     
		img0 = cvLoadImage( names[i], 1 );   
		if( !img0 )    
		{        
			printf("Couldn't load %s\n", names[i] );    
			continue;     
		}     
		img = cvCloneImage( img0 );       
    		cvNamedWindow( wndname, 1 );     
		cvCreateTrackbar( "canny thresh", wndname, &thresh, 1000, on_trackbar );     
		on_trackbar(0);       
		c = cvWaitKey(0);     
		cvReleaseImage( &img );    
		cvReleaseImage( &img0 );       
		cvClearMemStorage( storage );      
		if( c == 27 )       
			break;   
	}       
	cvDestroyWindow( wndname );   
	return 0;
}

