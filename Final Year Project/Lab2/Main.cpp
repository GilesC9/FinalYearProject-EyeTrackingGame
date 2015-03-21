//Main entry
#include "EyeTracker.h"
#include "Game.h"
#include <iostream>

Game* GameEngine;
EyeTracker* ETracker;

int trackingData = 0; //Temp variable to psudo sake
char input;

cv::CascadeClassifier m_faceCascade, m_eyeCascade;
////Load cascade templates
//m_faceCascade.load("haarcascade_frontalface_alt2.xml");
//m_eyeCascade.load("haarcascade_eye.xml");

int main(int argc, char *argv[])
{
	ETracker = new EyeTracker();
	GameEngine = new Game();
	ETracker->SetFaceCascade(m_faceCascade);
	ETracker->SetEyeCascade(m_eyeCascade);

	ETracker->InitTracker(); //Setup webcam etc.

	printf("Press 'c' to calibrate");
	std::cin >> input;
	if (input == 'c')
	{
		ETracker->UpdateTracker();
		ETracker->Calibrate();
	}

	GameEngine->Start(ETracker);

	while (cv::waitKey(15) != 'q')
	{
		if (ETracker->IsCalibrationSuccess())
		{
			ETracker->UpdateTracker(); //Update tracking
			GameEngine->Update(ETracker->GetEyeCentreCoord(), ETracker->GetInitEyeCentre()); //Update game		;
		}
		else
			ETracker->StartCalibration();
	}

	ETracker->~EyeTracker();
	return 0;
}