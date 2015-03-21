#include "stdafx.h"
#include "Terrain.h"
#include <iostream>

int Terrain::CreateFromBitmap( char *bmpFile, float heightScale, float delta )
{
	// read a bmp file, and create a height array. 
	FILE *fp;

	fp = fopen( bmpFile, "rb" );

	unsigned char bmpFileHeader[14];
	unsigned char dibHeader[124];

	fread( bmpFileHeader, 14, 1, fp );

	// check the first two bytes
	if ( bmpFileHeader[0] != 0x42 && bmpFileHeader[1] != 0x4D )
	{
		fprintf(stderr, "%s does not seem to be a BMP file\n", bmpFile );
		return 0;
	}
	// read the first few bytes of the DIB (device independent bitmap) header
	fread( dibHeader, 4, 1, fp );
	int dibHeaderSize = *(int*)dibHeader;

	// read the rest of the DIB header
	fread( (dibHeader+4), dibHeaderSize-4, 1, fp );
	
	m_nx = *(int*)(dibHeader+4);
	m_ny = *(int*)(dibHeader+8);
	m_delta = delta;
	
	int bitsPerPixel = *(short int*)(dibHeader+14);

	unsigned char *imageData = new unsigned char[bitsPerPixel*m_nx*m_ny/8];

	fread( imageData, bitsPerPixel*m_nx*m_ny/8, 1, fp );
	fclose( fp );

	// now have the image data, convert it into an array of heights
	m_heightData = new float[m_nx*m_ny];

	int stride = 1;
	if ( bitsPerPixel == 24 )
		stride = 3;

	int i, j;

	scaleFactor = heightScale;

	for ( i = 0, j = 0; i < m_ny*m_ny; i++, j += stride )
	{
		m_heightData[i] = (float)imageData[j] * (1.0f / 255.0f) * scaleFactor;
	}

	return 1;
}

