#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat imgHSV, imgMask;

int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;

VideoCapture cap(0);
Mat img;

void onTrackbarChange(int, void*) {
    // This function is a callback for trackbar events, it will be called whenever trackbar values change
}

int main() {
    if (!cap.isOpened()) {
        cerr << "Error: Camera not opened!" << endl;
        return -1;
    }

    // Create a Window for trackbars
    namedWindow("Trackbars", WINDOW_AUTOSIZE);

    // Create trackbars
    createTrackbar("Hue Min", "Trackbars", &hmin, 179, onTrackbarChange);
    createTrackbar("Hue Max", "Trackbars", &hmax, 179, onTrackbarChange);
    createTrackbar("Sat Min", "Trackbars", &smin, 255, onTrackbarChange);
    createTrackbar("Sat Max", "Trackbars", &smax, 255, onTrackbarChange);
    createTrackbar("Val Min", "Trackbars", &vmin, 255, onTrackbarChange);
    createTrackbar("Val Max", "Trackbars", &vmax, 255, onTrackbarChange);

    while (true) {
        if (!cap.read(img)) {
            cerr << "Error: Failed to capture image!" << endl;
            break;
        }

        // Convert to HSV color space
        cvtColor(img, imgHSV, COLOR_BGR2HSV);

        // Set the range of color to detect
        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);

        // Threshold the HSV image to get the color within the range
        inRange(imgHSV, lower, upper, imgMask);

        // Display the results
        imshow("Image", img);
        imshow("Mask", imgMask);

        // Exit on 'ESC' key
        if (waitKey(1) == 27) {
            break;
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
