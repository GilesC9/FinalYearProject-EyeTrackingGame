#ifndef _DYNAMIC_OBJECT_INC
#define _DYNAMIC_OBJECT_INC

class DynamicObject
{
protected:
	//The transform node in the scene graph. Don't modify this to change the transform, 
	//Use a callback in the scenegraph update traversal instead.
	osg::ref_ptr<osg::PositionAttitudeTransform> m_pSceneGraphNode;
public:
	DynamicObject();
	~DynamicObject();

	//Position and orientation
	osg::Vec3 m_pos;
	osg::Quat m_rot;

	osg::ref_ptr<osg::PositionAttitudeTransform> GetSceneGraphNode() { return m_pSceneGraphNode; }
};
#endif