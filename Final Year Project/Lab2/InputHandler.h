#ifndef _INPUT_HANDLER_INC_
#define _INPUT_HANDLER_INC_

#include "stdafx.h"

class InputHandler : public osgGA::GUIEventHandler
{

private:

	float terrainOffset;
	float m_fMouseX;
	float m_fMouseY;
	float m_fLastMouseX;
	float m_fLastMouseY;
	int m_bMouseDown;
	// bitmap with flags for directional keys (see DIRECTIONS enum)
	int m_bDirections;
	// bitmap with flags for action buttons (see BUTTONS enum)
	int m_bButtons;

public:

	enum
	{
		FORWARD = 1,
		BACK = 2,
		RIGHT = 4,
		LEFT = 8
	} DIRECTIONS;

	enum
	{
		ADD_OBJECT = 1,
		REMOVE_OBJECT = 2,
		FIRE = 4,
		RESET = 8,
		TRACKBALL = 10,
		FIRSTPERSON = 12
	} BUTTONS;

	InputHandler()
	{
		m_fMouseX = 0.0f;
		m_fMouseY = 0.0f;
		m_bMouseDown = 0;
		m_bDirections = 0;
		m_bButtons = 0;
	}
	~InputHandler() {}

	void GetUpdate(float &fDeltaX, float &fDeltaY, float &fAbsX, float &fAbsY, int &bMouseButton, int &bDirections, int &bButtons)
	{
		fDeltaX = m_fMouseX - m_fLastMouseX;
		fDeltaY = m_fMouseY - m_fLastMouseY;
		fAbsX = m_fLastMouseX;
		fAbsY = m_fLastMouseY;

		m_fLastMouseX = m_fMouseX;
		m_fLastMouseY = m_fMouseY;

		bMouseButton = m_bMouseDown;
		bDirections = m_bDirections;
		bButtons = m_bButtons;
		m_bButtons = 0;
	}

	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		if (!viewer) return false;

		switch (ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::KEYDOWN) :
		{
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Up || ea.getKey() == 'w')
			{
				m_bDirections |= FORWARD;
			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Down || ea.getKey() == 's')
			{
				m_bDirections |= BACK;
			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Left || ea.getKey() == 'a')
			{
				m_bDirections |= LEFT;

			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Right || ea.getKey() == 'd')
			{
				m_bDirections |= RIGHT;

			}
			return false;
		}

		case(osgGA::GUIEventAdapter::KEYUP) :
		{
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Up || ea.getKey() == 'w')
			{
				m_bDirections &= ~FORWARD;
			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Down || ea.getKey() == 's')
			{
				m_bDirections &= ~BACK;
			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Left || ea.getKey() == 'a')
			{
				m_bDirections &= ~LEFT;

			}
			else if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Right || ea.getKey() == 'd')
			{
				m_bDirections &= ~RIGHT;
			}
			else if (ea.getKey() == 'z')
			{
				m_bButtons |= ADD_OBJECT;
			}
			else if (ea.getKey() == 'x')
			{
				m_bButtons |= REMOVE_OBJECT;
			}
			else if (ea.getKey() == 't')
			{
				m_bButtons |= TRACKBALL;
			}
			else if (ea.getKey() == 'y')
			{
				m_bButtons |= FIRSTPERSON;
			}
			else if (ea.getKey() == 'f')
			{
				m_bButtons |= FIRE;
			}
			else if (ea.getKey() == 'r')
			{
				m_bButtons |= RESET;
			}
			return false;

		}
		case(osgGA::GUIEventAdapter::MOVE) :
		case(osgGA::GUIEventAdapter::DRAG) :
		{
			m_fMouseX = ea.getX();
			m_fMouseY = ea.getY();
			return false;
		}

		case(osgGA::GUIEventAdapter::PUSH) :
		{
			m_bMouseDown = 1;
			return false;
		}

		case(osgGA::GUIEventAdapter::RELEASE) :
		{
			m_bMouseDown = 0;
			return false;
		}

		default:
			return false;
		}
	}
};

#endif // ifndef _INPUT_HANDLER_INC_