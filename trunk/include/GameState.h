#ifndef GameState_H
#define GameState_H

#include <OISMouse.h>
#include <OISKeyboard.h>

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreStringConverter.h>

#include "GameManager.h"
#include "InputManager.h"

class GameState {
public:
    ~GameState( void ) { }

    virtual void enter( void )  = 0;
    virtual void exit( void )   = 0;

    virtual void pause( void )  = 0;
    virtual void resume( void ) = 0;
    virtual void update( unsigned long lTimeElapsed ) = 0;
    
    virtual void keyPressed( const OIS::KeyEvent &e )  = 0;
    virtual void keyReleased( const OIS::KeyEvent &e ) = 0;

    virtual void mouseMoved( const OIS::MouseEvent &e ) = 0;
    virtual void mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )  = 0;
    virtual void mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) = 0;

    void changeState( GameState *state );
    void pushState( GameState *state );
    void popState( void );
    void requestShutdown( void );
protected:
    GameState( void ) { }
private:
    GameState( const GameState& ) { }
    GameState & operator = ( const GameState& );
};
#endif