void Terrain::CreateOSGGeode(osg::Geode* pGeode)
{
	// create a geometry container
	osg::ref_ptr<osg::Geometry> pGeometry = new osg::Geometry;
	// create an array for the vertices
	osg::ref_ptr<osg::Vec3Array> pVertices = new osg::Vec3Array;
	// create an array for the normals
	osg::ref_ptr<osg::Vec3Array> pNormals = new osg::Vec3Array;
	// create an array for the triangle indices
	osg::DrawElementsUInt* triangles = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	//Create an array for UV coords
	osg::ref_ptr<osg::Vec2Array> pTexCoords = new osg::Vec2Array;
	//Create colour array
	osg::ref_ptr<osg::Vec4Array> pColors = new osg::Vec4Array;

	// calculate the centre and extents of the terrain
	osg::Vec3 centre = osg::Vec3( m_nx * m_delta * 0.5f,  m_ny * m_delta * 0.5f, 0.0f );

	m_x0 = -centre.x();
	m_y0 = -centre.y();
	m_x1 = m_x0 + (m_nx - 1) * m_delta;
	m_y1 = m_y0 + (m_ny - 1) * m_delta;


#pragma region Setup mesh vertices, normals, UVs
	for(int y = 0; y < m_ny; y++)
	{
		for(int x = 0; x < m_nx; x++)
		{
			osg::Vec4 vertColour;

			//Set vertices
			osg::Vec3 verts[1];
			verts[0] = osg::Vec3(m_x0 + x * m_delta, m_y0 + y * m_delta, m_heightData[y * m_nx + x]);

			pVertices->push_back(verts[0]);

			//Set normals
			if(x == 0 || y == 0 || m_nx-1 == 0 || m_ny == 0)
			{
				pNormals->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));

				//Set colour per vert
				vertColour = osg::Vec4(1.0, 0.0, 0.0, 0.0);
				pColors->push_back(vertColour);
			}
			else
			{			
				osg::Vec3 vXplus1 = osg::Vec3((m_x0 + x * m_delta)+1, m_y0 + y * m_delta, m_heightData[y * m_nx + (x+1)]);
				osg::Vec3 vXminus1 = osg::Vec3((m_x0 + x * m_delta)-1, m_y0 + y * m_delta, m_heightData[y * m_nx + (x-1)]);
				osg::Vec3 vYplus1 = osg::Vec3(m_x0 + x * m_delta, (m_y0 + y * m_delta)+1, m_heightData[(y+1) * m_nx + x]);
				osg::Vec3 vYminus1 = osg::Vec3(m_x0 + x * m_delta, (m_y0 + y * m_delta)-1, m_heightData[(y-1) * m_nx + x]);

				osg::Vec3 normal = (vXplus1 - vXminus1) ^ (vYplus1 - vYminus1);
				normal.normalize();
				pNormals->push_back(normal);

				//cosθ = a.b / |a||b|
				//Set vertColour based on angle of normal based on up vector
				osg::Vec3 up = osg::Vec3(0.0, 0.0, 1.0);
				float angleRad = (normal * up) / (normal.length() * up.length());
				float angleDeg = osg::RadiansToDegrees(angleRad);
				//If height is above specified value, make snow texture

				if (m_heightData[y * m_nx + x] > (45 + (rand() % 15))) //Set snow
				{
					//Set colour per vert
					vertColour = osg::Vec4(0.0, 0.0, 1.0, 0.0);
					pColors->push_back(vertColour); 
				}
				else //Else find angle to set either grass or rock texture
				{
					if (angleDeg > 50) //Set rock
					{
						//Set colour per vert
						vertColour = osg::Vec4(1.0, 0.0, 0.0, 0.0);
						pColors->push_back(vertColour); 
					}
					else if(angleDeg > 35 && angleDeg < 50) //Set dirt
					{
						//Set colour per vert
						vertColour = osg::Vec4(0.0, 0.0, 0.0, 1.0);
						pColors->push_back(vertColour); 
					}
					else //Set grass
					{
						//Set colour per vert
						vertColour = osg::Vec4(0.0, 1.0, 0.0, 0.0);
						pColors->push_back(vertColour); 
					} 
				}
			}

			//Set UV coords.
			osg::Vec2 uvCoords;
			float texScale = 0.025f;
			uvCoords = osg::Vec2((m_x0 + x * m_delta)*texScale, (m_y0 + y * m_delta)*texScale);		
			pTexCoords->push_back(uvCoords);
		}
	}

	//Set triangles
	for(int y = 0; y < m_ny - 1; y++)
	{
		for(int x = 0; x < m_nx - 1; x++)
		{
			//First triangle
			triangles->push_back(y * m_nx + x);
			triangles->push_back(y * m_nx + x + 1);
			triangles->push_back((y + 1) * m_nx + x);

			//Second triangle
			triangles->push_back(y * m_nx + x + 1);
			triangles->push_back((y + 1) * m_nx + x + 1);
			triangles->push_back((y + 1) * m_nx + x);
		}
	}  
	//Add normals to normal data
	m_normalData = new osg::Vec3[pNormals->size()];
	for(int i = 0; i < pNormals->size(); i++)
		m_normalData[i] = pNormals[0][i];

#pragma endregion

#pragma region Set variables to geometry, set textures
	//Add verts, normals to geometry.
	// add the vertex and normal arrays to the geometry
	pGeometry->setVertexArray( pVertices );
	pGeometry->setNormalArray( pNormals );

	//Assign colour arrays
	pGeometry->setColorArray(pColors);
	pGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	// tell it to use one normal per vertex
	pGeometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );

	// add the triangles to the geometry
	pGeometry->addPrimitiveSet( triangles );

	//Add UVs
	pGeometry->setTexCoordArray(0, pTexCoords);
	pGeometry->setTexCoordArray(1, pTexCoords);
	pGeometry->setTexCoordArray(2, pTexCoords);
	pGeometry->setTexCoordArray(3, pTexCoords);

	// create a simple material, and assign it to the geometry
	osg::Material *pMaterial = new osg::Material();
	pMaterial->setAmbient( osg::Material::FRONT, osg::Vec4( 0.2, 0.2, 0.2, 1.0 ) );
	pMaterial->setDiffuse( osg::Material::FRONT, osg::Vec4( 0.8, 0.8, 0.8, 1.0 ) );

	osg::StateSet* pStateSet = new osg::StateSet();

