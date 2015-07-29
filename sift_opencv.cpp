// sift_opencv.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "highgui.h"
#include<legacy/legacy.hpp>
#include <nonfree/features2d.hpp>
#include"iostream"
using namespace std;
using namespace cv;


int _tmain(int argc, _TCHAR* argv[])
{
	//SIFT特征点查找
//Mat image=imread("beaver.png");
//Mat iamgeGray=imread("beaver.png",0);
/*Mat image=imread("test.jpg");
Mat iamgeGray=imread("test.jpg",0);

Mat descriptors;
vector<KeyPoint> keypoints;

SiftFeatureDetector sift2(0.06f,10.0);
sift2.detect(iamgeGray,keypoints);
//SurfFeatureDetector detector(40);
drawKeypoints(image,keypoints,image,Scalar(255,0,255));
//imshow("beaver.png",image);
imshow("test.jpg",image);
imwrite("result.jpg",image);

waitKey(0);*/

	//SIFT特征点匹配
Mat input1=imread("beaver.png",1);
Mat input2=imread("beaver_xform.png",1);
//Mat input1 = imread("aa.jpg");  
  //  Mat input2 = imread("cc.jpg");  
SiftFeatureDetector detector;
vector<KeyPoint> keypoint1,keypoint2;
detector.detect(input1,keypoint1);

Mat output1;
drawKeypoints(input1,keypoint1,output1);
imshow("sift_result1.jpg",output1);
imwrite("sift_result1.jpg",output1);

Mat output2;
SiftDescriptorExtractor extractor;
Mat descriptor1,descriptor2;
//Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create( "BruteForce" );//创建特征匹配器 
BruteForceMatcher<L2<float>> matcher;

vector<DMatch> matches;
Mat img_matches;
detector.detect(input2,keypoint2);
drawKeypoints(input2,keypoint2,output2);

imshow("sift_result2.jpg",output2);
imwrite("sift_result2.jpg",output2);

extractor.compute(input1,keypoint1,descriptor1);
extractor.compute(input2,keypoint2,descriptor2);

matcher.match(descriptor1,descriptor2,matches);

drawMatches(input1,keypoint1,input2,keypoint2,matches,img_matches);
imshow("matches",img_matches);
imwrite("matches.jpg",img_matches);

waitKey();

	return 0;

    /* initModule_nonfree();//初始化模块，使用SIFT或SURF时用到  
    Ptr<FeatureDetector> detector = FeatureDetector::create( "SIFT" );//创建SIFT特征检测器  
    Ptr<DescriptorExtractor> descriptor_extractor = DescriptorExtractor::create( "SIFT" );//创建特征向量生成器  
    Ptr<DescriptorMatcher> descriptor_matcher = DescriptorMatcher::create( "BruteForce" );//创建特征匹配器  
    if( detector.empty() || descriptor_extractor.empty() )  
        cout<<"fail to create detector!";  
  
    //读入图像  
   // Mat img1 = imread("beaver.png");  
   // Mat img2 = imread("beaver_xform.png");  
	Mat img1 = imread("aa.jpg");  
    Mat img2 = imread("cc.jpg");  
  
    //特征点检测  
    double t = getTickCount();//当前滴答数  
    vector<KeyPoint> keypoints1,keypoints2;  
    detector->detect( img1, keypoints1 );//检测img1中的SIFT特征点，存储到keypoints1中  
    detector->detect( img2, keypoints2 );  
    cout<<"图像1特征点个数:"<<keypoints1.size()<<endl;  
    cout<<"图像2特征点个数:"<<keypoints2.size()<<endl;  
  
    //根据特征点计算特征描述子矩阵，即特征向量矩阵  
    Mat descriptors1,descriptors2;  
    descriptor_extractor->compute( img1, keypoints1, descriptors1 );  
    descriptor_extractor->compute( img2, keypoints2, descriptors2 );  
    t = ((double)getTickCount() - t)/getTickFrequency();  
    cout<<"SIFT算法用时："<<t<<"秒"<<endl;  
  
  
    cout<<"图像1特征描述矩阵大小："<<descriptors1.size()  
        <<"，特征向量个数："<<descriptors1.rows<<"，维数："<<descriptors1.cols<<endl;  
    cout<<"图像2特征描述矩阵大小："<<descriptors2.size()  
        <<"，特征向量个数："<<descriptors2.rows<<"，维数："<<descriptors2.cols<<endl;  
  
    //画出特征点  
    Mat img_keypoints1,img_keypoints2;  
    drawKeypoints(img1,keypoints1,img_keypoints1,Scalar::all(-1),0);  
    drawKeypoints(img2,keypoints2,img_keypoints2,Scalar::all(-1),0);  
    //imshow("Src1",img_keypoints1);  
    //imshow("Src2",img_keypoints2);  
  
    //特征匹配  
    vector<DMatch> matches;//匹配结果  
    descriptor_matcher->match( descriptors1, descriptors2, matches );//匹配两个图像的特征矩阵  
    cout<<"Match个数："<<matches.size()<<endl;  
  
    //计算匹配结果中距离的最大和最小值  
    //距离是指两个特征向量间的欧式距离，表明两个特征的差异，值越小表明两个特征点越接近  
    double max_dist = 0;  
    double min_dist = 100;  
    for(int i=0; i<matches.size(); i++)  
    {  
        double dist = matches[i].distance;  
        if(dist < min_dist) min_dist = dist;  
        if(dist > max_dist) max_dist = dist;  
    }  
    cout<<"最大距离："<<max_dist<<endl;  
    cout<<"最小距离："<<min_dist<<endl;  
  
    //筛选出较好的匹配点  
    vector<DMatch> goodMatches;  
    for(int i=0; i<matches.size(); i++)  
    {  
        if(matches[i].distance < 0.31 * max_dist)  
        {  
            goodMatches.push_back(matches[i]);  
        }  
    }  
    cout<<"goodMatch个数："<<goodMatches.size()<<endl;  
  
    //画出匹配结果  
    Mat img_matches;  
    //红色连接的是匹配的特征点对，绿色是未匹配的特征点  
    drawMatches(img1,keypoints1,img2,keypoints2,goodMatches,img_matches,  
                Scalar::all(-1)/*CV_RGB(255,0,0)*/
	//,CV_RGB(0,255,0),Mat(),2);  
  
 //imshow("MatchSIFT",img_matches);  
  //  waitKey(0);  
	//return 0;*/

}
  

