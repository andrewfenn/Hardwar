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
	 GUIManager			  *mGUIMgr;
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
	 void setupEditorUI();
	 bool GUIHandleShutdown(const CEGUI::EventArgs&);
};
#endif

