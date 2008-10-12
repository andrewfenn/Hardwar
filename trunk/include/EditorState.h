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

#ifndef EditorState_H
#define EditorState_H


#include "GameState.h"
#include <OgreEntity.h>
#include <OgreSceneQuery.h>
#include <OgreSubMesh.h>
#include <OgreRay.h>
#include <OgreMeshManager.h>
#include <OgreRenderable.h>
#include "WorldManager.h"
#include "boost/filesystem.hpp"

class EditorState : public GameState {
public:
    ~EditorState( void );

    void enter( void );
    void exit( void );

    void pause( void );
    void resume( void );
    void update( unsigned long lTimeElapsed );

    void keyPressed( const OIS::KeyEvent &e );
    void keyReleased( const OIS::KeyEvent &e );

    void mouseMoved( const OIS::MouseEvent &e );
    void mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    void mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

    static EditorState* getSingletonPtr( void );
private:

    EditorState( void ) { }
    EditorState( const EditorState& ) { }
    EditorState & operator = ( const EditorState& );

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneMgr;
    Ogre::Viewport       *mViewport;
    Ogre::OverlayManager *mOverlayMgr;
    Ogre::Overlay        *mMouseOverlay;
    Ogre::OverlayElement *mMousePointer;
    Ogre::LogManager     *mLogMgr;
	 GUIManager			  *mGUIMgr;
    Ogre::SceneNode *mWorldNode;
    static EditorState    *mEditorState;
     
    unsigned int mScreenshots; // keeps tabs on what number to call your screenshot file
    
    int mMouseX, mMouseY, mMouseRotX, mMouseRotY; // up, down, last up, last down
    bool mMouseDownButton2, zoomin, zoomout; // right click, up key, down key
    Ogre::Vector3 CamRotatePos, // last rotate position while mouse is down
    					CamLookAtPos; // direction to be looking at
    
    WorldManager *mWorldMgr;// World manager, manages the craters build placement, etc
        
    Ogre::MovableObject *mSelectedObject; // The object the user has currently selected
    												// also used in loading terrain files

    void updateCamera(); // Update the editor camera
    bool addBuilding(); // Gets position of terrain and adds a building at that position
    void changeSelectedObject(); // changes position and rotate of the current selected object
    
    // Helper functions pinched from the Ogre Wiki for raycasting on polygons
    // See the cpp file for more details
    bool RaycastFromPoint(const Ogre::Ray ray, Ogre::Vector3 &result);
    void getMeshInformation(const Ogre::MeshPtr mesh,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale);
                                
	 // GUI functions
	 bool mOnCEGUI; // if true don't process any key presses for our non-CEGUI functions
	 void setupEditorUI();
	 void updateGUIEditPos(); // adds new coords to the edit position boxes and enables them
	 void resetGUIEditPos(); // clears the edit position boxes and disables them
	 bool GUIHandleShutdown(const CEGUI::EventArgs&);
	 bool GUISaveWorld(const CEGUI::EventArgs&);
 	 bool CEGUIActivated(const CEGUI::EventArgs&);
	 bool CEGUIDeactivated(const CEGUI::EventArgs&);
};
#endif

