#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    using namespace cv;
    VideoCapture vc(0);
    if (!vc.isOpened() ) return 1;
    cv::Mat frame;

    cv::namedWindow("Window", WINDOW_AUTOSIZE); // Create a window for display.
    int i = 0;
    while (waitKey(10) != 27) {
        vc >> frame;
        std::cout << "frame info: " << frame.rows << " " << frame.cols << std::endl;
        flip(frame, frame, 1);
//        imwrite("frame" + std::to_string(i++) + ".png", frame);
//         (this does not work on Windows)
        cv::imshow("Frame", frame);

    }
    return 0;
}