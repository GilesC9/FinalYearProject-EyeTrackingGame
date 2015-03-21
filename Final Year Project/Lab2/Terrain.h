#ifndef _TERRAIN_INC_
#define _TERRAIN_INC_


class Terrain
{
	// simple terrain class - contains a floating point array of heights, and a function to create an OpenSceneGraph Geode
public:
	Terrain() // default constructor
	{
		m_heightData = NULL;
		m_nx = 0;
		m_ny = 0;
		m_delta = 0.0f;
	}

	~Terrain() // destructor
	{
		if ( m_heightData != NULL )
			delete m_heightData;
	}

	int CreateFromBitmap( char *bmpFile, float heightScale, float delta );
	void CreateOSGGeode(osg::Geode* pGeode);

	//Utility
	void LoadShaderSource( osg::Shader* shader, const std::string& fileName );
	float HeightAtPoint(float xPos, float yPos);
	osg::Vec3 NormalAtPoint(float xPos, float yPos);
	osg::Vec3 SmoothNormalAtPoint(float xPos, float yPos);

	//Getters
	float GetX0(void) { return m_x0; };
	float GetY0(void) { return m_y0; };
	float GetX1(void) { return m_x1; };
	float GetY1(void) { return m_y1; };
	float GetDelta(void) { return m_delta; };
private:
	float *m_heightData; // array of height values
	osg::Vec3 *m_normalData;
	int m_nx; // number of vertices in the x direction
	int m_ny; // number of vertices in the y direction
	float m_delta; // distance between vertices
	float scaleFactor; //Height scale factor

	float m_x0, m_x1, m_y0, m_y1; // x, y extents of the terrain
};

#endif // ifndef _TERRAIN_INC_