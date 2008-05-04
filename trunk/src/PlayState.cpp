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

#include "PlayState.h"
#include <OgreTextureUnitState.h>

using namespace Ogre;

PlayState* PlayState::mPlayState;

void PlayState::enter( void ) {
    mRoot             = Root::getSingletonPtr();
    mOverlayMgr       = OverlayManager::getSingletonPtr();
    mInputDevice      = InputManager::getSingletonPtr()->getKeyboard();
    mSceneMgr         = mRoot->getSceneManager( "ST_GENERIC" );
    mCamera           = mSceneMgr->createCamera( "PlayCamera" );
    mViewport         = mRoot->getAutoCreatedWindow()->addViewport( mCamera );

    mInfoOverlay      = mOverlayMgr->getByName( "Overlay/Info" );
    mPlayOverlay      = mOverlayMgr->getByName( "Overlay/PlayState" );
    mMouseOverlay     = mOverlayMgr->getByName( "Overlay/MousePointer" );

    mInfoInstruction  = mOverlayMgr->getOverlayElement( "Info/Instruction" );
    mInfoNotification = mOverlayMgr->getOverlayElement( "Info/Notification" );
    mMousePointer     = mOverlayMgr->getOverlayElement( "MousePointer/Pointer" );

    mInfoOverlay->show();
    mPlayOverlay->show();
    mMouseOverlay->show();

    mInfoInstruction->setCaption( "Press space for pause" );
}

void PlayState::exit( void ) {
    mInfoOverlay->hide();
    mPlayOverlay->hide();
    mMouseOverlay->hide();

    mSceneMgr->clearScene();
    mSceneMgr->destroyAllCameras();
    mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void PlayState::pause( void ) {
    mInfoOverlay->hide();
    mPlayOverlay->hide();
    mMouseOverlay->hide();
}

void PlayState::resume( void ) {
    mInfoOverlay->show();
    mPlayOverlay->show();
    mMouseOverlay->show();

    mInfoInstruction->setCaption( "Press space for pause" );
}

void PlayState::update( unsigned long lTimeElapsed ) {
    // Update wat je moet updaten
}

void PlayState::keyPressed( const OIS::KeyEvent &e ) {
}

void PlayState::keyReleased( const OIS::KeyEvent &e ) {
    if( e.key == OIS::KC_SPACE ) {
        this->pushState( PauseState::getSingletonPtr() );
    }
    else if( e.key == OIS::KC_ESCAPE ) {
        this->requestShutdown();
    }
}

void PlayState::mouseMoved( const OIS::MouseEvent &e ) {
    const OIS::MouseState &mouseState = e.state;
    mMousePointer->setTop(mouseState.Y.abs);
    mMousePointer->setLeft(mouseState.X.abs);
}

void PlayState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
}

void PlayState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
}

PlayState* PlayState::getSingletonPtr( void ) {
    if( !mPlayState ) {
        mPlayState = new PlayState();
    }

    return mPlayState;
}

