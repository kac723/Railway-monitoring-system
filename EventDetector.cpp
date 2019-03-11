#include "EventDetector.h"

using namespace std;
using namespace cv;

//default constructor
EventDetector::EventDetector()
{
}

//default destructor
EventDetector::~EventDetector()
{
}

//overloaded constructor which takes as an input Mat object and ZoneParameters object.
EventDetector::EventDetector(Mat input, ZoneParameters zp)
{
	//assigning input ZoneParameters object to class zoneParameters object
	this->zoneParamters = zp;
	//creating input image based on input image and zoneParameters.zonePoints vector
	this->image = input(Range(getMinPoint(this->zoneParamters.zonePoints).y, getMaxPoint(this->zoneParamters.zonePoints).y), 
		Range(getMinPoint(this->zoneParamters.zonePoints).x, getMaxPoint(this->zoneParamters.zonePoints).x)).clone();

	//blurring zone image if zone is not railway or train
	if(this->zoneParamters.zoneName !="Railway" && this->zoneParamters.zoneName !="Train")
	{ 
		GaussianBlur(this->image, this->image, Size(this->zoneParamters.gaussianBlurSize, this->zoneParamters.gaussianBlurSize), 0);
	}

	//conversion to gray scale. Gray scale image will be stored in grayImage object
	cvtColor(this->image, this->grayImage, COLOR_BGR2GRAY);
	//extracting red channel and storing it in rImage object.
	extractChannel(this->image, this->rImage, 2);

	//trimming red channel image and gray scale image
	trimZone(this->rImage);
	trimZone(this->grayImage);

	//Histogram equalization is performed for railway and train zones
	if (this->zoneParamters.zoneName == "Railway" || this->zoneParamters.zoneName =="Train")
	{
		equalizeHist(this->grayImage, this->grayImage);
	}

	//Performing adaptive threshold on rImage and grayImage objects using zoneParameters parameters.
	adaptiveThreshold(rImage, this->rBin, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, zoneParamters.adaptiveThresholdSize, zoneParamters.adaptiveThresholdConstant);
	adaptiveThreshold(grayImage, this->grayBin, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, zoneParamters.adaptiveThresholdSize, zoneParamters.adaptiveThresholdConstant);

	//trimming zones after thresholding
	trimZone(this->grayBin);
	trimZone(this->rBin);
	
}

