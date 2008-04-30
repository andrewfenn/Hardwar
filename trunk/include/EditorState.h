#ifndef EditorState_H
#define EditorState_H


#include "GameState.h"
#include "TerrainManager.h"
#include <OgreEntity.h>

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
    Ogre::Overlay        *mEditorOverlay;
    Ogre::Overlay        *mMouseOverlay;
    Ogre::OverlayElement *mMousePointer;
    static EditorState    *mEditorState;
    
    int mMouseX, mMouseY, mMouseRotX, mMouseRotY; // up, down, last up, last down
    bool mMouseDownButton2, zoomin, zoomout; // right click, up key, down key
    Ogre::Vector3 CamRotatePos; // last rotate position while mouse is down
    // Level manager
    TerrainManager *mTerrainMgr;
    
    Ogre::SceneNode *mObjectsNode; // attach all ground objects to this
    Ogre::SceneNode *mBuildingsNode; // attach all building meshs to this please
    
    void loadTerrain(void);
    
};
#endif

