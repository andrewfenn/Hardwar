#ifndef PauseState_H
#define PauseState_H

#include "PlayState.h"
#include "GameState.h"

class PauseState : public GameState {
public:
    ~PauseState( void ) { }

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

    static PauseState* getSingletonPtr( void );
private:
    PauseState( void ) { }
    PauseState( const PauseState& ) { }
    PauseState & operator = ( const PauseState& );

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneMgr;
    Ogre::Viewport       *mViewport;
    Ogre::OverlayManager *mOverlayMgr;
    Ogre::Overlay        *mInfoOverlay;
    Ogre::Overlay        *mPauseOverlay;
    Ogre::Overlay        *mMouseOverlay;
    Ogre::OverlayElement *mMousePointer;
    Ogre::OverlayElement *mInfoInstruction;

    static PauseState    *mPauseState;
};
#endif

