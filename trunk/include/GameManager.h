#ifndef GameManager_H
#define GameManager_H

#include <OISMouse.h>
#include <OISKeyboard.h>

#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>

#include "InputManager.h"

class GameState;

class GameManager : public OIS::KeyListener, OIS::MouseListener {
public:
    ~GameManager( void );

    void startGame( GameState *gameState );

    void changeState( GameState *gameState );
    void pushState( GameState *gameState );
    void popState( void );
    void requestShutdown( void );

    static GameManager* getSingletonPtr( void );
private:
    GameManager( void );
    GameManager( const GameManager& ) { }
    GameManager & operator = ( const GameManager& );

    bool configureGame( void );
    void setupResources( void );

    bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e );

    bool mouseMoved( const OIS::MouseEvent &e );
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

    Ogre::Root         *mRoot;
    Ogre::RenderWindow *mRenderWindow;
    InputManager       *mInputMgr;

    GameState          *mIntroState;
    GameState          *mPlayState;
    GameState          *mPauseState;

    bool bShutdown;
    std::vector<GameState*> mStates;
    static GameManager *mGameManager;
};
#endif
