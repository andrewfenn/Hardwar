/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2008  Andrew Fenn
    
    Hardwar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Hardwar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "EditorState.h"
#include <OgreLogManager.h>

using namespace Ogre;

EditorState* EditorState::mEditorState;

void EditorState::enter( void ) {

	mRoot         = Ogre::Root::getSingletonPtr();
	
	mOverlayMgr   = Ogre::OverlayManager::getSingletonPtr();
	mSceneMgr     = mRoot->getSceneManager( "PlaySceneMgr" );
//    mWorldNode    = mSceneMgr->getRootSceneNode()->getSceneNode("world");
//	mCamera       = static_cast<Ogre::Camera*>(mWorldNode->getAttachedObject( "PlayerCamera" ));

	mViewport     = mRoot->getAutoCreatedWindow()->addViewport( mCamera, 1 );
	mGUIMgr		  = GUIManager::getSingletonPtr();
    mLogMgr       = Ogre::LogManager::getSingletonPtr();
		

	// Initialise CEGUI for user interface stuff
	//mGUIMgr->initialise(mSceneMgr, mRoot->getAutoCreatedWindow());
	//setupEditorUI();

   // Start the world manager and load up a world
   mWorldMgr = new WorldManager;
   // load up our SQLite world database
/*   if (!mWorldMgr->loadWorldData("world/default.db", mSceneMgr))
   	this->requestShutdown();	
  */ 
   mSelectedObject = 0;
   mMouseY = mMouseX = mMouseRotX = mMouseRotY = 0;
   mMouseDownButton2 = zoomin = zoomout = false;
   CamRotatePos = CamLookAtPos = 0;
   mScreenshots = 0;
   mOnCEGUI = false;
}

// ------------------------------------
//  Setup the editors UI
// ------------------------------------
void EditorState::setupEditorUI() {

    // setup the skin and font to use
	CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");	
  	mGUIMgr->mSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
    mGUIMgr->mSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-10");

	// setup the CEGUI sheet
	CEGUI::Window* sheet= CEGUI::WindowManager::getSingleton().loadWindowLayout("Editor.layout");
    mGUIMgr->mSystem->setGUISheet(sheet);

	// Grab the window manager   
	CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
	
    // setup the quit button
    CEGUI::Window *quit1 = wmgr->getWindow((CEGUI::utf8*)"Root//SystemTab/QuitButton");
    quit1->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorState::GUIHandleShutdown, this));

    // setup the save world button
    CEGUI::Window *savebutton = wmgr->getWindow((CEGUI::utf8*)"Root//SystemTab/SaveButton");
    savebutton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorState::GUISaveWorld, this));

	// start to populate mesh combo box list
	CEGUI::Combobox* meshList = (CEGUI::Combobox*)CEGUI::WindowManager::getSingleton().getWindow("Root//NewTab/MeshList");
	CEGUI::ListboxTextItem* item = new CEGUI::ListboxTextItem((CEGUI::utf8*)"null", 0);
	// Grab all the mesh names and push them to our combo box	
	boost::filesystem::path hanger_path( "../media/models/hangers" );
	int i = 0;
	if (boost::filesystem::is_directory(hanger_path)) {
		for (boost::filesystem::directory_iterator itr(hanger_path); itr!=boost::filesystem::directory_iterator(); ++itr) {
			std::string temp = (std::string)itr->path().leaf();
			temp = temp.substr(temp.length()-4, 4);
			if (temp.compare("mesh") == 0) {
				item = new CEGUI::ListboxTextItem((std::string)itr->path().leaf(), i);
				meshList->addItem(item); // attach to the combo box
				i++;
			}
		}
	}
	
	meshList->setReadOnly(true);
	if (i > 0) 
		meshList->setItemSelectState(item, true);
		
	CEGUI::Window* rootwindow = (CEGUI::Window*)CEGUI::WindowManager::getSingleton().getWindow("Root");	
	rootwindow->subscribeEvent(CEGUI::Window::EventActivated, CEGUI::Event::Subscriber(&EditorState::CEGUIActivated, this));
	rootwindow->subscribeEvent(CEGUI::Window::EventDeactivated, CEGUI::Event::Subscriber(&EditorState::CEGUIDeactivated, this));
	
	// Setup the position edit boxes
	CEGUI::Editbox* EditXPos = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow("Root//EditTab/EditXPos");
	EditXPos->setValidationString("[0-9.]*");
