#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, char **argv) {
    using namespace std;
    using namespace cv;

    array<int, 3> lower = {135, 145, 0};
    array<int, 3> upper = {255, 255, 255};

    VideoCapture vc(0);
    if (!vc.isOpened()) return 1;
    cv::Mat frame, dst, pierwszy;

    namedWindow("detected", WINDOW_AUTOSIZE);

    createTrackbar("lh", "detected", &lower[0], 255);
    createTrackbar("ls", "detected", &lower[1], 255);
    createTrackbar("lv", "detected", &lower[2], 255);
    createTrackbar("hh", "detected", &upper[0], 255);
    createTrackbar("hs", "detected", &upper[1], 255);
    createTrackbar("hv", "detected", &upper[2], 255);

    while (waitKey(10) != 27) {
        vc >> frame;
        flip(frame, frame, 1);
        cvtColor(frame, dst, COLOR_BGR2HSV);
        inRange(dst, lower, upper, pierwszy);
        auto kern = getStructuringElement(MORPH_ELLIPSE, {5, 5});
        morphologyEx(pierwszy, pierwszy, MORPH_CLOSE, kern);
        morphologyEx(pierwszy, pierwszy, MORPH_OPEN, kern);

        imshow("pierwszy", pierwszy);
        imshow("frame", frame);

        vector<vector<Size>> contours;
        findContours(pierwszy, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

        if (contours.size() >= 2) {

            sort(contours.begin(), contours.end(), [](vector<Size> a, vector<Size> b) {
                return contourArea(a) > contourArea(b);
            });

            double epsilon = 5;

            auto big_con = moments(contours[0], false);
            auto big_con_sec = moments(contours[1], false);


            int x_center_big_con = (int) (big_con.m10 / big_con.m00);
            int y_center_big_con = (int) (big_con.m01 / big_con.m00);

            int x_center_big_con_sec = (int) (big_con_sec.m10 / big_con_sec.m00);
            int y_center_big_con_sec = (int) (big_con_sec.m01 / big_con_sec.m00);

            Point big_p = {x_center_big_con, y_center_big_con};
            Point big_p_sec = {x_center_big_con_sec,y_center_big_con_sec};

            ellipse(frame, big_p, Size{10, 10}, 0.0, 0, 360, {0, 255, 0}, 3);
            ellipse(frame, big_p_sec, Size{10, 10}, 0.0, 0, 360, {0, 255, 0}, 3);

            imshow("frame", frame);

            if ((y_center_big_con - y_center_big_con_sec > -3) && (y_center_big_con - y_center_big_con_sec < 3) ) {
                line(frame, big_p, big_p_sec, {0,255,0}, 2);
                line(frame, Point (x_center_big_con, y_center_big_con + 10),
                     Point (x_center_big_con_sec, y_center_big_con_sec + 10), {255,0,0}, 2);
                line(frame, Point (x_center_big_con, y_center_big_con - 10),
                     Point (x_center_big_con_sec, y_center_big_con_sec - 10), {0,0,255}, 2);

                imshow("frame", frame);
            }
        }
    }
    return 0;
}