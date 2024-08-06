#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat img;
vector<vector<int>> newPoints;

// Define color ranges for detection (HSV values)
vector<vector<int>> myColors{ 
    {38, 43, 146, 144, 210, 232}, // Example color range 1 (HSV values)
    {71, 139, 75, 121, 255, 255}  // Example color range 2 (HSV values)
};

// Define colors for drawing (BGR values)
vector<Scalar> myColorValues{ 
    {0, 0, 255},   // Red color for drawing
    {0, 255, 255}  // Yellow color for drawing
};

// Function to detect contours and draw bounding boxes
Point getContours(Mat imgDil) {
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());
    Point myPoint(0, 0);

    for (int i = 0; i < contours.size(); i++) {
        int area = contourArea(contours[i]);
        if (area > 1000) {
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

            boundRect[i] = boundingRect(conPoly[i]);
            myPoint.x = boundRect[i].x + boundRect[i].width / 2; 
            myPoint.y = boundRect[i].y + boundRect[i].height / 2;

            rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
            drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
        }
    }
    return myPoint;
}

// Function to find colors in the image
vector<vector<int>> findColor(Mat img) {
    Mat imgHSV;
    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    vector<vector<int>> newPoints;
    
    for (int i = 0; i < myColors.size(); i++) {
        Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
        Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
        Mat imgMask;
        inRange(imgHSV, lower, upper, imgMask);
        imshow("Mask " + to_string(i), imgMask);

        Point myPoint = getContours(imgMask);
        if (myPoint.x != 0 && myPoint.y != 0) {
            newPoints.push_back({ myPoint.x, myPoint.y, i });  // Store the point and color index
        }
    }
    return newPoints;
}

// Function to draw detected points on the image
void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues) {
    for (int i = 0; i < newPoints.size(); i++) {
        circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorValues[newPoints[i][2]], FILLED);
    }
}

int main() {
    VideoCapture cap(0); // Open default camera

    while (true) {
        cap.read(img);

        if (img.empty()) {
            cerr << "Error: Unable to capture image" << endl;
            break;
        }

        newPoints = findColor(img);

        drawOnCanvas(newPoints, myColorValues);
        imshow("Image", img);

        if (waitKey(1) == 27) { // Exit on 'ESC' key
            break;
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