#pragma region Texture loading
	//Grass Texture
	osg::Texture2D* pTextureGrass = new osg::Texture2D;
	// set the wrapping properties
	pTextureGrass->setWrap( osg::Texture::WRAP_R, osg::Texture::REPEAT );
	pTextureGrass->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
	pTextureGrass->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT );

	// protect from being optimized away as static state:
	pTextureGrass->setDataVariance(osg::Object::DYNAMIC); 

	// load an image by reading a file: 
	osg::Image* pTexImageGrass = osgDB::readImageFile("Textures\\grass2.png");
	if (!pTexImageGrass)
	{
		std::cout << " couldn't find texture." << std::endl;
		return;
	}

	//Grass Texture
	osg::Texture2D* pTextureDirt = new osg::Texture2D;
	// set the wrapping properties
	pTextureDirt->setWrap( osg::Texture::WRAP_R, osg::Texture::REPEAT );
	pTextureDirt->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
	pTextureDirt->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT );

	// protect from being optimized away as static state:
	pTextureDirt->setDataVariance(osg::Object::DYNAMIC); 

	// load an image by reading a file: 
	osg::Image* pTexImageDirt = osgDB::readImageFile("Textures\\dirt.png");
	if (!pTexImageDirt)
	{
		std::cout << " couldn't find texture." << std::endl;
		return;
	}

	//Rock texture
	osg::Texture2D* pTextureRock = new osg::Texture2D;
	pTextureRock->setWrap( osg::Texture::WRAP_R, osg::Texture::REPEAT );
	pTextureRock->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
	pTextureRock->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT );

	// protect from being optimized away as static state:
	pTextureRock->setDataVariance(osg::Object::DYNAMIC); 

	// load an image by reading a file: 
	osg::Image* pTexImageRock = osgDB::readImageFile("Textures\\rock2.png");
	if (!pTexImageRock)
	{
		std::cout << " couldn't find texture." << std::endl;
		return;
	}

	//Snow texture
	osg::Texture2D* pTextureSnow = new osg::Texture2D;
	pTextureSnow->setWrap( osg::Texture::WRAP_R, osg::Texture::REPEAT );
	pTextureSnow->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
	pTextureSnow->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT );

	// protect from being optimized away as static state:
	pTextureSnow->setDataVariance(osg::Object::DYNAMIC); 

	// load an image by reading a file: 
	osg::Image* pTexImageSnow = osgDB::readImageFile("Textures\\snow2.png");
	if (!pTexImageSnow)
	{
		std::cout << " couldn't find texture." << std::endl;
		return;
	}

	// Assign the snow texture to the image we read from file: 
	pTextureSnow->setImage(pTexImageSnow);
	// Assign the rock texture to the image we read from file: 
	pTextureRock->setImage(pTexImageRock);
	// Assign the grass texture to the image we read from file: 
	pTextureGrass->setImage(pTexImageGrass);  
	// Assign the dirt texture to the image we read from file: 
	pTextureDirt->setImage(pTexImageDirt);  

	// add texturing with this texture to the state set
	//pStateSet->setTextureAttributeAndModes(0, pTextureRock, osg::StateAttribute::ON);

	//Will use textures set by shader files
	osg::Program *MultiTexProgram = new osg::Program;
	MultiTexProgram->setName( "multitex" );
	osg::Shader *MTVertObj = new osg::Shader( osg::Shader::VERTEX );
	osg::Shader *MTFragObj = new osg::Shader( osg::Shader::FRAGMENT );

	MultiTexProgram->addShader( MTFragObj );
	MultiTexProgram->addShader( MTVertObj );

	LoadShaderSource( MTVertObj, "Shaders\\multitex.vert" );
	LoadShaderSource( MTFragObj, "Shaders\\multitex.frag" );

	pStateSet->setAttributeAndModes( MultiTexProgram, osg::StateAttribute::ON);

	pStateSet->addUniform( new osg::Uniform("Tex0", 0) );
	pStateSet->addUniform( new osg::Uniform("Tex1", 1) );
	pStateSet->addUniform( new osg::Uniform("Tex2", 2) );
	pStateSet->addUniform( new osg::Uniform("Tex3", 3) );

	pStateSet->setTextureAttribute( 0, pTextureGrass );
	pStateSet->setTextureAttribute( 1, pTextureRock );
	pStateSet->setTextureAttribute( 2, pTextureSnow );
	pStateSet->setTextureAttribute( 3, pTextureDirt );
