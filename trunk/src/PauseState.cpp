#include "PauseState.h"

using namespace Ogre;

PauseState* PauseState::mPauseState;

void PauseState::enter( void ) {
    mRoot            = Root::getSingletonPtr();
    mOverlayMgr      = OverlayManager::getSingletonPtr();
    mViewport        = mRoot->getAutoCreatedWindow()->getViewport( 0 );

    mInfoOverlay     = mOverlayMgr->getByName( "Overlay/Info" );
    mPauseOverlay    = mOverlayMgr->getByName( "Overlay/PauseState" );
    mMouseOverlay    = mOverlayMgr->getByName( "Overlay/MousePointer" );

    mInfoInstruction = mOverlayMgr->getOverlayElement( "Info/Instruction" );
    mMousePointer    = mOverlayMgr->getOverlayElement( "MousePointer/Pointer" );

    mInfoOverlay->show();
    mPauseOverlay->show();
    mMouseOverlay->show();

    mInfoInstruction->setCaption( "Press space to return" );
}

void PauseState::exit( void ) {
    mInfoOverlay->hide();
    mPauseOverlay->hide();
    mMouseOverlay->hide();
}

void PauseState::pause( void ) {
    mInfoOverlay->hide();
    mPauseOverlay->hide();
    mMouseOverlay->hide();
}

void PauseState::resume( void ) {
    mInfoOverlay->show();
    mPauseOverlay->show();
    mMouseOverlay->show();

    mInfoInstruction->setCaption( "Press space to return" );
}

void PauseState::update( unsigned long lTimeElapsed ) {
    // Update wat je moet updaten
}

void PauseState::keyPressed( const OIS::KeyEvent &e ) {
}

void PauseState::keyReleased( const OIS::KeyEvent &e ) {
    if( e.key == OIS::KC_SPACE ) {
        this->popState();
    }
    else if( e.key == OIS::KC_ESCAPE ) {
        this->requestShutdown();
    }
}

void PauseState::mouseMoved( const OIS::MouseEvent &e ) {
    const OIS::MouseState &mouseState = e.state;
//    mMousePointer->setTop(mouseState.Y);
//    mMousePointer->setLeft(mouseState.X);
}

void PauseState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
}

void PauseState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
}

PauseState* PauseState::getSingletonPtr( void ) {
    if( !mPauseState ) {
        mPauseState = new PauseState();
    }

    return mPauseState;
}
