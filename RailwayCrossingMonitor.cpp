#include "RailwayCrossingMonitor.h"

using namespace std;
using namespace cv;

RailwayCrossingMonitor::RailwayCrossingMonitor(string fileName, vector<ZoneParameters> zp)
{
	//initializing fileName string
	this->fileName = fileName;
	//reading image into Mat object
	inputImage = imread(this->fileName);
	//Validation of read
	if (inputImage.empty())
	{
		cout << "Error while opening file " << this->fileName << endl;
	}
	//initializing zoneParams object
	this->zoneParams = zp;
	//initializing eventN array
	this->eventN=vector<bool>(5,false);
}

void RailwayCrossingMonitor::showImageWithZones()
{
	//copying source image
	Mat imgWithZones = inputImage.clone();
	//Point pointer and int required to draw poly using polylines
	Point *pts;
	int npts;
	//looping until all zones are drawn
	for (int i = 0; i < ((this->zoneParams).size()); i++)
	{
		pts = (Point *)Mat(this->zoneParams[i].zonePoints).data;
		npts = Mat(this->zoneParams[i].zonePoints).rows;
		//drawing polygon on image
		polylines(imgWithZones, &pts, &npts, 1,true,this->zoneParams[i].zoneColor,1,CV_AA, 0);
	}
	//displaying image with zones drawn
	imshow(this->fileName, imgWithZones);
}
bool RailwayCrossingMonitor::isEventN(int n)
{
	//EventDetector object which defines which event occured in image
	EventDetector E;
	//intpu parameter n used in switch to assure proper pipeline of processing.
	//Each event has different pipeline
	switch (n)
		{
		//case 0 is event when railway is not clear of road/pedestrian traffic
		case 0:
		{
			//if train isn't detected on railway program detects if railway is clear of traffic
			if (!this->eventN[4])
			{
				//setting up EventDetector object with proper zoneParameters
				E = EventDetector(inputImage, this->zoneParams[0]);
				if (!E.railwayTrack())
				{
					//program triggers event 1 if railway is not clear
					this->eventN[0] = true;
				}
			}
			break;
		}
		case 1:
		{
			//if there is train on track program will only search for objects in zone B1
			if (eventN[4])
			{
				//setting up EventDetector object with proper zoneParameters
				E = EventDetector(inputImage,  this->zoneParams[2]);
				if (E.object())
				{
					//program triggers event 2 if there is object in zone B1
					this->eventN[1] = true;
				}
			}
			else //if there is no train program checks zones B2, B1, B3 (in that order)
			{
				//setting up EventDetector object with proper zoneParameters (B2 zone)
				E = EventDetector(inputImage, this->zoneParams[3]);
				//detection of object in zone B2 will be performed only is barrier is not deployed
				if (E.object() && !this->eventN[3])
				{
					this->eventN[1] = true;
				}
				//if barrier is not deployed program checks zones B1 and B3
				else
				{
					//setting up EventDetector object with proper zoneParameters (B1 zone)
					E = EventDetector(inputImage, this->zoneParams[2]);
					//checking if there is object in zone B1
					if (E.object())
					{
						this->eventN[1] = true;
					}
					else
					{
						//setting up EventDetector object with proper zoneParameters (B3 zone)
						E = EventDetector(inputImage,  this->zoneParams[4]);
						//checking if there is object in zone B3
						if (E.object())
						{
							this->eventN[1] = true;
						}
					}
				}
			}
			break;
		}
		case 2:
		{
			//detection of event 2 is performed only if there is no train on tracks
			if (!this->eventN[4])
			{
				//setting up EventDetector object with proper zoneParameters (C3 zone)
				E = EventDetector(inputImage,  this->zoneParams[7]);
				//checking if there is object in zone C3
				if (E.object())
				{
					this->eventN[2] = true;
				}
				else
				{
					//setting up EventDetector object with proper zoneParameters (C2 zone)
					E = EventDetector(inputImage,  this->zoneParams[6]);
					//detection of object in zone C2 is performed only if barrier is not deployed
					if (E.object() && !this->eventN[3])
					{
						this->eventN[2] = true;
					}
					else
					{
						//setting up EventDetector object with proper zoneParameters (C4 zone)
						E = EventDetector(inputImage,  this->zoneParams[8]);
						//checking if there is object in zone C4
						if (E.object())
						{
							this->eventN[2] = true;
						}
						else
						{
							//setting up EventDetector object with proper zoneParameters (C1 zone)
							E = EventDetector(inputImage,  this->zoneParams[5]);
							//detection of object in zone C1 is performed only if barrier is not deployed
							if (E.object() && !this->eventN[3])
							{
								this->eventN[2] = true;
							}
						}
					}
				}
			}
			break;
		}
		case 3:
		{
			//barrier detection will be performed only if there is no train
			if (!this->eventN[4])
			{
				// setting up EventDetector object with proper zoneParameters(barrier zone)
				E = EventDetector(inputImage,  this->zoneParams[9]);
				//checking if barrier is deployed
				if (E.barrier())
				{
					this->eventN[3] = true;
				}
			}		
			//if there is a train on track program assumes that barrier is deployed
			else
			{
				this->eventN[3] = true;
			}
			break;
		}
		case 4:
		{
			// setting up EventDetector object with proper zoneParameters(train zone)
			E = EventDetector(inputImage,  this->zoneParams[1]);
			//checking if there is a train on track
			if (E.train())
			{
				this->eventN[4] = true;
			}
			break;
		}
		default:
			break;
		}
	return this->eventN[n];
}

RailwayCrossingMonitor::RailwayCrossingMonitor()
{
}


RailwayCrossingMonitor::~RailwayCrossingMonitor()
{
}
