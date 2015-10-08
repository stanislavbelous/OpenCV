#include <cv.h>
#include <stdlib.h>
#include <stdio.h>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;
#define N 50

IplImage* image = 0;
IplImage* dst = 0;
IplImage* dst2 = 0;

unsigned long long cv_time[N];
unsigned long long my[N];

void get_time_diff (bool clock_round, const struct timespec *start,
               struct timespec *res)
{
        if (clock_round && clock_gettime (CLOCK_REALTIME, res) == 0) {
                if (res->tv_nsec < start->tv_nsec) {
                        res->tv_nsec = 1000000000 + res->tv_nsec - start->tv_nsec;
                        res->tv_sec -= 1 + start->tv_sec;
                } else {
                        res->tv_nsec -= start->tv_nsec;
                        res->tv_sec -= start->tv_sec;
                }


        } else {
                res->tv_nsec -= start->tv_nsec;
                res->tv_sec -= start->tv_sec;
        }
}

int time_measurement(unsigned long long time_seq[], string st)
{
    unsigned long long total;
    unsigned long long min, max;

    int i;

    total = 0;
    min = time_seq[0];
    max = time_seq[0];

    for(i = 0; i < N; i++) {
        total+=time_seq[i];
        if(time_seq[i] < min)
            min = time_seq[i];
        if(time_seq[i] > max)
            max = time_seq[i];
    }

    cout<< "total = " << total;
    cout<< "\nmean = " << (unsigned long long)(total/N);
    cout<< "\nmin = " << min;
    cout<< "\nmax = " << max << endl;

    return 0;
}

void testCVLaplace(char* filename, char* fileout) 
{
	struct timespec start, end;
	printf("%s", "CV Laplace");
	for(int i = 0; i < N; i++) 
	{
		image = cvLoadImage(filename, 1);
		dst = cvCreateImage( cvGetSize(image), IPL_DEPTH_16S, image->nChannels);
		dst2 = cvCreateImage( cvGetSize(image), image->depth, image->nChannels);

		assert( image != 0 );
	
		clock_gettime (CLOCK_REALTIME, &start);
		cvLaplace(image, dst, 3);
		get_time_diff(true, &start,&end);
		cv_time[i] = (unsigned long long )end.tv_nsec + (unsigned long long)(end.tv_sec * 1000000000);
	}	
	time_measurement(cv_time, "CV Laplace");
        cvConvertScale(dst, dst2);

	cvSaveImage(fileout, dst2, 0);

        cvWaitKey(0);

        cvReleaseImage(& image);
        cvReleaseImage(&dst);
        cvReleaseImage(&dst2);

        cvDestroyAllWindows();
}

void testMyLaplace(char* filename, char* fileout) 
{
	Mat src, dst;
	float sum;
	struct timespec start, end;
	printf("%s", "My");
	vector<int> params;
	params.push_back(0);

	for(int i = 0; i < N; i++) 
	{
		src = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);

		if( !src.data )
		{ return ; }
	
		float Kernel[3][3] = {
				    {0, 1, 0},
				    {1, -4, 1},
				    {0, 1, 0}
				   }; 
		dst = src.clone();
		clock_gettime (CLOCK_REALTIME, &start);

		for(int y = 1; y < src.rows - 1; y++){
		    for(int x = 1; x < src.cols - 1; x++){
			sum = 0.0;
			for(int k = -1; k <= 1;k++){
			    for(int j = -1; j <=1; j++){
				sum = sum + Kernel[j+1][k+1]*src.at<uchar>(y - j, x - k);
			    }
			}
			dst.at<uchar>(y,x) = sum;
		    }
		}
		get_time_diff(true, &start,&end);
 		my[i] = (unsigned long long )end.tv_nsec + (unsigned long long)(end.tv_sec * 1000000000);
	}
	time_measurement(my, "My filter");
	
	imwrite(fileout, dst, params);
}

int main(int argc, char* argv[]) 
{
	char* filename = argc >= 2 ? argv[1] : strdup("cat.jpg");
	char* fileoutLForLaplaceCV = strdup("cvLaplace.jpg");
	char* fileoutForMyFilter = strdup("myFilter.jpg");
	
	testCVLaplace(filename, fileoutLForLaplaceCV);
	testMyLaplace(filename, fileoutForMyFilter);
	
	return 0;
}
