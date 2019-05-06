// HW2 Prob.1.(b)
// canny edge detector
// Input & Usage: need to run on a computer with OpenCV environment


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

//![variables]
Mat src, src_gray;
Mat dst, detected_edges;

int lowThreshold = 60;
const int max_lowThreshold = 180;
const int ratio = 3;
const int kernel_size = 3;
const char* window_name = "Pig Edge Map";
//![variables]

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
static void CannyThreshold(int, void*)
{
    blur( src_gray, detected_edges, Size(3,3) );

    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

    dst = Scalar::all(0);

    src.copyTo( dst, detected_edges);

    imshow( window_name, dst );

}
int main(int argc, char *argv[])
{
    src = imread("/Users/wenjun/ee569/hw_2_src/Pig.jpg");
    dst.create(src.size(), src.type());

    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    namedWindow(window_name, WINDOW_AUTOSIZE);

    createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

    CannyThreshold(0, 0);
    waitKey(0);

    return 0;
}
