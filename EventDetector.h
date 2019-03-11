#pragma once
#include <opencv2/opencv.hpp>
#include "opencv2/highgui.hpp"
#include <iostream>

//ZoneParameters structure contains information about each zone, i.e.:
	//-value of adaptive threshold block size parameter used in adaptiveThreshold function (int type)
	//-value of adaptive threshold constant parameter used in adaptiveThreshold function (int type)
	//-threshold of area contour. If there is a contour which size if bigger than this parameter then proper flag is set (int type)
	//-threshold of pixe count in binnary image. If pixel count of current binnary image is higher than this parameter then proper flag is set (int type)
	//-size of gaussian blur kernel (int type)
	//-vector of Point type which stores coordinates of zone in image (vector<Point> type)
	//-color of zone border which can be drawn on image (Scalar type)
	//-name of zone (string type)
struct ZoneParameters
{
	int adaptiveThresholdSize;
	int adaptiveThresholdConstant;
	int contourAreaThreshold;
	int pixelCountThreshold;
	int gaussianBlurSize;
	std::vector<cv::Point> zonePoints;
	cv::Scalar zoneColor;
	std::string zoneName;
};

//class EventDetector can detect several events in image. Class stores private objects:
	//-input image (Mat type)
	//-red channel of input image (Mat type)
	//-grayscale input image (Mat type)
	//-binnary image obtained using adaptive threshold on red channel image(Mat type)
	//-binnary image obtained using adaptive threshold on grayscale image(Mat type)
	//-znoeParameters object which holds information about zone points, zone name, threshold parameters for Gaussian kernel, etc. (zoneParameters type)
//This class has 2 constructors, one default destructor and 7 methods:
	//-default empty constructor.
	//-default empty destructor.
	//-constructor which takes an input image (Mat type) and ZoneParameters object. This constructor initializes private objects of class and performs zone trimming.
	//-getMaxPoint method returns maxium x and y cooridnates from given vector<Point> input parameter (returns Point type).
	//-getMaxPoint method returns minimum x and y cooridnates from given vector<Point> input parameter (returns Point type).
	//-trimZone method trimms input image (Mat type) using zone points from ZoneParameters.zonePoints vector. Pixels which are not in zonePoints vector are set to 0 (black).
	//-railwayTrack method which returns true if railway track is clear of traffic or false if railway is not clear.
	//-train method returns true if train was detected in image and false if not.
	//-object method returns true if object was detected in zone image and false if not.
	//-barrier method returns true if barrier was detected in zone image and false if not.
class EventDetector
{
private:
	cv::Mat image;
	cv::Mat rImage;
	cv::Mat grayImage;
	cv::Mat rBin;
	cv::Mat grayBin;
	ZoneParameters zoneParamters;
public:
	EventDetector();
	~EventDetector();
	EventDetector(cv::Mat input,ZoneParameters zp);
	bool railwayTrack();
	bool train();
	bool object();
	bool barrier();
	cv::Point getMaxPoint(std::vector<cv::Point> points);
	cv::Point getMinPoint(std::vector<cv::Point> points);
	void trimZone(cv::Mat& input);
};

