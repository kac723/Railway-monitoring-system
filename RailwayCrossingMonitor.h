#pragma once
#include "EventDetector.h"
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

//Class RailwayCrossingMonitor can define which event occured in image. This classes uses EventDetector object to do that
//Class stores 4 objects:
	//-input image (Mat type)
	//-vector of bools which stores information about which event occured in image (vector<bool> type)
	//-vector of ZoneParameters structure which describes parameters of each section (vector<ZoneParameters> type)
	//-name of image file (string type)
//This class implements 2 constructors, 1 destructor and 2 methods
	//-first constructor is default constructor which doesn't initalize any object of class and doesn't require any parameters
	//-second constructor takes as an input string and vector<ZoneParameters>. String if image file name and vector<ZoneParameters> defines parameters of zones
	//-empty destructor
	//-method showImageWithZones which doesn't require input parameters. This method displays input image with drawned zones on it and returns void.
	//-method isEvent which takes as an input integer n. Method returns bool value which defines if event n occured in image
class RailwayCrossingMonitor
{
private:
	cv::Mat inputImage;
	std::vector<bool> eventN;
	std::vector<ZoneParameters> zoneParams;
	std::string fileName;
public:
	RailwayCrossingMonitor(std::string file, std::vector<ZoneParameters> zp);
	bool isEventN(int n);
	void showImageWithZones();
	RailwayCrossingMonitor();
	~RailwayCrossingMonitor();
};