//   EditXPos->subscribeEvent(CEGUI::Editbox::EventInvalidEntryAttempted, CEGUI::Event::Subscriber(&EditorState::CEGUIDeactivated, this));
	EditXPos->subscribeEvent(CEGUI::Window::EventActivated, CEGUI::Event::Subscriber(&EditorState::CEGUIActivated, this));
//	EditXPos->subscribeEvent(CEGUI::Window::EventDeactivated, CEGUI::Event::Subscriber(&EditorState::CEGUIDeactivated, this));
//	EditXPos->subscribeEvent(CEGUI::Editbox::EventTextAccepted, CEGUI::Event::Subscriber(&EditorState::CEGUIDeactivated, this));

	resetGUIEditPos(); // clear the edit boxes and disable them when starting the editor
}

void EditorState::exit( void ) {
    mSceneMgr->clearScene();
    mSceneMgr->destroyAllCameras();
    mRoot->getAutoCreatedWindow()->removeAllViewports();
}

// No pausing or resume in the editor so leave it
void EditorState::pause( void ) { }
void EditorState::resume( void ) { }


// ------------------------------------
//  Update Camera Position
// ------------------------------------
void EditorState::updateCamera() {
  	Ogre::Vector3 pos = mCamera->getPosition();

	// Do mouse rotation
	if (mMouseDownButton2) {
		int diffx=mMouseX-mMouseRotX;
		int diffy=mMouseY-mMouseRotY;
		CamRotatePos.x += 0.00005f * diffx;
		CamRotatePos.y += 0.00005f * diffy;
		
		mCamera->lookAt(CamLookAtPos); // that'll do for now

	//	mCamera->setDirection(CamRotatePos);
	/*	mCamera->yaw((Ogre::Degree)CamRotatePos.x);
		mCamera->pitch((Ogre::Degree)CamRotatePos.y);*/

	} else {
	   if (mMouseY < 10)
		 	pos.z+=15;
		if (mMouseY > mViewport->getActualHeight()-10)
		 	pos.z-=15;
		if (mMouseX < 10)
		 	pos.x+=15;
		if (mMouseX > mViewport->getActualWidth()-10)
		 	pos.x-=15;
		if (zoomin)
			pos.y-= 10;
		if (zoomout)
			pos.y+= 10;
		mCamera->setPosition(pos);
	}
}



// ------------------------------------
//  Render loop update
// ------------------------------------
void EditorState::update( unsigned long lTimeElapsed ) {

	updateCamera();	// Do mouse movement

}


// ------------------------------------
// Clicked on to a CEGUI field
// ------------------------------------
bool EditorState::CEGUIActivated(const CEGUI::EventArgs& evt) {

	// give the keyboard only to CEGUI
	mOnCEGUI = true;
	
	return true;
}

// ------------------------------------
// Clicked off a CEGUI field 
// ONLY EDITBOXS subscibed to this function
// ------------------------------------
bool EditorState::CEGUIDeactivated(const CEGUI::EventArgs& evt) {
  const CEGUI::WindowEventArgs& we = static_cast<const CEGUI::WindowEventArgs&>(evt);
  CEGUI::String senderID = we.window->getName();

	CEGUI::Editbox* editBox = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow(senderID);
	// if the editbox still has input focus do not apply changes
	changeSelectedObject();
	// give the keyboard back to everyone
	mOnCEGUI = false;
	
	return true;
}

void EditorState::resetGUIEditPos() {
	CEGUI::Editbox* EditBoxPos;
	// Reset X position
	EditBoxPos = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow("Root//EditTab/EditXPos");
	EditBoxPos->setText("");
	EditBoxPos->setEnabled(false);
	// Reset Y position
	EditBoxPos = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow("Root//EditTab/EditYPos");
	EditBoxPos->setText("");
	EditBoxPos->setEnabled(false);
	// Reset Z position
	EditBoxPos = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow("Root//EditTab/EditZPos");
	EditBoxPos->setText("");
	EditBoxPos->setEnabled(false);

	// Reset X rotation
	EditBoxPos = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow("Root//EditTab/EditXRot");
	EditBoxPos->setText("");
	EditBoxPos->setEnabled(false);
	// Reset Y rotation
	EditBoxPos = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow("Root//EditTab/EditYRot");
	EditBoxPos->setText("");
	EditBoxPos->setEnabled(false);
	// Reset Z rotation
	EditBoxPos = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow("Root//EditTab/EditZRot");
	EditBoxPos->setText("");
	EditBoxPos->setEnabled(false);
}

