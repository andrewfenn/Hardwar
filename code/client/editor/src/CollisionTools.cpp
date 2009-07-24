/******************************************************************************************
MOC - Minimal Ogre Collision v 1.0 beta
The MIT License

Copyright (c) 2008 MouseVolcano (Thomas Gradl, Esa Kylli, Erik Biermann, Karolina Sefyrin)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
******************************************************************************************/
#include "CollisionTools.h"

namespace MOC {

#ifdef ETM_TERRAIN
CollisionTools::CollisionTools(Ogre::SceneManager *sceneMgr, const ET::TerrainInfo* terrainInfo)
{
	mRaySceneQuery = sceneMgr->createRayQuery(Ogre::Ray());
    if (mRaySceneQuery == 0)
    {
      // LOG_ERROR << "Failed to create Ogre::RaySceneQuery instance" << ENDLOG;
      return;
    }
    mRaySceneQuery->setSortByDistance(true);
    
    mTSMRaySceneQuery = 0;
    
	mTerrainInfo = terrainInfo;
	
	_heightAdjust = 0.0f;
}
#endif

CollisionTools::CollisionTools(Ogre::SceneManager *sceneMgr)
{
	mSceneMgr = sceneMgr;
	
	mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
    if (mRaySceneQuery == 0)
    {
      // LOG_ERROR << "Failed to create Ogre::RaySceneQuery instance" << ENDLOG;
      return;
    }
    mRaySceneQuery->setSortByDistance(true);
    
    mTSMRaySceneQuery =  mSceneMgr->createRayQuery(Ogre::Ray());
    
    _heightAdjust = 0.0f;    	
}

CollisionTools::~CollisionTools()
{
	if (mRaySceneQuery != 0)
		delete mRaySceneQuery;
		
	if (mTSMRaySceneQuery != 0)
		delete mTSMRaySceneQuery;
}

bool CollisionTools::raycastFromCamera(RenderWindow* rw, Camera* camera, const OIS::MouseEvent &e, Vector3 &result, unsigned long &target,float &closest_distance, const uint32 queryMask)
{
	// Create the ray to test
	Real tx = (Real) e.state.X.abs / (Real) rw->getWidth();
	Real ty = (Real) e.state.Y.abs / (Real) rw->getHeight();
	Ray ray = camera->getCameraToViewportRay(tx, ty);

	return raycast(ray, result, target, closest_distance, queryMask);
}

bool CollisionTools::collidesWithEntity(const Vector3& fromPoint, const Vector3& toPoint, const float collisionRadius, const float rayHeightLevel, const uint32 queryMask)
{
	Vector3 fromPointAdj(fromPoint.x, fromPoint.y + rayHeightLevel, fromPoint.z);
	Vector3 toPointAdj(toPoint.x, toPoint.y + rayHeightLevel, toPoint.z);	
	Vector3 normal = toPointAdj - fromPointAdj;
	float distToDest = normal.normalise();

	Vector3 myResult(0, 0, 0);
	Ogre::Entity* myObject = 0;
	float distToColl = 0.0f;

	if (raycastFromPoint(fromPointAdj, normal, myResult, (unsigned long&)myObject, distToColl, queryMask))
	{
		distToColl -= collisionRadius; 
		return (distToColl <= distToDest);
	}
	else
	{
		return false;
	}
}

float CollisionTools::getTSMHeightAt(const float x, const float z) {
	float y=0.0f;

    static Ray updateRay;
    
    updateRay.setOrigin(Vector3(x,9999,z));
    updateRay.setDirection(Vector3::NEGATIVE_UNIT_Y);
    
    mTSMRaySceneQuery->setRay(updateRay);
    RaySceneQueryResult& qryResult = mTSMRaySceneQuery->execute();
    
    RaySceneQueryResult::iterator i = qryResult.begin();
    if (i != qryResult.end() && i->worldFragment)
    {
        y=i->worldFragment->singleIntersection.y;           
    }		
	return y;
}

void CollisionTools::calculateY(SceneNode *n, const bool doTerrainCheck, const bool doGridCheck, const float gridWidth, const uint32 queryMask)
{
	Vector3 pos = n->getPosition();
	
	float x = pos.x;
	float z = pos.z;
	float y = pos.y;
	
	Vector3 myResult(0,0,0);
	Ogre::Entity *myObject=0;
	float distToColl = 0.0f;

	float terrY = 0, colY = 0, colY2 = 0;

	if( raycastFromPoint(Vector3(x,y,z),Vector3::NEGATIVE_UNIT_Y,myResult,(unsigned long&)myObject, distToColl, queryMask)){
		if (myObject != 0) {
			colY = myResult.y;
		} else {
			colY = -99999;
		}
	}
	
	//if doGridCheck is on, repeat not to fall through small holes for example when crossing a hangbridge
	if (doGridCheck) {		
		if( raycastFromPoint(Vector3(x,y,z)+(n->getOrientation()*Vector3(0,0,gridWidth)),Vector3::NEGATIVE_UNIT_Y,myResult,(unsigned long&)myObject, distToColl, queryMask)){
			if (myObject != 0) {
				colY = myResult.y;
			} else {
				colY = -99999;
			}	
		}
		if (colY<colY2) colY = colY2;
	}
	
	// set the parameter to false if you are not using ETM or TSM
	if (doTerrainCheck) {
	
#ifdef ETM_TERRAIN
		// ETM height value
		terrY = mTerrainInfo->getHeightAt(x,z);
#else	
		// TSM height value
		terrY = getTSMHeightAt(x,z);
#endif

		if(terrY < colY ) {
			n->setPosition(x,colY+_heightAdjust,z);
		} else {
			n->setPosition(x,terrY+_heightAdjust,z);
		}
	} else {
		if (!doTerrainCheck && colY == -99999) colY = y;
		n->setPosition(x,colY+_heightAdjust,z);
	}
}

// raycast from a point in to the scene.
// returns success or failure.
// on success the point is returned in the result.
bool CollisionTools::raycastFromPoint(const Vector3 &point,
                                        const Vector3 &normal,
										Vector3 &result,unsigned long &target,float &closest_distance,
										const uint32 queryMask)
{
    // create the ray to test
    static Ogre::Ray ray;
	ray.setOrigin(point);
	ray.setDirection(normal);

	return raycast(ray, result, target, closest_distance, queryMask);
}

bool CollisionTools::raycast(const Ray &ray, Vector3 &result,unsigned long &target,float &closest_distance, const uint32 queryMask)
{
	target = 0;

    // check we are initialised
   if (mRaySceneQuery != 0)
   {
      // create a query object
      mRaySceneQuery->setSortByDistance(true);
      mRaySceneQuery->setQueryMask(queryMask);
      mRaySceneQuery->setRay(ray);
      // execute the query, returns a vector of hits
      if (mRaySceneQuery->execute().size() <= 0)
      {
         // raycast did not hit an objects bounding box
         return (false);
      }
   }
   else
   {
      //LOG_ERROR << "Cannot raycast without RaySceneQuery instance" << ENDLOG;
      return (false);
   }   

    // at this point we have raycast to a series of different objects bounding boxes.
    // we need to test these different objects to see which is the first polygon hit.
    // there are some minor optimizations (distance based) that mean we wont have to
    // check all of the objects most of the time, but the worst case scenario is that
    // we need to test every triangle of every object.
    //Ogre::Real closest_distance = -1.0f;
	 closest_distance = -1.0f;
    Ogre::Vector3 closest_result;
    Ogre::RaySceneQueryResult &query_result = mRaySceneQuery->getLastResults();
    for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
    {
        // stop checking if we have found a raycast hit that is closer
        // than all remaining entities
        if ((closest_distance >= 0.0f) &&
            (closest_distance < query_result[qr_idx].distance))
        {
            break;
        }

        // only check this result if its a hit against an entity
        if ((query_result[qr_idx].movable != 0)  &&
            (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0)) 
        {
            // get the entity to check
			Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable); 			
						
            // mesh data to retrieve         
            size_t vertex_count;
            size_t index_count;
            Ogre::Vector3 *vertices;
            unsigned long *indices;

            // get the mesh information
			GetMeshInformation(pentity->getMesh(), vertex_count, vertices, index_count, indices,             
                              pentity->getParentNode()->_getDerivedPosition(),
                              pentity->getParentNode()->_getDerivedOrientation(),
                              pentity->getParentNode()->getScale());

            // test for hitting individual triangles on the mesh
            bool new_closest_found = false;
            for (int i = 0; i < static_cast<int>(index_count); i += 3)
            {
                // check for a hit against this triangle
                std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
                    vertices[indices[i+1]], vertices[indices[i+2]], true, false);

                // if it was a hit check if its the closest
                if (hit.first)
                {
                    if ((closest_distance < 0.0f) ||
                        (hit.second < closest_distance))
                    {
                        // this is the closest so far, save it off
                        closest_distance = hit.second;
                        new_closest_found = true;
                    }
                }
            }

			// free the verticies and indicies memory
            delete[] vertices;
            delete[] indices;

            // if we found a new closest raycast for this object, update the
            // closest_result before moving on to the next object.
            if (new_closest_found)
            {
				target = (unsigned long)pentity;
                closest_result = ray.getPoint(closest_distance);               
            }
        }       
    }

    // return the result
    if (closest_distance >= 0.0f)
    {
        // raycast success
		result = closest_result;
        return (true);
    }
    else
    {
        // raycast failed
        return (false);
    } 
}