//method used for detection of object in image
bool EventDetector::object()
{
	//assigning thresholds from zoneParameters to local variables
	int contourAreaThreshold= this->zoneParamters.contourAreaThreshold;
	int pixelCountThreshold= this->zoneParamters.pixelCountThreshold;

	//arrays of Point which will store contour points for gray image and red channel image
	vector<vector<Point>> grayContours;
	vector<vector<Point>> redContours;

	//finding contours in rBin and grayBin object
	findContours(this->rBin, redContours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	findContours(this->grayBin, grayContours , CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	
	//flag which indicates if there was contour in gray scale image bigger than contourAreaThreshold
	bool contourAreaGrayFlag = false;
	//flag which indicates if amount of white pixels in gray sacle image is bigger than pixelCountThreshold
	bool pixelCountGrayFlag = false;

	//maximum contour size in gray image
	double maxContourGray = 0;

	//amount of white pixels in gray image
	double pixelCountGray = (cv::sum(grayBin)[0] / 255);

	//looping through all contours detected in gray scale image
	for (int i = 0; i < grayContours.size(); i++)
	{
		//if current contour size (area) is bigger than contourAreaThreshold value then proper flag (contourAreaGrayFlag) is set to true
		if (contourArea(grayContours[i]) > contourAreaThreshold)
		{
			contourAreaGrayFlag = true;	
		}
		//if current contour size is bigger than maximum contour program updates maximum contour value
		if (contourArea(grayContours[i]) > maxContourGray)
		{
			maxContourGray = contourArea(grayContours[i]);
		}
	}
	// if amount of white pixels in gray image is bigger than pixelCountThreshold value proper flag (pixelCountGrayFlag) is set
	if (pixelCountGray > pixelCountThreshold)
	{
		pixelCountGrayFlag = true;
	}

	//flag which indicates if there was contour in red channel image bigger than contourAreaThreshold
	bool contourAreaRedFlag = false;
	//flag which indicates if amount of white pixels in red channel image is bigger than pixelCountThreshold
	bool pixelCountRedFlag = false;
	
	//maximum contour size in red channel image
	double maxContourRed=0;

	//amount of white pixels in red channel image
	double pixelCountRed= (cv::sum(rBin)[0] / 255);

	//looping through all contours detected in red channel image
	for (int i = 0; i < redContours.size(); i++)
	{
		//if current contour size (area) is bigger than contourAreaThreshold value then proper flag (contourAreaRedFlag) is set to true
		if (contourArea(redContours[i]) > contourAreaThreshold)
		{
			contourAreaRedFlag = true;

		}
		//if current contour size is bigger than maximum contour program updates maximum contour value
		if (contourArea(redContours[i]) > maxContourRed)
		{
			maxContourRed = contourArea(redContours[i]);
		}
	}

	// if amount of white pixels in red channel image is bigger than pixelCountThreshold value proper flag (pixelCountRedFlag) is set
	if (pixelCountRed> pixelCountThreshold)
	{
		pixelCountRedFlag = true;
	}

	//decision pipeline
	//for zones different than B3 decision pipeline is different
	if (this->zoneParamters.zoneName != "B3")
	{
		//if there was contour in gray scale image bigger than contourAreaThreshold and amount of white pixels in gray scale image is bigger than pixelCountThreshold then program
		//deduces thath there was object in zone and returns true
		if (pixelCountGrayFlag && contourAreaGrayFlag) 
		{
			return true;
		}
		//if one of maximum contours in red channel image or gray scale image is bigger than 1.4*contourAreaThreshold and
		//amount of white pixels is bigger than pixelCountThreshold in one of images then program deduces that there was an object in zone and returns true
		else if ((maxContourGray > (1.4 * contourAreaThreshold) || maxContourRed > (1.4*contourAreaThreshold)) && (pixelCountRedFlag || pixelCountGrayFlag)) 
		{
			return true;
		}
		//if mixum contours in both gray scale and red channnel images were bigger than 1.4*contourAreaThreshold then program deduces that there was an object in zone and returns true
		else if ((maxContourGray > (1.4 * contourAreaThreshold) && maxContourRed > (1.4*contourAreaThreshold))) 
		{
			return true;
		}
		//if there was contour in red channel image bigger than contourAreaThreshold and amount of white pixels in red channel image is bigger than pixelCountThreshold then program
		//deduces thath there was object in zone and returns true
		else if (pixelCountRedFlag && contourAreaRedFlag) 
		{
			return true;
		}
		//if amount of white pixels in both red channel and gray scale images are bigger than 1.85*pixelCountThreshold then program deduces that there was an object in zone and returns true
		else if (pixelCountGray > 1.85*pixelCountThreshold && pixelCountRed > 1.85*pixelCountThreshold) 
		{
			return true;
		}
		//if non of conditions were satisfied then program returns false
		else
		{
			return false;
		}
	}
	//if current zone is B3 only pixelCountGrayFlag must be true.
	else
	{
		if (pixelCountGrayFlag) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

//barrier detection based on HoughLinesP and proper line properties filtering
bool EventDetector::barrier()
{
	//vector of Vec4i which will store information about detected lines
	vector<Vec4i> hLines;
	//finding lines using HoughLinesP. Parameters of function were obtained by experiments
	HoughLinesP(this->grayBin, hLines, 1, CV_PI / 180, 100, 10, 20);
	//angle of current line
	double lineAngle;
	//length of line
	double lineLength;
	//looping through all lines in order to determine if one lines fits line properties
	for (int il = 0; il < hLines.size(); il++)
	{
		//creating beginning point
		Point begin = Point(hLines[il][0], hLines[il][1]);
		//creating end point
		Point end = Point(hLines[il][2], hLines[il][3]);
		//calculating line angle based on beginning and end points
		lineAngle = atan2(begin.x - end.x, begin.y - end.y);
		//conversion from radians to degrees
		lineAngle = -lineAngle * 180 / CV_PI;
		//Point p used to calculate line length
		Point p = begin - end;
		//calculating line length
		lineLength = sqrt(p.ddot(p));
		//barrier line shall start within x coordinate lower than 15 and y cooridnate greater than 180. Additionally line angle should be between 10 and 90
		//degrees and line length should be greater than 250. if all of conditions are satisfied program returns true
		if(begin.x<15 && begin.y > 180 && lineAngle >10 && lineAngle < 90 && lineLength> 250)
		{
			return true;
		}
	}
	//returning false if there was no line of proper properties
	return false;
}
//method railwayTrack detects if railway is clear of traffic using HoughLines function
bool EventDetector::railwayTrack()
{
	// vector of Vec2f which will store information about detected lines
	vector<Vec2f> hLines;
	//detecting liines using HoughLines. Function parameter threshold (the las one) was obtained by experiments.
	HoughLines(this->grayBin, hLines, 1, CV_PI / 180, 550);
	//flag which defines if lower track is visible
	bool railwayLower = false;
	//flag whchi defines if upper track is visible 
	bool railwayUpper = false;
	//looping through all deteced lines to determine which of tracks (or both of them) appeared in image
	for (int il = 0; il < hLines.size(); il++)
	{
		// perpendicular  distance  in  pixels  from  top  left  corner  of  image  (origin  of  image)  to  the line
		double rho = hLines[il][0];
		//rho bigger tahn 320 means that lower track is visible
		if (rho > 320)
		{
			railwayLower = true;
		}
		//rho betwwen 255 and 270 mean that upper track is visible
		else if (rho > 255 && rho <270)
		{
			railwayUpper = true;
		}
	}
	//if both of tracks were detected then railway is clear of traffic and program returns true
	if (railwayLower && railwayUpper)
	{
		return true;
	}
	//if at least on track line wasn't detected then program returns false
	else
	{
		return false;
	}

}

//method for detection of train in image. Method uses HoughLinesP function to detect if track and railway lines are detected
bool EventDetector::train()
{
	//vector of Vec4i which will store information about detected railway lines
	vector<Vec4i> hLinesRailway;
	//vector of Vec4i which will store information about detected track lines
	vector<Vec4i> hLinesTrack;
	//detecting railway lines using HoughLinesP. Results will be stores in hLinesRailway
	HoughLinesP(this->grayBin, hLinesRailway, 1, CV_PI / 180, 45,15,20);
	//detecting track lines using HoughLinesP. Results will be stores in hLinesTrack
	HoughLinesP(this->grayBin, hLinesTrack, 1, CV_PI / 180, 75, 10, 100);

	//bool variable which defines if track is visible. 
	bool track = false;
	//bool variable which defines if railway is visible.
	bool railway = false;
	//angle of detected line
	double lineAngle;
	for (int il = 0; il < hLinesRailway.size(); il++)
	{
		//creating beginning point of track line
		Point begin = Point(hLinesRailway[il][0], hLinesRailway[il][1]);
		//creating end point of track line
		Point end = Point(hLinesRailway[il][2], hLinesRailway[il][3]);
		//calculating line angle based on beginning and end points
		lineAngle = atan2(begin.x - end.x, begin.y - end.y);
		//conversion from radians to degrees
		lineAngle = -lineAngle * 180 / CV_PI;
		//if line angle is between 55 and 65 program deduces that it is track and set track flag to true
		if (lineAngle <65 && lineAngle>55)
		{
			track = true;
		}
	}

	for (int il = 0; il < hLinesTrack.size(); il++)
	{
		//creating beginning point of railway line
		Point begin = Point(hLinesTrack[il][0], hLinesTrack[il][1]);
		//creating beginning point of railway line
		Point end = Point(hLinesTrack[il][2], hLinesTrack[il][3]);
		//calculating railway line angle based on beginning and end points
		lineAngle = atan2(begin.x - end.x, begin.y - end.y);
		//conversion from radians to degrees
		lineAngle = -lineAngle * 180 / CV_PI;
		//if railway line angle is between128 and 140 program deduces that it is railway and sets railway flag to true
		if (lineAngle >= 128 && lineAngle < 140 )
		{
			railway = true;
		}
	}
	//if railway is not visible program deduces thath train is railway and returns true
	if (!railway)
	{
		return true;
	}
	else
	{
		//if track is visible program deduces that there is no train and returns false
		if (track)
		{
			return false;
		}
		//if railway line is not visible and track line is also not visible program deduces thath there must be train of railway
		else
		{
			return true;
		}
	}
}

//method which find minimum x and y cooridnates in input vector of points
Point EventDetector::getMinPoint(vector<Point> points)
{
	//local variables which will store minimum x and y cooridnates
	int minX, minY;
	//initializing minX and minY as first Point in input vector<Point>
	minX = points[0].x;
	minY = points[0].y;
	//looping through points in vector
	for (int i = 1; i < points.size(); i++)
	{
		//upate minX value if current point x cooridnate is lower than minX
		if (points[i].x < minX)
		{
			minX = points[i].x;
		}
		//upate miny value if current point y cooridnate is lower than minY
		if (points[i].y < minY)
		{
			minY = points[i].y;
		}
	}
	//returning Point with minimum cooridantes
	return Point(minX, minY);
}
Point EventDetector::getMaxPoint(vector<Point> points)
{
	//local variables which will store maximum x and y cooridnates
	int maxX, maxY;
	//initializing maxX and maxY as first Point in input vector<Point>
	maxX = points[0].x;
	maxY = points[0].y;
	for (int i = 1; i < points.size(); i++)
	{
		//upate maxX value if current point x cooridnate is bigger than maxX
		if (points[i].x > maxX)
		{
			maxX = points[i].x;
		}
		//upate maxY value if current point y cooridnate is bigger than maxY
		if (points[i].y > maxY)
		{
			maxY = points[i].y;
		}
	}
	//returning Point with maximum cooridantes
	return Point(maxX, maxY);
}

//method which sets pixels outside of specific zone to black(0)
void EventDetector::trimZone(Mat &input)
{
	//assigning zonePoints attribute to local vector with points
	vector<Point> zoneTmp = this->zoneParamters.zonePoints;
	//subtracting minimum x and y cooridnates from each of points
	//this operation is performed in order to obtain the same zone in smaller image
	for (int i = 0; i < zoneTmp.size(); i++)
	{
		zoneTmp[i].x -= getMinPoint(this->zoneParamters.zonePoints).x;
		zoneTmp[i].y -= getMinPoint(this->zoneParamters.zonePoints).y;
	}
	//looping through all points in image
	for (int i = 0; i < input.rows; i++)
	{
		for (int j = 0; j < input.cols; j++)
		{
			//setting pixel to 0 (black) if it is outside of zoneTmp polygon
			//pointPolygonTest return -1 if point is outside of polygon. false flag is set in function as there is no need to calculate distance to point
			if (pointPolygonTest(zoneTmp, Point(j, i), false) < 0)
			{
				input.at<uchar>(i, j) = 0;
			}
		}
	}
}