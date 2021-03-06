#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
 
using namespace std;
using namespace cv;
 
int main()
{ 
      Mat src, dst;
      float sum;
 
      /// Load an image
      src = imread("cat.jpg", CV_LOAD_IMAGE_GRAYSCALE);
 
      if( !src.data )
      { return -1; }
 
      // define the kernel
      float Kernel[3][3] = {
                            {0, 1, 0},
                            {1, -4, 1},
                            {0, 1, 0}
                           }; 
        dst = src.clone();

        //convolution operation
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
 
        namedWindow("final");
        imshow("final", dst);
 
        namedWindow("initial");
        imshow("initial", src);
 
      waitKey();
 
 
    return 0;
}
