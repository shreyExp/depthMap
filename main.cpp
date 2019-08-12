#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <plplot/plstream.h>
#include <plplot/plplot.h>
#include "helper.h"
#include "getParams.h"
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
using namespace cv;
using namespace std;
template <class T>
void shiftAboveZero(Mat &img);
template <class T>
void minmax(Mat& depth, T &min, T &max);
int main(int argc, char**argv){
	string name;	
	if(argc > 1)
		name = argv[1];
	else
		name = "parameters.json";
	const char* fi = name.c_str();
	GetPara para(fi);
	Mat im1, im2;
	if(para.doubleImage()){
		im1 = imread(para.getLeftImageFileName(), 1);
		im2 = imread(para.getRightImageFileName(), 1);
		cout<<para.getLeftImageFileName()<<endl;
		cout<<para.getRightImageFileName()<<endl;
		cvtColor(im1, im1, CV_BGR2GRAY);
		cvtColor(im2, im2, CV_BGR2GRAY);
	}else{
		Mat img = imread(para.getImageFileName() ,1);
		auto s = img.size();
		cvtColor(img, img, CV_BGR2GRAY);
		Rect r(0,0,(s.width/2), (s.height));
		im1 = img(r);
		Rect r1((s.width/2), 0, (s.width/2), (s.height));
		im2 = img(r1);
	}

//	double *hhh, *bbb;
//	histogram<uchar>(im1, hhh, bbb, 100);
	Mat disp;
	if(!para.getSBMChoice()){
		Ptr<StereoBM> sbm = StereoBM::create(16,21);
		if(para.getDefault()){
    			sbm->setPreFilterSize(para.getPreFilterSize());  // must be an odd between 5 and 255
    			sbm->setPreFilterCap(para.getPreFilterCap());  // must be within 1 and 63
    			sbm->setBlockSize(para.getBlockSize());  // must be odd, be within 5..255 and be not > w or h 
    			sbm->setMinDisparity(para.getMinDisparity());
    			sbm->setNumDisparities(para.getNumDisparity());  // must be > 0 and divisible by 16
    			sbm->setTextureThreshold(para.getTextureThreshold());  // must be non-negative
    			sbm->setUniquenessRatio(para.getUniquenessRatio());  // must be non-negative
    			sbm->setSpeckleWindowSize(para.getSpeckleWindowSize());
    			sbm->setSpeckleRange(para.getSpeckleRange());
    			sbm->setDisp12MaxDiff(para.getDisp12MaxDiff());
		}
		sbm->compute( im1, im2, disp );
	}else{
		Ptr<StereoSGBM> sgbm = StereoSGBM::create();
    			sgbm->setPreFilterCap(para.getPreFilterCap());  // must be within 1 and 63
    			sgbm->setBlockSize(para.getBlockSize());  // must be odd, be within 5..255 and be not > w or h 
    			sgbm->setMinDisparity(para.getMinDisparity());
    			sgbm->setNumDisparities(para.getNumDisparity());  // must be > 0 and divisible by 16
    			sgbm->setUniquenessRatio(para.getUniquenessRatio());  // must be non-negative
    			sgbm->setSpeckleWindowSize(para.getSpeckleWindowSize());
    			sgbm->setSpeckleRange(para.getSpeckleRange());
    			sgbm->setDisp12MaxDiff(para.getDisp12MaxDiff());
			sgbm->compute( im1, im2, disp );
	}
	//shiftAboveZero<short>(disp);
//	GaussianBlur( disp, disp, Size( 5, 5 ), 0, 0 );
//	medianBlur ( disp, disp, 5 );
	short mi, ma;
	minmax<short>(disp, mi, ma);

	cout<<" Minimum value of disp is "<<mi<<" Maximum value of disp is "<<ma<<endl;

	Mat disp8U = Mat(im1.rows, im1.cols, CV_8UC1);
	normalize(disp, disp8U, 0, 255, CV_MINMAX, CV_8UC1);
	

	double *hist;
	double *bins;

	bool histFlag = 1;
	if(histFlag)
		histogram<short>(disp,hist,bins, 100);

	Mat depth;
	float* p;
	//float baselineF = 22500;
	float baselineF;
	if(para.getBaselineFocus())
		baselineF = para.getBaselineFocus();
	else
		baselineF = 22500;
	cout<<"Base line is"<<baselineF<<endl;
	disp.convertTo(depth, CV_32FC1);
	for(int i = 0; i < depth.rows; i++){
		p = depth.ptr<float>(i);
		for(int j = 0; j < depth.cols; j++){
			if(p[j] < 0.000001 && p[j] > -0.000001)
				p[j] =10000;
			p[j] = baselineF/p[j];

		}
	}
	float min, max;
	minmax<float>(depth, min, max);
	double *hh;
	double *bb;
	shiftAboveZero<float>(depth);
	minmax<float>(depth, min, max);
	histogram<float>(depth, hh, bb, 1000);
	cout<<"Minimum value of depth is "<<min<<" Maximum value of depth is "<<max<<endl;
	cout<<"The size of depth is "<<depth.size()<<endl;
	cout<<"The type of depth is "<<depth.type()<<endl;
//	cout<<depth<<endl;
//	shiftAboveZero<float>(depth);
//	medianBlur(depth, depth, 7);
	Mat depth8U;
	normalize(depth, depth8U, 0, 255, CV_MINMAX, CV_8UC1);
//	Mat depthLowP;
//	medFilter(depth8U, depthLowP, 11);
	//cout<<depth8U<<endl;
	string out = para.getOutput();
	char cstr[out.size()];
	out.copy(cstr, out.size()+1);
	cstr[out.size()] = '\0';
	auto chh = mkdir(cstr, 0777);
	cout<<"out put of mkdir id "<<chh<<endl;
	cout<<"c string is"<<out.c_str()<<endl;
	imwrite(out + "/Depth.jpg", depth8U);
	imwrite(out + "/Original.jpg", im2);
	namedWindow("Example");
	while(1){
		imshow("Example", depth8U);
		if(waitKey(1000) == 27) break;
		imshow("Example", disp8U);
		if(waitKey(1000) == 27) break;
		imshow("Example", im1);
		if(waitKey(1000) == 27) break;
		imshow("Example", im2);
		if(waitKey(1000) == 27) break;
	}
	destroyWindow("Example");
	return 0;
}
