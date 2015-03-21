#include "EyeTracker.h"

void EyeTracker::SetFaceCascade(cv::CascadeClassifier faceCas)
{
	m_faceCascade = faceCas;
}

void EyeTracker::SetEyeCascade(cv::CascadeClassifier eyeCas)
{
	m_eyeCascade = eyeCas;
}

void EyeTracker::InitTracker()
{
	//Open webcam
	try
	{
		// Read the video stream
		m_pCapture = cvCaptureFromCAM(0);

		// Load the cascades
		if (!m_faceCascade.load(face_cascade_name))
		{
			printf("--(!)Error loading face cascade, please change face_cascade_name in source code.\n");
			InitTracker();
		};

		//cv::namedWindow(main_window_name, CV_WINDOW_NORMAL);
		//cv::moveWindow(main_window_name, 400, 100);
		cv::namedWindow(face_window_name, CV_WINDOW_NORMAL);
		cv::moveWindow(face_window_name, 10, 100);
		//cv::namedWindow("Right Eye", CV_WINDOW_NORMAL);
		//cv::moveWindow("Right Eye", 10, 600);
		//cv::namedWindow("Left Eye", CV_WINDOW_NORMAL);
		//cv::moveWindow("Left Eye", 10, 800);
		//cv::namedWindow("aa", CV_WINDOW_NORMAL);
		//cv::moveWindow("aa", 10, 800);
		//cv::namedWindow("aaa", CV_WINDOW_NORMAL);
		//cv::moveWindow("aaa", 10, 800);

		createCornerKernels();
		ellipse(skinCrCbHist, cv::Point(113, 155.6), cv::Size(23.4, 15.2), 43.0, 0.0, 360.0, cv::Scalar(255, 255, 255), -1);

		if (!m_pCapture)
			throw 'E';

		//Set video to 320x240
		//m_capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
		//m_capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	}
	catch (char e)
	{
		printf("Webcam not found\n");
		//exit(0);
	}

}

void EyeTracker::StartCalibration()
{
	//Detection stage
	//Try to detect the face and the eye of the user
	//DetectEye(m_gray, m_eyeTemplate, m_eyeBoundingBox);
	//DetectAndDisplay(m_frame);
	m_calibrationSuccess = true;
}

void EyeTracker::Calibrate()
{
	//Find distance between eye and eye corner.
	double distance = 0;
	cv::Point a = m_eyeCentre;
	cv::Point b = m_eyeCorner;
	m_initEyeCentre = a;
	distance = sqrt(((a.x * a.x) - (b.x * b.x)) + ((a.y * a.y) - (b.y * b.y)));

	m_InitialDistEyeCentreEyeCorner = distance;
	std::cout << "\nCalibrated initial eye coords: " << m_initEyeCentre <<"\n";
}

void EyeTracker::UpdateTracker()
{
	//m_pCapture >> m_frame;

	//if (m_frame.empty())
	//	InitTracker();

	//if (m_eyeBoundingBox.width == 0 && m_eyeBoundingBox.height == 0)
	//	m_calibrationSuccess = false;


	////Flip the frame horizontally.
	//cv::flip(m_frame, m_frame, 1);

	////Convert to grayscale and adjust the image contrast using histogram equalization
	//cv::cvtColor(m_frame, m_gray, CV_BGR2GRAY);

	////Tracking stage with template matching
	//TrackEye(m_gray, m_eyeTemplate, m_eyeBoundingBox);

	////Draw bounding rectangle for the eye
	//cv::rectangle(m_frame, m_eyeBoundingBox, CV_RGB(0, 255, 0));

	//cv::imshow("Video", m_frame);

	
	if (m_pCapture)
	{
		//while (true) 
		//{
			m_frame = cvQueryFrame(m_pCapture);
			// mirror it
			cv::flip(m_frame, m_frame, 1);
			m_frame.copyTo(debugImage);

			// Apply the classifier to the frame
			if (!m_frame.empty()) 
			{
				DetectAndDisplay(m_frame);
				//return true;
			}
			else 
			{
				printf(" --(!) No captured frame -- Break!");
				//return false;
				//break;
			}

			//imshow(main_window_name, debugImage);
			
			int c = cv::waitKey(10);
			
			if ((char)c == 'c') 
			{ 
				//break; 
			}
			if ((char)c == 'f') 
			{
				cv::imwrite("frame.png", debugImage);
			}

		//}
	}
	else
	{
		InitTracker();
		//return false;
	}

}

void EyeTracker::findEyes(cv::Mat frame_gray, cv::Rect face) 
{
	cv::Mat faceROI = frame_gray(face);
	cv::Mat debugFace = faceROI;

	if (kSmoothFaceImage) {
		double sigma = kSmoothFaceFactor * face.width;
		GaussianBlur(faceROI, faceROI, cv::Size(0, 0), sigma);
	}
	//-- Find eye regions and draw them
	int eye_region_width = face.width * (kEyePercentWidth / 100.0);
	int eye_region_height = face.width * (kEyePercentHeight / 100.0);
	int eye_region_top = face.height * (kEyePercentTop / 100.0);
	//cv::Rect leftEyeRegion(face.width*(kEyePercentSide / 100.0),
		//eye_region_top, eye_region_width, eye_region_height);
	cv::Rect rightEyeRegion(face.width - eye_region_width - face.width*(kEyePercentSide / 100.0),
		eye_region_top, eye_region_width, eye_region_height);

	//-- Find Eye Centers
	//cv::Point leftPupil = findEyeCenter(faceROI, leftEyeRegion, "Left Eye");
	cv::Point rightPupil = findEyeCenter(faceROI, rightEyeRegion, "Right Eye");
	m_eyeCentre = rightPupil;
	// get corner regions
	//cv::Rect leftRightCornerRegion(leftEyeRegion);
	//leftRightCornerRegion.width -= leftPupil.x;
	//leftRightCornerRegion.x += leftPupil.x;
	//leftRightCornerRegion.height /= 2;
	//leftRightCornerRegion.y += leftRightCornerRegion.height / 2;
	//cv::Rect leftLeftCornerRegion(leftEyeRegion);
	//leftLeftCornerRegion.width = leftPupil.x;
	//leftLeftCornerRegion.height /= 2;
	//leftLeftCornerRegion.y += leftLeftCornerRegion.height / 2;
	//cv::Rect rightLeftCornerRegion(rightEyeRegion);
	//rightLeftCornerRegion.width = rightPupil.x;
	//rightLeftCornerRegion.height /= 2;
	//rightLeftCornerRegion.y += rightLeftCornerRegion.height / 2;
	cv::Rect rightRightCornerRegion(rightEyeRegion);
	rightRightCornerRegion.width -= rightPupil.x;
	rightRightCornerRegion.x += rightPupil.x;
	rightRightCornerRegion.height /= 2;
	rightRightCornerRegion.y += rightRightCornerRegion.height / 2;
	//rectangle(debugFace, leftRightCornerRegion, 200);
	//rectangle(debugFace, leftLeftCornerRegion, 200);
	//rectangle(debugFace, rightLeftCornerRegion, 200);
	rectangle(debugFace, rightRightCornerRegion, 200);
	// change eye centers to face coordinates
	rightPupil.x += rightEyeRegion.x;
	rightPupil.y += rightEyeRegion.y;
	//leftPupil.x += leftEyeRegion.x;
	//leftPupil.y += leftEyeRegion.y;
	// draw eye centers
	circle(debugFace, rightPupil, 3, 1234);
	//circle(debugFace, leftPupil, 3, 1234);

	//-- Find Eye Corners
	if (kEnableEyeCorner) 
	{
		//cv::Point2f leftRightCorner = findEyeCorner(faceROI(leftRightCornerRegion), true, false);		
		//leftRightCorner.x += leftRightCornerRegion.x;
		//leftRightCorner.y += leftRightCornerRegion.y;
		//cv::Point2f leftLeftCorner = findEyeCorner(faceROI(leftLeftCornerRegion), true, true);
		//leftLeftCorner.x += leftLeftCornerRegion.x;
		//leftLeftCorner.y += leftLeftCornerRegion.y;
		//cv::Point2f rightLeftCorner = findEyeCorner(faceROI(rightLeftCornerRegion), false, true);
		//rightLeftCorner.x += rightLeftCornerRegion.x;
		//rightLeftCorner.y += rightLeftCornerRegion.y;
		cv::Point2f rightRightCorner = findEyeCorner(faceROI(rightRightCornerRegion), false, false);
		rightRightCorner.x += rightRightCornerRegion.x;
		rightRightCorner.y += rightRightCornerRegion.y;
		//circle(faceROI, leftRightCorner, 3, 200);
		//circle(faceROI, leftLeftCorner, 3, 200);
		//circle(faceROI, rightLeftCorner, 3, 200);
		circle(faceROI, rightRightCorner, 3, 200);
		m_eyeCorner = rightRightCorner;
	}

	//imshow(main_window_name, faceROI);
	imshow(face_window_name, faceROI);
	//cv::Rect roi( cv::Point( 0, 0 ), faceROI.size());
	//cv::Mat destinationROI = debugImage( roi );
	//faceROI.copyTo( destinationROI );
}


cv::Mat EyeTracker::findSkin(cv::Mat &frame) 
{
	cv::Mat input;
	cv::Mat output = cv::Mat(frame.rows, frame.cols, CV_8U);

	cvtColor(frame, input, CV_BGR2YCrCb);

	for (int y = 0; y < input.rows; ++y) {
		const cv::Vec3b *Mr = input.ptr<cv::Vec3b>(y);
		//    uchar *Or = output.ptr<uchar>(y);
		cv::Vec3b *Or = frame.ptr<cv::Vec3b>(y);
		for (int x = 0; x < input.cols; ++x) {
			cv::Vec3b ycrcb = Mr[x];
			//      Or[x] = (skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) > 0) ? 255 : 0;
			if (skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) == 0) {
				Or[x] = cv::Vec3b(0, 0, 0);
			}
		}
	}
	return output;
}

/**
* @function detectAndDisplay
*/
void EyeTracker::DetectAndDisplay(cv::Mat frame)
{
	std::vector<cv::Rect> faces;
	//cv::Mat frame_gray;

	std::vector<cv::Mat> rgbChannels(3);
	cv::split(frame, rgbChannels);
	cv::Mat frame_gray = rgbChannels[2];

	//cvtColor( frame, frame_gray, CV_BGR2GRAY );
	//equalizeHist( frame_gray, frame_gray );
	//cv::pow(frame_gray, CV_64F, frame_gray);
	//-- Detect faces
	m_faceCascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(150, 150));
	//  findSkin(debugImage);

	for (int i = 0; i < faces.size(); i++)
	{
		rectangle(debugImage, faces[i], 1234);
	}
	//-- Show what you got
	if (faces.size() > 0)
	{
		findEyes(frame_gray, faces[0]);
	}
}


