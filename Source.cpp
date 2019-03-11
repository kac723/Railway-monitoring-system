//HOW TO RUN PROGRAM

//EXECUTABLE FILE REQUIRES ONE OR SEVERAL INPUT IMAGES AS PARAMETERS.
// TO RUN PROGRAM WITH ONE IMAGE PROVIDE ONE INPUT PARAMETER AS:
// PATH_TO_FILE\FILENAME.PNG
//EXAMPLE OF EXECUTION FOR ONE IMAGE:
// PROGRAM.EXE IMAGE1.JPG

// TO RUN PROGRAM WITH SEVERAL IMAGES PROVIDE ONE INPUT PARAMETER AS:
// PATH_TO_FOLDER\*PNG
// EXAMPLE OF EXECUTION FOR SEVERAL IMAGES IN FOLDER:
// PROGRAM.EXE FOLDER\*PNG

//Author: Kacper Grajcar
// Cranfield University
// March 2019

//This software was created using OpenCV library,
// which is open source library for computer vision
// under BSD license.
// In this software version 3.4.2 was used.
// https://docs.opencv.org/3.4.2/
//Furthermore, some sample codes were provided by Dr Zeeshan Rana.
//These samples included example of looping through all images in folder, testing if point is in particular area.
// Those examples were adpated into this code.

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include "EventDetector.h"
#include "RailwayCrossingMonitor.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	//array of Point type zones contains points in image which describe each created zone
	vector<vector<Point>> zones(10);
	//zoneParam vector of ZoneParameters conatains paramateters for each zone (zone Points, blurring kernel size, threshold parameters,...)
	vector<ZoneParameters> zoneParam(10);
	//input key from keyboard
	int key = 0;

	//ZONE A1 RAILWAY
	zones[0].push_back(Point(200, 461));
	zones[0].push_back(Point(286, 461));
	zones[0].push_back(Point(695, 190));
	zones[0].push_back(Point(632, 130));
	zones[0].push_back(Point(131, 396));

	zoneParam[0].adaptiveThresholdSize = 71;
	zoneParam[0].adaptiveThresholdConstant = 8;
	zoneParam[0].contourAreaThreshold = 0;
	zoneParam[0].pixelCountThreshold = 0;
	zoneParam[0].gaussianBlurSize = 0;
	zoneParam[0].zonePoints = zones[0];
	zoneParam[0].zoneColor = Scalar(0, 0, 255);
	zoneParam[0].zoneName = "Railway";

	//ZONE A2 TRAIN
	zones[1].push_back(Point(38, 458));
	zones[1].push_back(Point(223, 462));
	zones[1].push_back(Point(151, 388));

	zoneParam[1].adaptiveThresholdSize = 15;
	zoneParam[1].adaptiveThresholdConstant = 13;
	zoneParam[1].contourAreaThreshold = 0;
	zoneParam[1].pixelCountThreshold = 0;
	zoneParam[1].gaussianBlurSize = 0;
	zoneParam[1].zonePoints = zones[1];
	zoneParam[1].zoneColor = Scalar(0,0,255);
	zoneParam[1].zoneName = "Train";


	//ZONE B1
	zones[2].push_back(Point(24, 212));
	zones[2].push_back(Point(252, 130));
	zones[2].push_back(Point(93, 55));
	zones[2].push_back(Point(24, 55));

	zoneParam[2].adaptiveThresholdSize = 35;
	zoneParam[2].adaptiveThresholdConstant = 20;
	zoneParam[2].contourAreaThreshold = 1000;
	zoneParam[2].pixelCountThreshold = 600;
	zoneParam[2].gaussianBlurSize = 5;
	zoneParam[2].zonePoints = zones[2];
	zoneParam[2].zoneColor = Scalar(255, 0, 0);
	zoneParam[2].zoneName = "B1";
	//ZONE B2
	zones[3].push_back(Point(49, 207));
	zones[3].push_back(Point(220, 337));
	zones[3].push_back(Point(471, 210));
	zones[3].push_back(Point(270, 126));

	zoneParam[3].adaptiveThresholdSize = 81;
	zoneParam[3].adaptiveThresholdConstant = 40;
	zoneParam[3].contourAreaThreshold = 115;
	zoneParam[3].pixelCountThreshold = 220;
	zoneParam[3].gaussianBlurSize = 5;
	zoneParam[3].zonePoints = zones[3];
	zoneParam[3].zoneColor = Scalar(255, 0, 0);
	zoneParam[3].zoneName = "B2";

	//ZONE B3
	zones[4].push_back(Point(573, 275));
	zones[4].push_back(Point(690, 335));
	zones[4].push_back(Point(690, 196));

	zoneParam[4].adaptiveThresholdSize = 49;
	zoneParam[4].adaptiveThresholdConstant = 16;
	zoneParam[4].contourAreaThreshold = 1000;
	zoneParam[4].pixelCountThreshold = 450;
	zoneParam[4].gaussianBlurSize = 7;
	zoneParam[4].zonePoints = zones[4];
	zoneParam[4].zoneColor = Scalar(255, 0, 0);
	zoneParam[4].zoneName = "B3";

	//ZONE C1
	zones[5].push_back(Point(120, 55));
	zones[5].push_back(Point(267, 115));
	zones[5].push_back(Point(345, 85));
	zones[5].push_back(Point(275, 55));

	zoneParam[5].adaptiveThresholdSize = 21;
	zoneParam[5].adaptiveThresholdConstant = 30;
	zoneParam[5].contourAreaThreshold = 47;
	zoneParam[5].pixelCountThreshold = 146;
	zoneParam[5].gaussianBlurSize = 7;
	zoneParam[5].zonePoints = zones[5];
	zoneParam[5].zoneColor = Scalar(0, 255, 0);
	zoneParam[5].zoneName = "C1";

	//ZONE C2
	zones[6].push_back(Point(271, 116));
	zones[6].push_back(Point(376, 71));
	zones[6].push_back(Point(595, 141));
	zones[6].push_back(Point(476, 201));

	zoneParam[6].adaptiveThresholdSize = 31;
	zoneParam[6].adaptiveThresholdConstant = 40;
	zoneParam[6].contourAreaThreshold = 20;
	zoneParam[6].pixelCountThreshold = 50;
	zoneParam[6].gaussianBlurSize = 5;
	zoneParam[6].zonePoints = zones[6];
	zoneParam[6].zoneColor = Scalar(0, 255, 0);
	zoneParam[6].zoneName = "C2";

	//ZONE C3
	zones[7].push_back(Point(348, 430));
	zones[7].push_back(Point(385, 450));
	zones[7].push_back(Point(605, 301));
	zones[7].push_back(Point(571, 282));

	zoneParam[7].adaptiveThresholdSize = 21;
	zoneParam[7].adaptiveThresholdConstant = 13;
	zoneParam[7].contourAreaThreshold = 110;
	zoneParam[7].pixelCountThreshold = 380;
	zoneParam[7].gaussianBlurSize = 5;
	zoneParam[7].zonePoints = zones[7];
	zoneParam[7].zoneColor = Scalar(0, 255, 0);
	zoneParam[7].zoneName = "C3";

	//ZONE C4
	zones[8].push_back(Point(383, 461));
	zones[8].push_back(Point(615, 303));
	zones[8].push_back(Point(690, 340));
	zones[8].push_back(Point(690, 462));

	zoneParam[8].adaptiveThresholdSize = 81;
	zoneParam[8].adaptiveThresholdConstant = 20;
	zoneParam[8].contourAreaThreshold = 600;
	zoneParam[8].pixelCountThreshold = 1611;
	zoneParam[8].gaussianBlurSize = 5;
	zoneParam[8].zonePoints = zones[8];
	zoneParam[8].zoneColor = Scalar(0, 255, 0);
	zoneParam[8].zoneName = "C4";

	//Barrier zone
	zones[9].push_back(Point(31, 313));
	zones[9].push_back(Point(393, 181));
	zones[9].push_back(Point(263, 59));
	zones[9].push_back(Point(29, 58));

	zoneParam[9].adaptiveThresholdSize = 3;
	zoneParam[9].adaptiveThresholdConstant = 5;
	zoneParam[9].contourAreaThreshold = 600;
	zoneParam[9].pixelCountThreshold = 1611;
	zoneParam[9].gaussianBlurSize = 1;
	zoneParam[9].zonePoints = zones[9];
	zoneParam[9].zoneColor = Scalar(255, 255, 0);
	zoneParam[9].zoneName = "Barrier";

	//Object of class RailwayCrossingMonitor which will process current image and return information about event which occured in image
	RailwayCrossingMonitor RCM;
	//bool variable used to indicate if there i no even
	bool noEvent = true;

	//looping until all of images inputed are processed
	for(int i=1;i<argc;i++)
	{
		//setting up RailwayCrossingMonitor object with current image and zone parameters
		RCM = RailwayCrossingMonitor(argv[i], zoneParam);
		//displaying image name
		cout << argv[i] << " :" ;

		//vector of bool type which will store information about which event occured
		vector<bool> r(5, false);
		//looping to define which event occured
		for (int k = 4; k >= 0; k--)
		{
			r[k] = RCM.isEventN(k);
		}

		//printing results to console
		for (int l = 0; l < 5; l++)
		{
			if (r[l])
			{
				cout << " Event " << l + 1;
				//if at least one event occurred program changs noEvent flag
				noEvent = false;
			}
		}
		//output 0 to console if there was no event
		if (noEvent)
		{
			cout << " 0 ";
		}
		cout << endl;
		//reseting noEvent flag
		noEvent = true;
	}
	return 0;
}
