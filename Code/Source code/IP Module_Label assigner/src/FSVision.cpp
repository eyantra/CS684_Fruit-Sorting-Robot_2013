/*
 * FSVision.cpp
 *
 *  Created on: 31-Oct-2013
 *      Author: prpankajsingh
 */

#include "FSVision.h"
#include "cv.hpp"
#include "highgui/highgui.hpp"
#include "imgproc/imgproc.hpp"
#include <iostream>
using namespace std;
using namespace cv;
/*
string convertInt(int number)
{
	std::stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}
*/

FSVision::FSVision():cap(1) {

	if(!cap.isOpened()) // check if we succeeded
		{
			cout<<"camera is not opened...!!!";

		}
	// open the default cameraVideoCapture cap(1);
	CV_Assert(cap.isOpened());



}

FSVision::FSVision(int n):cap(n) {

	if(!cap.isOpened()) // check if we succeeded
		{
			cout<<"camera is not opened...!!!";

		}
	// open the default cameraVideoCapture cap(1);
	CV_Assert(cap.isOpened());



}


//returns Fruit's attributes after taking n images continuously from the camera...
FruitAttribute FSVision::getAttributeFromCamera(int n)
{
	FruitAttribute fa;
	double avg_mask_area=0;
	int avg_hue=0;
	int NO_OF_FRAMES_WITH_OBJECT=0;
	Mat frame;
	Mat circle_src, circle_src_gray;
	while(n>0){
		n--;
		cap>>frame;
		if(frame.empty())
		{
			CV_Assert(!frame.empty());
			break;
		}
		frame=frame.clone();
		//cvSmooth(frame, frame, CV_GAUSSIAN,5,5);
		//vector<int> compression_params; //vector that stores the compression parameters of the image
		Mat imgHSV;
		cvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
		Mat imgThresh1 =imgHSV.clone();
		Mat imgThresh;
		inRange(imgThresh1, Scalar(HUE_MIN_THRESH,SAT_MIN_THRESH,VAL_MIN_THRESH), Scalar(HUE_MAX_THRESH,SAT_MAX_THRESH,VAL_MAX_THRESH), imgThresh);
		//cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,7,7); //smooth the binary image using Gaussian kernel

		//imshow("Ball", imgThresh);

		// your input binary image
		// assuming that blob pixels have positive values, zero otherwise
		Mat binary_image=imgThresh.clone();

		//Commented as countor is not working properly....
		// threashold specifying minimum area of a blob
		double threshold = CONTOUR_AREA_THRESHHOLD;
		vector< vector< Point > > contours;
		vector<Vec4i> hierarchy;
		vector<int> big_blobs;
		double contour_area;
		Mat temp_image;
		// find all contours in the binary image
		binary_image.copyTo(temp_image);
		findContours(temp_image, contours, hierarchy, CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
		// Find indices of contours whose area is less than `threshold`
		if ( !contours.empty()) {
			//cout<<"\nNo.Of.Contours in Frame"<<i<<" is "<<contours.size()<<"\n";
			for (size_t i=0; i<contours.size(); ++i) {
				contour_area = contourArea(contours[i]) ;
				// cout<<contour_area<<"\t";
				if ( contour_area > threshold)
					big_blobs.push_back(i);
			}
		}

		// fill-in all small contours with zeros
		//Mat frameMat(cvGetSize(frame).height,cvGetSize(frame).width,1,Scalar(0));

		Mat mask = Mat::zeros(frame.size().height,frame.size().width, CV_8UC1);
		//Mat frame2(binary_image);
		//frame2(0);
		int mask_area=0;
		for (size_t i=0; i < big_blobs.size(); ++i) {
			drawContours(mask, contours, big_blobs[i], cv::Scalar(255),CV_FILLED, 8);
			//drawContours(frame1, contours, small_blobs[i], cv::Scalar(0,0,255),1, 8);
			// drawContours(frame2, contours, small_blobs[i], cv::Scalar(1),CV_FILLED, 8);
			mask_area=mask_area+contourArea(contours[big_blobs[i]]);
		}
		if(mask_area>0)
		{
			NO_OF_FRAMES_WITH_OBJECT++;
			avg_mask_area=((NO_OF_FRAMES_WITH_OBJECT-1)*avg_mask_area+mask_area)/NO_OF_FRAMES_WITH_OBJECT;
		}
		//smoothing imgHSV
		//smooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel
		GaussianBlur(imgThresh,imgThresh,Size(3,3),0);
		Mat imgHSVMat=imgHSV;
		Mat imgHSVROI;
		imgHSVMat.copyTo(imgHSVROI, mask);

		Mat frameMat=frame;
		Mat frameROI;
		frameMat.copyTo(frameROI, mask);
		//calculate histogram per frame
		vector<Mat> hsv_planes;
		//Experimrnt: commented split and now splitting imgHSVROI
		//split(imgHSV,hsv_planes);
		split(imgHSVROI,hsv_planes);
		int histSize=90;
		float range[]={0,90};
		const float* histRange = { range };
		bool uniform = true; bool accumulate = false;

		Mat h_hist;
		//Mat s_hist, v_hist;

		calcHist( &hsv_planes[0], 1, 0, mask, h_hist, 1, &histSize, &histRange, uniform, accumulate );
		//calcHist( &hsv_planes[1], 1, 0, mask, s_hist, 1, &histSize, &histRange, uniform, accumulate );
		//calcHist( &hsv_planes[2], 1, 0, mask, v_hist, 1, &histSize, &histRange, uniform, accumulate );

		// Draw the histograms for R, G and B
		//int hist_w = 512; int hist_h = 400;
		//int bin_w = cvRound( (double) hist_w/histSize );
		//index of max count in h_hist
		int maxh_ind=1;
		for(int i=1;i<h_hist.rows;i++)
		{
			if(cvRound(h_hist.at<float>(i))>cvRound(h_hist.at<float>(maxh_ind)))
				maxh_ind=i;
		}
		if(mask_area>0)
		{
			avg_hue=((NO_OF_FRAMES_WITH_OBJECT-1)*avg_hue+maxh_ind)/NO_OF_FRAMES_WITH_OBJECT;
		}

	}
	if(avg_mask_area>0){
		fa.area=avg_mask_area;
		fa.hueVal=avg_hue;
	}
	cout<<"\nHue Value is :"<<fa.hueVal<<" And Area value is "<<fa.area<<"\n";
	return fa;
}

