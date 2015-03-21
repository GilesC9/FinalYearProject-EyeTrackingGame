#include "SkyDome.h"

SkyDome::SkyDome(){ }

void SkyDome::CreateSkyDome(osg::Geode* pGeode, float radius, int rings, int subdivisons)
{
	//Create a geometry container
	osg::ref_ptr<osg::Geometry> pGeometry = new osg::Geometry;
	//Create an array for the vertices
	osg::ref_ptr<osg::Vec3Array> pVertices = new osg::Vec3Array;
	//Create an array for the triangle indices
	osg::DrawElementsUInt* pTriangles = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
	//Create an array for UV coords
	osg::ref_ptr<osg::Vec2Array> pTexCoords = new osg::Vec2Array;
	//Create colour array
	osg::ref_ptr<osg::Vec4Array> pColors = new osg::Vec4Array;
	pColors->push_back(osg::Vec4(1.0f , 1.0f , 1.0f , 1.0f));


	//Verts
	for(int i = 0; i < rings; i++) //Number of rings of vertices
	{
		float rad = (float)i / float(rings - 1);
		for(int j = 0; j < subdivisons; j++) //Number of angular subdivisions
		{
			float theta = 2.0f * PI * (float ) j / (float)subdivisons;
			float x = (rad * sin(theta));
			float y = (rad * cos(theta));
			float z = (1.0f - x * x - y * y);

			if (z < 0.0f) z = 0.0f;

			z = sqrt(z);

			//Set verts
			osg::Vec3 verts[1];
			verts[0] = osg::Vec3(x * radius, y * radius, z * radius);
			pVertices->push_back(verts[0]);

			//UV coords
			pTexCoords->push_back(osg::Vec2((x + 1) / 2, (y + 1) / 2));
		}
	}

	//Triangles
	for (int a = 0; a < rings - 1; a++)
	{
		for (int b = 0; b < subdivisons; b++)
		{
			//Create triangles
			int ind0, ind1, ind2, ind3;
			ind0 = a * subdivisons + b;
			ind1 = (a + 1 ) * subdivisons + b;
			ind2 = a * subdivisons + (b + 1) % subdivisons;
			ind3 = (a + 1 ) * subdivisons + (b + 1) % subdivisons;

			//Triangle 1
			pTriangles->push_back(ind1);
			pTriangles->push_back(ind0);
			pTriangles->push_back(ind3);
			//Triangle 2
			pTriangles->push_back(ind0);
			pTriangles->push_back(ind3);
			pTriangles->push_back(ind2);
		}
	}

	//Add verts, normals to geometry.
	// add the vertex and normal arrays to the geometry
	pGeometry->setVertexArray(pVertices);

	//Assign colour arrays
	pGeometry->setColorArray(pColors);
	pGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

	//Add the triangles to the geometry
	pGeometry->addPrimitiveSet(pTriangles);

	//Create state set
	osg::StateSet* pStateSet = new osg::StateSet();
	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	//Create an OSG texture
	osg::Texture2D* pTextureSky = new osg::Texture2D;
	
	//Set the wrapping properties
	pTextureSky->setWrap( osg::Texture::WRAP_R, osg::Texture::REPEAT );
	pTextureSky->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
	pTextureSky->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT );
	
	//Protect from being optimized away as static state:
	pTextureSky->setDataVariance(osg::Object::DYNAMIC);
	
	//Load an image by reading a ﬁle
	osg::Image* pTexImageSky = osgDB::readImageFile("Textures\\skydome.png");
	if (!pTexImageSky)
	{
		std::cout << " couldn't find texture." << std::endl;
		return;
	}

	//Assign the texture to the image we read from ﬁle:
	pTextureSky->setImage(pTexImageSky);
	//Add texturing with this texture to the state set
	pStateSet->setTextureAttributeAndModes(0, pTextureSky, osg::StateAttribute::ON);
	//Add state set
	pGeode->setStateSet(pStateSet); 
	//Add UVs
	pGeometry->setTexCoordArray(0, pTexCoords);
 
	//Add the geometry to the geode
	pGeode->addDrawable(pGeometry);
}