bool CollisionTools::raycastEntityPolygons(Ogre::Entity* entity, const Ray ray, Vector3 &result, unsigned long &target,float &closest_distance)
{
   // mesh data to retrieve         
   size_t vertex_count;
   size_t index_count;
   Ogre::Vector3 *vertices;
   unsigned long *indices;

   // get the mesh information
   GetMeshInformation(entity->getMesh(), vertex_count, vertices, index_count, indices,             
                     entity->getParentNode()->_getDerivedPosition(),
                     entity->getParentNode()->_getDerivedOrientation(),
                     entity->getParentNode()->getScale());

   // test for hitting individual triangles on the mesh
   bool new_closest_found = false;
   for (int i = 0; i < static_cast<int>(index_count); i += 3)
   {
       // check for a hit against this triangle
       std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
           vertices[indices[i+1]], vertices[indices[i+2]], true, false);

       // if it was a hit check if its the closest
       if (hit.first)
       {
           if ((closest_distance < 0.0f) ||
               (hit.second < closest_distance))
           {
               // this is the closest so far, save it off
               closest_distance = hit.second;
               new_closest_found = true;
           }
       }
   }

   // free the verticies and indicies memory
   delete[] vertices;
   delete[] indices;

   // if we found a new closest raycast for this object, update the
   // closest_result before moving on to the next object.
   if (new_closest_found)
   {
      target = (unsigned long)entity;
      result = ray.getPoint(closest_distance);
   }
   return new_closest_found;
}

