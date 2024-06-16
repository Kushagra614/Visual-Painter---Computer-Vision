#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

Mat img;
vector<vector<int>> newPoints;
 
//hmin << "," << smin << "," << vmin << "," << hmax << "," << smax << "," << vmax
vector<vector<int>> myColors{ {38,43,146,144,210,232 },{71,139,75,121,255,255 } };// org

vector<Scalar> myColorValues{ {0,0,225} ,{255, 165, 0}  }; // organeg and blue


Point getContours(Mat imgDil)
{

	/*{{Point(20,30),Point(50,60)},
	{},
	{}
	}*/

	vector<vector<Point>> contours;
	vector<Vec4i>hierarchy; //-> Vec4i means it has 4 int values



	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 4); //-> -1 -> draw all the contours, then colour then thickness

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	string objType;
	Point myPoint(0, 0);

	//looping through all the countours 
	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;
		if (area > 1000) {


			//calculating the perimeter
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			//this will only draw for shapes with area above 1000
			//drawContours(img, conPoly,i, Scalar(255, 0, 255), 4); //-> -1 -> draw all the contours, then colour then thickness

			cout << conPoly[i].size() << endl;

			// we are going to find out the bounding box(making rectangles around the shapes
			boundRect[i] = boundingRect(conPoly[i]);
			myPoint.x = boundRect[i].x + boundRect[i].width / 2; 
			myPoint.y = boundRect[i].y;

			rectangle(img, boundRect[i].tl() /*-> tl mean top left */, boundRect[i].br() /*-> br mean bottom right */, Scalar(0, 255, 0), 5);
			drawContours(img, conPoly, i, Scalar(255, 0, 255), 4); //-> -1 -> draw all the contours, then colour then thickness
			
		}
	}
	return myPoint;
}
vector<vector<int>> findColor(Mat img)
{
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i = 0; i < myColors.size(); i++)
	{
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		Mat imgMask;
		inRange(imgHSV, lower, upper, imgMask);
		imshow(to_string(i), imgMask);

		Point myPoint = getContours(imgMask);
		if (myPoint.x != 0 && myPoint.y != 0) {
			newPoints.push_back({ myPoint.x, myPoint.y,i });  // i-> is the color
		}
	}
	return newPoints;
}

void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues)
{
	for (int i = 0; i < newPoints.size(); i++)
	{
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorValues[newPoints[i][2]], FILLED);
	}
}

void main() {

	VideoCapture cap(0); // 0 -> It is using default Webcam
	

	while (true)
	{
		cap.read(img);

		newPoints = findColor(img);

		drawOnCanvas(newPoints, myColorValues);
		imshow("image", img);
		waitKey(1);
	}

	
}