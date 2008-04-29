#ifndef PlayState_H
#define PlayState_H

#include <OgreCamera.h>

#include "GameState.h"
#include "PauseState.h"

class PlayState : public GameState {
public:
    ~PlayState( void );

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

    static PlayState* getSingletonPtr( void );
private:
    PlayState( void ) { }
    PlayState( const PlayState& ) { }
    PlayState & operator = ( const PlayState& );

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneMgr;
    Ogre::Viewport       *mViewport;
    Ogre::OverlayManager *mOverlayMgr;
    Ogre::Overlay        *mInfoOverlay;
    Ogre::Overlay        *mPlayOverlay;
    Ogre::Overlay        *mMouseOverlay;
    Ogre::OverlayElement *mMousePointer;
    Ogre::OverlayElement *mInfoInstruction;
    Ogre::OverlayElement *mInfoNotification;

    OIS::Keyboard        *mInputDevice;

    static PlayState *mPlayState;
};
#endif

