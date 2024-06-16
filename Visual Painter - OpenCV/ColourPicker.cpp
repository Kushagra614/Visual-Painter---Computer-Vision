#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

////////// COLOR DETECTION //////////

Mat imgHSV, imgMask; 



int hmin = 0, smin = 0, vmin = 0;
int hmax = 255, smax = 255, vmax = 255;

VideoCapture cap(0);
Mat img;


int main() {

	

	//Creating a Window for trackbars
	namedWindow("Trackbars", (640, 200));

	//creating taskbars
	createTrackbar("Hue Min", "Trackbars", &hmin, 179);
	createTrackbar("Hue Max", "Trackbars", &hmax, 179);
	createTrackbar("Sat Min", "Trackbars", &smin, 255);
	createTrackbar("Sat Max", "Trackbars", &smax, 255);
	createTrackbar("Val Min", "Trackbars", &vmin, 255);
	createTrackbar("Val Max", "Trackbars", &vmax, 255);


	while (true)
	{

		cap.read(img);


		//Converting to HSV color
		cvtColor(img, imgHSV, COLOR_BGR2HSV);

		// func to detect color-> inRange
		// lower and upper limits have been set for the rsnge of colour 
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		inRange(imgHSV, lower, upper, imgMask);

		cout << hmin << "," << smin << "," << vmin << "," << hmax << "," << smax << "," << vmax << endl;
		
		imshow("image", img);
		//imshow("image HSV", imgHSV);
		imshow("Mask", imgMask);

		waitKey(1);
	}

}