void EditorState::updateGUIEditPos() {
	// put position and rotation coords into the edit boxes
	bool enabled = false;
	Ogre::Entity *pentity = 0;
	CEGUI::Editbox* EditBoxPos;
			
	if (mSelectedObject) {
			pentity = static_cast<Ogre::Entity*>(mSelectedObject);
			enabled = true;
	}
	
	Ogre::Vector3 vect;
	// grab the position and fill the text boxes
	vect = pentity->getParentNode()->getPosition();
		
	// Insert X position
	EditBoxPos = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow("Root//EditTab/EditXPos");
	EditBoxPos->setText(StringConverter::toString(vect.x));
	EditBoxPos->setEnabled(enabled);
	// Insert Y position
	EditBoxPos = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow("Root//EditTab/EditYPos");
	EditBoxPos->setText(StringConverter::toString(vect.y));
	EditBoxPos->setEnabled(enabled);
	// Insert Z position
	EditBoxPos = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow("Root//EditTab/EditZPos");
	EditBoxPos->setText(StringConverter::toString(vect.z));
	EditBoxPos->setEnabled(enabled);

	Ogre::Degree deg;
	// Insert X rotation
	deg = pentity->getParentNode()->getOrientation().getYaw();
	EditBoxPos = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow("Root//EditTab/EditXRot");
	EditBoxPos->setText(StringConverter::toString(deg));
	EditBoxPos->setEnabled(enabled);
	// Insert Y rotation
	deg = pentity->getParentNode()->getOrientation().getPitch();
	EditBoxPos = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow("Root//EditTab/EditYRot");
	EditBoxPos->setText(StringConverter::toString(deg));
	EditBoxPos->setEnabled(enabled);
	// Insert Z rotation
	deg = pentity->getParentNode()->getOrientation().getRoll();
	EditBoxPos = (CEGUI::Editbox*)CEGUI::WindowManager::getSingleton().getWindow("Root//EditTab/EditZRot");
	EditBoxPos->setText(StringConverter::toString(deg));
	EditBoxPos->setEnabled(enabled);
}


void EditorState::keyPressed( const OIS::KeyEvent &e ) {

	if (mOnCEGUI) {
			 CEGUI::System::getSingleton().injectKeyDown(e.key);
		    CEGUI::System::getSingleton().injectChar(e.text);
			return;
	}

	if( e.key == OIS::KC_DOWN ) {
		zoomout = true;
	}
	if( e.key == OIS::KC_UP ) {
		zoomin = true;
	}	 
}

void EditorState::keyReleased( const OIS::KeyEvent &e ) {
		
	if (e.key == OIS::KC_SYSRQ) { // screenshot
		char filename[30] ;

       
       mLogMgr->logMessage("Taking Screenshot.\n");
	   std::sprintf(filename, "./screenshots/screenshot_%d.png", ++mScreenshots);
	   mRoot->getAutoCreatedWindow()->writeContentsToFile(filename);
	}    
	
	
	    
	if (mOnCEGUI) {
		// CEGUI is active
		
		if( e.key == OIS::KC_ESCAPE ) {
			mOnCEGUI = false;
		}
		
		if (e.key == OIS::KC_DELETE) {
			if (mWorldMgr->deleteBuilding(mSelectedObject->getName())) {
				// object was deleted in the worldmanager, delete the object from ogre too
				mSelectedObject->_getManager()->destroyMovableObject(mSelectedObject);
				// update editboxes
				mSelectedObject = 0;
				resetGUIEditPos();
			}
		}
	
		CEGUI::System::getSingleton().injectKeyUp(e.key);
		return;
	}


	if( e.key == OIS::KC_ESCAPE ) {
	//	this->requestShutdown();
	} else {
		// this->changeState( PlayState::getSingletonPtr() );
	}

   // camera controls
   
	if( e.key == OIS::KC_DOWN )
	 		zoomout = false;
	
	if( e.key == OIS::KC_UP )
	 		zoomin = false;
	
	
	// scale terrain up and down
	
	if(e.key == OIS::KC_1) {

	}
	if(e.key == OIS::KC_2) {
		Ogre::Vector3 pos = mCamera->getPosition();
		std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
	}
	
	if(e.key == OIS::KC_A) {
		addBuilding();
	}
	
	if(e.key == OIS::KC_B) {
		mSceneMgr->showBoundingBoxes(true);
	}

}


