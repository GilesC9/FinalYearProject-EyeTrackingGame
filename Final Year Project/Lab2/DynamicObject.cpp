#include "stdafx.h"
#include "DynamicObject.h"


//User data class for the scene graph update callback
class DynamicObjectUserData : public osg::Referenced
{
public:
	DynamicObject *pDynamicObject;
	//Default constructor
	DynamicObjectUserData(){};
	//Data constructor 
	DynamicObjectUserData(DynamicObject *pNewDynamicObject)
	{
		pDynamicObject = pNewDynamicObject;
	}
};

//Scene graph update callback
class DynamicObjectCallback : public osg::NodeCallback
{
public:

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::ref_ptr<DynamicObjectUserData> userData = dynamic_cast<DynamicObjectUserData*>(node->getUserData());
		if (userData)
		{
			//Update the node transform here
			osg::PositionAttitudeTransform *trans = dynamic_cast< osg::PositionAttitudeTransform*>(node);
			trans->setPosition(userData->pDynamicObject->m_pos);
		}
		//Continue traversing the graph so that OSG can process any other nodes with callbacks
		traverse(node, nv);
	}
};


DynamicObject::DynamicObject()
{
	//Create a transform node - it's up to any derived class to add geometry to it.
	m_pSceneGraphNode = new osg::PositionAttitudeTransform();
	m_pSceneGraphNode->setPosition(m_pos);
	//Set up the update callback
	m_pSceneGraphNode->setUserData(new DynamicObjectUserData(this));
	m_pSceneGraphNode->setUpdateCallback(new DynamicObjectCallback);
}

DynamicObject::~DynamicObject()
{

}