#pragma endregion

	pStateSet->setAttribute( pMaterial );
	pGeode->setStateSet( pStateSet );  
#pragma endregion


	// add the geometry to the geode
	pGeode->addDrawable(pGeometry);
}

// Load shaders
void Terrain::LoadShaderSource( osg::Shader* shader, const std::string& fileName )
{
	//Taken from the OpenSceneGraph shader sample
	std::string fqFileName = osgDB::findDataFile(fileName);
	if( fqFileName.length() != 0 )
	{
		shader->loadShaderSourceFromFile( fqFileName.c_str() );
	}
	else
	{
		osg::notify(osg::WARN) << "File \"" << fileName << "\" not found." << std::endl;
	}
}

//Utilities
float Terrain::HeightAtPoint(float xPos, float yPos)
{
	//Find the "cell" that x and y lie in
	int x = (xPos - m_x0) / m_delta;
	int y = (yPos - m_y0) / m_delta;

	if (x < 0 || y < 0 || x > m_nx - 2 || y > m_nx - 2)
		return 0.0f; //Not on terrain grid

	float xFrac = (xPos - (int)xPos) / m_delta; //Get x between 0->1
	float yFrac = (yPos - (int)yPos) / m_delta; //Get y between 0->1

	//Find the four corners
	float c00 = m_heightData[y * m_nx + x];
	float c01 = m_heightData[(y + 1) * m_nx + x];
	float c11 = m_heightData[(y + 1) * m_nx + (x + 1)];
	float c10 = m_heightData[y * m_nx + (x + 1)];

	//Interpolate in x direction
	float r1 = c00 + (c10 - c00) * xFrac;
	float r2 = c01 + (c11 - c01) * xFrac;

	//Interpolate between r1 and r2 to find z
	return r1 + (r2 - r1) * yFrac;

}

osg::Vec3 Terrain::NormalAtPoint(float xPos, float yPos)
{
	osg :: Vec3 normal;
	osg :: Vec3 v[3];

	//Find the "cell" that x and y lie in
	int x = (xPos - m_x0) / m_delta;
	int y = (yPos - m_y0) / m_delta;

	if (yPos < xPos)
	{
		v[0] = osg::Vec3(m_x0 + x * m_delta, m_y0 + y * m_delta, m_heightData[y * m_nx + x]);
		v[1] = osg::Vec3(m_x0 + (x + 1) * m_delta, m_y0 + (y + 1) * m_delta, m_heightData[(y + 1) * m_nx + x + 1]);
		v[2] = osg::Vec3(m_x0 + (x + 1) * m_delta, m_y0 + y * m_delta, m_heightData[y * m_nx + x + 1]);	

		normal = (v[2] - v[0]) ^ (v[1] - v[0]);
		normal.normalize();
	}
	else
	{
		v[0] = osg::Vec3(m_x0 + x * m_delta, m_y0 + y * m_delta, m_heightData[y * m_nx + x]);
		v[1] = osg::Vec3(m_x0 + x * m_delta, m_y0 + (y + 1) * m_delta, m_heightData[(y + 1) * m_nx + x]);	
		v[2] = osg::Vec3(m_x0 + (x + 1) * m_delta, m_y0 + (y + 1) * m_delta, m_heightData[(y + 1) * m_nx + x + 1]);

		normal = (v[2] - v[0]) ^ (v[1] - v[0]);
		normal.normalize();
	}

	return normal;
}

osg::Vec3 Terrain::SmoothNormalAtPoint(float xPos, float yPos)
{
	//Find the "cell" that x and y lie in
	int x = (xPos - m_x0) / m_delta;
	int y = (yPos - m_y0) / m_delta;


	osg :: Vec3 nTop, nBottom, nInterp;

	nBottom = m_normalData[x + y * m_nx] * (1.0 - xPos) + m_normalData[x + 1 + y*m_nx] * xPos;
	nTop = m_normalData[x + m_nx * (y + 1)] * (1.0 - xPos) + m_normalData[x + 1 +m_nx * (y + 1)] * xPos;
	
	nTop.normalize();
	nBottom.normalize();
	
	nInterp = nBottom * (1.0 - yPos) + nTop * yPos;
	nInterp.normalize();

	return nInterp;
}


