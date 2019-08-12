#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <plplot/plstream.h>
#include <plplot/plplot.h>
using namespace cv;
using namespace std;

template <class T>
void histogram(Mat &img, double *&hist, double* &bins, int noOfBins);

template <class T>
void shiftAboveZero(Mat &img);

template <class T>
void minmax(Mat& img, T &min, T &max);

template <class T>
void threshold(Mat &img, T lower, T upper, T value);

template <class T>
void histo(Mat &img, double * &hist, double * &bins);



template <class T>
void threshold(Mat &img, T lower, T upper, T value){
	T* p;
	T temp = 0;
	for(int i = 0; i < img.rows; i++){
		p = img.ptr<T>(i);
		for(int j = 0; j < img.cols; j++){
			if((p[j]  > lower) && (p[j] < upper))
				p[j] = value;
		}
	}
}

template <class T>
void histo(Mat &img, double * &hist, double * &bins){
	T temp = 0;
	T min = 0;
	T max = 0;

	T* p;
	for(int i = 0; i<img.rows; i++){
		p = img.ptr<T>(i);
		for(int j = 0; j<img.cols; j++){
			temp = *(p + j);
			if(temp < min)
				min = temp;
			if(temp > max)
				max = temp;
		}
	}
	const int sizeOfHist = (int)(max - min + 1);
	hist = new double[sizeOfHist];
	bins = new double[sizeOfHist];

	for(int i = 0; i < sizeOfHist; i++)
		hist[i] = 0;

	for(int i = 0; i<img.rows; i++){
		for(int j = 0; j<img.cols; j++){
			temp = *(img.ptr<uchar>(i) + j);
			temp = temp - min;
			hist[(int)temp] ++;		
		}
	}

	int fmin = 0;
	int fmax = 0;
	for(int i = 0; i < sizeOfHist; i++){
		if(hist[i] < min)
			fmin = hist[i];
		if(hist[i] > max)
			fmax = hist[i];
	}
	cout<<"min is "<<min<<endl;
	cout<<"max is "<<max<<endl;
	cout<<"fmin is "<<fmin<<endl;
	cout<<"fmax is "<<fmax<<endl;
	cout<<"hist[0]  is "<<hist[0]<<endl;

	plstream *pls = new plstream();
	pls->init();
	pls->env(min, max, fmin, fmax, 0, 0);
	pls->lab("Value", "Frequency","Histogram" );

	for(int i = 0; i < sizeOfHist; i++)
		bins[i] = i+min;

	pls->line(sizeOfHist, bins, hist);
	delete pls;
}
template <class T>
void minmax(Mat& img, T &min, T &max){
	T* p;
	min = 0;
	max = 0;
	for(int i = 0; i < img.rows; i++){
		p = img.ptr<T>(i);
		for(int j = 0; j < img.cols; j++){
			if(p[j] < min)
				min = p[j];
			if(p[j] > max)
				max = p[j];
		}
	}

}
template <class T>
void shiftAboveZero(Mat &img){
	T* p;
	T min, max;
	minmax(img, min, max);
	for(int i = 0; i < img.rows; i++){
			p = img.ptr<T>(i);
		for(int j = 0; j < img.cols; j++){
			p[j] = p[j] - min;
		}
	}
}

template <class T>
void histogram(Mat &img, double *&hist, double *&bins, int noOfBins){
	T* p;
	T val;
	T min, max;
	minmax(img, min, max);
	hist = new double[noOfBins];
	bins = new double[noOfBins];
	const T step = (max-min)/noOfBins;
	bins[0] = min;
	hist[0] = 0;
	for(int k = 1; k < noOfBins; k++){
		bins[k] = bins[k-1] + step;
		hist[k] = 0;
	}
	for(int i = 0; i < img.rows; i++){
		p = img.ptr<T>(i);
		for(int j = 0; j < img.cols; j++){
			val = p[j];
			for(int q = 0; q < noOfBins; q++){
				if((q+1) != noOfBins){
					if(val >= bins[q] && val < bins[q+1]){
						hist[q]++;
						break;
					}
				}else{
					hist[q]++;
				}
			}
		}
	}	
	double fmin = hist[0];
	double fmax = hist[0];
	for(int i = 0; i < noOfBins; i++)
		//cout<<"h["<<i<<"]"<<" = "<<hist[i]<<endl;
	for(int i = 0; i < noOfBins; i++){
		if (fmin > hist[i])
			fmin = hist[i];
		if(fmax < hist[i])
			fmax = hist[i];
	}
	plstream *pls = new plstream();
	pls->init();
	pls->env(min, max, fmin, fmax, 0, 0);
	pls->lab("Value", "Frequency","Histogram" );
	pls->line(noOfBins, bins, hist);
	delete pls;
}
