/*
 * FSVision.h
 *
 *  Created on: 31-Oct-2013
 *      Author: prpankajsingh
 */

#ifndef FSVISION_H_
#define FSVISION_H_
#include "cv.hpp"
#include "highgui/highgui.hpp"
#include "imgproc/imgproc.hpp"
#include "FruitAttribute.h"
#include <iostream>
#include "constants.h"


using namespace cv;

class FSVision {

public:
	VideoCapture cap;
	/**
	 * Constructor will throw cv_Error
	 * with code of 101 if camera is not initialized....
	 */
	FSVision();
	FSVision(int n);
	/**
	 * returns Fruit Attribute using camera .
	 * It uses #n number of frames to decide hue value,and area.........
	 * If there is no lemon then it will return default FruitAttribute with area=0 and hue =-1
	 * Use the above information to detect No lemon.......
	 */

	FruitAttribute getAttributeFromCamera(int n=10);
};

#endif /* FSVISION_H_ */
