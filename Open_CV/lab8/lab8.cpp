#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    using namespace std;
    using namespace cv;

    array<int,3> lower = {123,124,88};
    array<int,3> upper = {178,255,255};

    VideoCapture vc(0);
    if (!vc.isOpened() ) return 1;
    cv::Mat frame, dst, pierwszy;

    namedWindow("detected",  WINDOW_AUTOSIZE);
    namedWindow("pierwszy",  WINDOW_AUTOSIZE);

    createTrackbar("lh", "detected", &lower[0], 255);
    createTrackbar("ls", "detected", &lower[1], 255);
    createTrackbar("lv", "detected", &lower[2], 255);
    createTrackbar("hh", "detected", &upper[0], 255);
    createTrackbar("hs", "detected", &upper[1], 255);
    createTrackbar("hv", "detected", &upper[2], 255);

    while (waitKey(10) != 27) {
        vc >> frame;
        flip(frame, frame, 1);
        cvtColor(frame, dst,COLOR_BGR2HSV);
//        imshow("Frame", frame);
//        imshow("dst", dst);
        inRange(dst, lower,upper, pierwszy);
        auto kern = getStructuringElement(MORPH_ELLIPSE, {5, 5});
        morphologyEx(pierwszy, pierwszy, MORPH_CLOSE, kern);
        morphologyEx(pierwszy, pierwszy, MORPH_OPEN, kern);
        imshow("pierwszy", pierwszy);

        vector<vector<Size>> contours;
        vector<Vec4i> hierarchy;
        findContours (pierwszy, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);
//        cout << contours.size() << endl;
        auto selected = *std::max_element(contours.begin(), contours.end(),
                                          [](auto a, auto b){
            return contourArea(a) < contourArea(b);});
        cout << " " << selected[0] << endl;
        vector<vector<Size>> c = {selected};
        drawContours(frame,c,0,{255,0,0}, 2);
        imshow("frame",frame);
    }
    return 0;
}