// Get the mesh information for the given mesh.
// Code found on this forum link: http://www.ogre3d.org/wiki/index.php/RetrieveVertexData
void CollisionTools::GetMeshInformation(const Ogre::MeshPtr mesh,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale)
{
   bool added_shared = false;
   size_t current_offset = 0;
   size_t shared_offset = 0;
   size_t next_offset = 0;
   size_t index_offset = 0;

   vertex_count = index_count = 0;

   // Calculate how many vertices and indices we're going to need
   for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
   {
      Ogre::SubMesh* submesh = mesh->getSubMesh( i );

      // We only need to add the shared vertices once
      if(submesh->useSharedVertices)
      {
         if( !added_shared )
         {
            vertex_count += mesh->sharedVertexData->vertexCount;
            added_shared = true;
         }
      }
      else
      {
         vertex_count += submesh->vertexData->vertexCount;
      }

      // Add the indices
      index_count += submesh->indexData->indexCount;
   }


   // Allocate space for the vertices and indices
   vertices = new Ogre::Vector3[vertex_count];
   indices = new unsigned long[index_count];

   added_shared = false;

   /* Run through the submeshes again, adding the data into the arrays */
   for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
   {
      Ogre::SubMesh* submesh = mesh->getSubMesh(i);
      Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

      if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
      {
         if(submesh->useSharedVertices)
         {
            added_shared = true;
            shared_offset = current_offset;
         }

         const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

         Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

         unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

         /* There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
          * as second argument. So make it float, to avoid trouble when Ogre::Real will
          * be comiled/typedefed as double:
          *     Ogre::Real* pReal;
          */
         float* pReal;

         for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
         {
             posElem->baseVertexPointerToElement(vertex, &pReal);

             Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

             vertices[current_offset + j] = (orient * (pt * scale)) + position;
         }

         vbuf->unlock();
         next_offset += vertex_data->vertexCount;
      }


      Ogre::IndexData* index_data = submesh->indexData;
      size_t numTris = index_data->indexCount / 3;
      Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

      bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

      unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
      unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


      size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

      if ( use32bitindexes )
      {
         for ( size_t k = 0; k < numTris*3; ++k)
         {
            indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
         }
      }
      else
      {
         for ( size_t k = 0; k < numTris*3; ++k)
         {
            indices[index_offset++] = static_cast<unsigned long>(pShort[k]) + static_cast<unsigned long>(offset);
         }
      }

      ibuf->unlock();
      current_offset = next_offset;
   }
}

   void CollisionTools::setHeightAdjust(const float heightadjust)
   {
      _heightAdjust = heightadjust;
   }

   float CollisionTools::getHeightAdjust(void)
   {
      return _heightAdjust;
   }
};