// ------------------------------------
//  Add a building to the terrain
// ------------------------------------
bool EditorState::addBuilding() {

	// Shoot out a ray from the mouse cursor
    Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mMouseX/float(mViewport->getActualWidth()), mMouseY/float(mViewport->getActualHeight()));
   	
	// Cast a ray at the polygon level
	Ogre::Vector3 result;
	// Use the result to place our building
	if (RaycastFromPoint(mouseRay, result)) {
		// Get which building type is to be placed
		CEGUI::Combobox* meshList = (CEGUI::Combobox*)CEGUI::WindowManager::getSingleton().getWindow("Root//NewTab/MeshList");
		CEGUI::ListboxItem* item = meshList->getSelectedItem();
		CEGUI::String name = item->getText();
		mWorldMgr->addBuilding(result, name.c_str());
		return true;
	}
	return false;
}

void EditorState::changeSelectedObject() {
	
}

void EditorState::mouseMoved( const OIS::MouseEvent &e ) {
    const OIS::MouseState &mouseState = e.state;
    /*mMousePointer->setTop(mouseState.Y.abs);
    mMousePointer->setLeft(mouseState.X.abs);*/
    mMouseX = mouseState.X.abs;
    mMouseY = mouseState.Y.abs;
    CEGUI::System::getSingleton().injectMousePosition(mouseState.X.abs, mouseState.Y.abs);

}

void EditorState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
		
	if (id==0)
		CEGUI::System::getSingleton().injectMouseButtonDown((CEGUI::MouseButton)0);
	if (id==1) {
		CEGUI::System::getSingleton().injectMouseButtonDown((CEGUI::MouseButton)1);
	}

	if (mOnCEGUI)
		return;
	
	Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(mMouseX/float(mViewport->getActualWidth()), mMouseY/float(mViewport->getActualHeight()));

	if (id == 1) { // right mouse button click, rotate camera
		const OIS::MouseState &mouseState = e.state;
		mMouseDownButton2 = true;
		mMouseRotX = mouseState.X.abs;
		mMouseRotY = mouseState.Y.abs;
		CamRotatePos = mCamera->getDirection();
		RaycastFromPoint(mouseRay, CamLookAtPos);
	}
	if (id == 0) { // left mouse button click, select
		Ogre::RaySceneQuery* mRaySceneQuery = 0;
		
		// turn off the bounding box before we get rid of the pointer
		if (mSelectedObject)
			mSelectedObject->getParentSceneNode()->showBoundingBox(false);
		resetGUIEditPos(); // clear the edit box of anything before clicking around
		mSelectedObject = 0;
		
		
		// Shoot out a ray from the mouse cursor
      mRaySceneQuery = mSceneMgr->createRayQuery(mouseRay, Ogre::SceneManager::ENTITY_TYPE_MASK);
		mRaySceneQuery->setSortByDistance(true);
		
      // Execute query
      Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
      Ogre::RaySceneQueryResult::iterator itr;

		
      // Get results, create a node/entity on the position
      for (itr = result.begin(); itr != result.end(); itr++) {
      	// check that the object isn't the terrain
			if (itr->movable && itr->movable->getMovableType() != "TerrainMipMap") {
				
				// select the object, draw the bounding box and enable cegui lock				
				mSelectedObject = itr->movable;
				updateGUIEditPos();
				mSelectedObject->getParentSceneNode()->showBoundingBox(true);
				mOnCEGUI = true;
				break;
			}
      } // end for

      mSceneMgr->destroyQuery(mRaySceneQuery);

   }
}

