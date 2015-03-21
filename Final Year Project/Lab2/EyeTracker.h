#ifndef _EYETRACKER
#define _EYETRACKER

#include <iostream>
#include <queue>
#include <stdio.h>
#include <math.h>
#include "stdafx.h"

#include "Constants.h"
#include "FindEyeCenter.h"
#include "FindEyeCorner.h"

class EyeTracker
{
private:
	/** Global variables */
	//-- Note, either copy these two files from opencv/data/haarscascades to your current folder, or change these locations
	cv::String face_cascade_name = "haarcascade_frontalface_alt.xml";
	std::string main_window_name = "Capture - Face detection";
	std::string face_window_name = "Capture - Face";
	//cv::RNG rng(12345);
	cv::Mat debugImage;
	cv::Mat skinCrCbHist = cv::Mat::zeros(cv::Size(256, 256), CV_8UC1);

	cv::CascadeClassifier m_faceCascade, m_eyeCascade;
	cv::Mat m_frame, m_gray, m_eyeTemplate; //Frame image captured from webcam
	cv::Rect m_eyeBoundingBox;
	cv::Point m_eyeCentre; //Current eye centre
	cv::Point m_eyeCorner;
	cv::Point m_initEyeCentre; //Initial calibrate eye centre

	CvCapture *m_pCapture;
	bool m_calibrationSuccess = false;
	double m_DistEyeCentreEyeCorner, m_InitialDistEyeCentreEyeCorner;


	int DetectEye(cv::Mat& image, cv::Mat& tpl, cv::Rect& rect); //Function to detect user's face and eye
	void TrackEye(cv::Mat& image, cv::Mat& tpl, cv::Rect& rect); //Function to track user's eye given its template

	void findEyes(cv::Mat frame_gray, cv::Rect face);
	cv::Mat findSkin(cv::Mat &frame);

public:
	EyeTracker()
	{	

	}

	~EyeTracker(){ releaseCornerKernels(); }

	bool IsCalibrationSuccess() { return m_calibrationSuccess; }


	void InitTracker(); //Will search for webcam, and start it up
	void StartCalibration();
	void Calibrate(); //Find various variables such as find distance between eye centre and eye corner.
	void UpdateTracker();

	void DetectAndDisplay(cv::Mat frame);

	void SetFaceCascade(cv::CascadeClassifier faceCas);
	void SetEyeCascade(cv::CascadeClassifier eyeCas);
	cv::Point GetEyeCentreCoord() { return m_eyeCentre; }
	cv::Point GetEyeCornerCoord() { return m_eyeCorner; }
	cv::Point GetInitEyeCentre() { return m_initEyeCentre; }
	double GetInitialEyeCentreCornerDistance() { return m_InitialDistEyeCentreEyeCorner; }
};


#endif

