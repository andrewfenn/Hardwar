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