void EditorState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
 
 	if (id==0)
		CEGUI::System::getSingleton().injectMouseButtonUp((CEGUI::MouseButton)0);
	if (id==1)
		CEGUI::System::getSingleton().injectMouseButtonUp((CEGUI::MouseButton)1);
		
	if (mOnCEGUI)
		return;
		
    //this->changeState( PlayState::getSingletonPtr() );
    if (id == 1) {
    	mMouseDownButton2 = false;
    	CamLookAtPos = 0;
    }
}

EditorState* EditorState::getSingletonPtr( void ) {
    if( !mEditorState ) {
        mEditorState = new EditorState();
    }
    return mEditorState;
}


// Ray casting on the polygon level
// Code found in Wiki: www.ogre3d.org/wiki/index.php/Raycasting_to_the_polygon_level
bool EditorState::RaycastFromPoint(const Ogre::Ray ray, Ogre::Vector3 &pResult) {
	 // create a query object
    Ogre::RaySceneQuery* m_pray_scene_query = mSceneMgr->createRayQuery(ray, Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);
     // execute the query, returns a vector of hits
    if (m_pray_scene_query->execute().size() == 0)
    {
      // raycast did not hit an objects bounding box
       mSceneMgr->destroyQuery(m_pray_scene_query);
       std::cout << "Line 517: Raycast failed" << std::endl;
	   return (false);
    }

    // at this point we have raycast to a series of different objects bounding boxes.
    // we need to test these different objects to see which is the first polygon hit.
    // there are some minor optimizations (distance based) that mean we wont have to
    // check all of the objects most of the time, but the worst case scenario is that
    // we need to test every triangle of every object.
    Ogre::Real closest_distance = -1.0f;
    Ogre::Vector3 closest_result = Ogre::Vector3(0,0,0);
    Ogre::RaySceneQueryResult &query_result = m_pray_scene_query->getLastResults();
	
	 Ogre::Entity *pentity = 0;
	
    for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
    {

        // stop checking if we have found a raycast hit that is closer
        // than all remaining entities
        if ((closest_distance >= 0.0f) &&
            (closest_distance < query_result[qr_idx].distance))
        {
             break;
        }
       
        if ((query_result[qr_idx].worldFragment != NULL)) {
	        // if it's a world fragment just return the result
	        pResult = query_result[qr_idx].worldFragment->singleIntersection;
	        mSceneMgr->destroyQuery(m_pray_scene_query);
        	return (true);
        }
        
        // only check this result if its a hit against an entity
        if ((query_result[qr_idx].movable != NULL) &&
            (query_result[qr_idx].movable->getMovableType().compare("Entity") == 0))
        {
            // get the entity to check
            pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable);
        }

        if (pentity != NULL) {

            // mesh data to retrieve         
            size_t vertex_count;
            size_t index_count;
            Ogre::Vector3 *vertices;
            unsigned long *indices;

            // get the mesh information
         	getMeshInformation(pentity->getMesh(), vertex_count, vertices, index_count, indices,             
                              pentity->getParentNode()->_getDerivedPosition(),
                              pentity->getParentNode()->_getDerivedOrientation(),
                              pentity->getParentNode()->_getDerivedScale());

    
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
                closest_result = ray.getPoint(closest_distance);               
            }
        }   // */   
    }
    
    mSceneMgr->destroyQuery(m_pray_scene_query);
    
    // return the result
    if (closest_distance >= 0.0f)
    {
        // raycast success
        pResult = closest_result;
        return (true);
    }
    else
    {
        // raycast failed
        std::cout << "ERROR: Raycast failed" << std::endl;
        return (false);
    } 
}

// Get the mesh information for the given mesh.
// Code found in Wiki: www.ogre3d.org/wiki/index.php/RetrieveVertexData
void EditorState::getMeshInformation(const Ogre::MeshPtr mesh,
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

    // Run through the submeshes again, adding the data into the arrays
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

            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //      Ogre::Real* pReal;
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
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                    static_cast<unsigned long>(offset);
            }
        }

        ibuf->unlock();
        current_offset = next_offset;
    }
}


bool EditorState::GUIHandleShutdown(const CEGUI::EventArgs& e) {
	this->requestShutdown();
	return true;
}

bool EditorState::GUISaveWorld(const CEGUI::EventArgs& e) {
	if (!mWorldMgr->saveWorldData()) {
		// error when saving
		std::cerr << " ERROR: There was a problem saving the world data." << std::endl;
		return false;
	}
	return true;
} 
