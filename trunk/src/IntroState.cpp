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

#include "IntroState.h"

using namespace Ogre;

IntroState* IntroState::mIntroState;

void IntroState::enter( void ) {

	mRoot         = Root::getSingletonPtr();
	mOverlayMgr   = OverlayManager::getSingletonPtr();
	mSceneMgr     = mRoot->getSceneManager( "ST_GENERIC" );
	mCamera       = mSceneMgr->createCamera( "IntroCamera" );
	mViewport     = mRoot->getAutoCreatedWindow()->addViewport( mCamera );

	mMouseOverlay     = mOverlayMgr->getByName( "Overlay/MousePointer" );
	mMousePointer     = mOverlayMgr->getOverlayElement( "MousePointer/Pointer" );
	mMouseOverlay->show();
	
	mIntroOverlay = mOverlayMgr->getByName( "Overlay/IntroState" );
	mIntroOverlay->show();
    
}

void IntroState::exit( void ) {
    mIntroOverlay->hide();
    
    mSceneMgr->clearScene();
    mSceneMgr->destroyAllCameras();
    mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void IntroState::pause( void ) {
    mIntroOverlay->hide();
}

void IntroState::resume( void ) {
    mIntroOverlay->show();
}

void IntroState::update( unsigned long lTimeElapsed ) {

}

void IntroState::keyPressed( const OIS::KeyEvent &e ) {
}

void IntroState::keyReleased( const OIS::KeyEvent &e ) {
    if( e.key == OIS::KC_ESCAPE ) {
        this->requestShutdown();
    }
    else {
        this->changeState( PlayState::getSingletonPtr() );
    }
}

void IntroState::mouseMoved( const OIS::MouseEvent &e ) {
    const OIS::MouseState &mouseState = e.state;
    mMousePointer->setTop(mouseState.Y.abs);
    mMousePointer->setLeft(mouseState.X.abs);
}

void IntroState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
}

void IntroState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    this->changeState( PlayState::getSingletonPtr() );
}

IntroState* IntroState::getSingletonPtr( void ) {
    if( !mIntroState ) {
        mIntroState = new IntroState();
    }

    return mIntroState;
}
