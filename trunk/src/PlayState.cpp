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
    mRoot         = Ogre::Root::getSingletonPtr();
	mRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE, "EditorSceneMgr" );
	
	mOverlayMgr   = Ogre::OverlayManager::getSingletonPtr();
	mSceneMgr     = mRoot->getSceneManager( "EditorSceneMgr" );
	mCamera       = mSceneMgr->createCamera( "EditCamera" );
	mViewport     = mRoot->getAutoCreatedWindow()->addViewport( mCamera );
	mGUIMgr		  = GUIManager::getSingletonPtr();

	/*mMouseOverlay     = mOverlayMgr->getByName( "Overlay/MousePointer" );
	mMousePointer     = mOverlayMgr->getOverlayElement( "MousePointer/Pointer" );
	mMouseOverlay->show();*/
	
	// Initialise CEGUI for user interface stuff
	mGUIMgr->initialise(mSceneMgr, mRoot->getAutoCreatedWindow());


	// setup some light
	Ogre::Light *light = mSceneMgr->createLight("Light1");
   light->setType(Ogre::Light::LT_POINT);
   light->setPosition(Ogre::Vector3(1000, 10000, 1000));
   light->setDiffuseColour(Ogre::ColourValue::White);
   light->setSpecularColour(Ogre::ColourValue::White);
   
   // Setup our camera position in the world
   mCamera->setPosition(Ogre::Vector3(25275, 59670, 25255));		
	mCamera->pitch((Ogre::Degree)-90);
	mCamera->roll((Ogre::Degree)180);   	
   
   mMouseY = mMouseX = mMouseRotX = mMouseRotY = 0;
}

void PlayState::exit( void ) {
    mSceneMgr->clearScene();
    mSceneMgr->destroyAllCameras();
    mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void PlayState::pause( void ) { }

void PlayState::resume( void ) { }

void PlayState::update( unsigned long lTimeElapsed ) {

}

void PlayState::keyPressed( const OIS::KeyEvent &e ) {
}

void PlayState::keyReleased( const OIS::KeyEvent &e ) {
    if( e.key == OIS::KC_ESCAPE ) {
        this->requestShutdown();
    }
}

void PlayState::mouseMoved( const OIS::MouseEvent &e ) {
    const OIS::MouseState &mouseState = e.state;
    mMouseX = mouseState.X.abs;
    mMouseY = mouseState.Y.abs;
    CEGUI::System::getSingleton().injectMousePosition(mouseState.X.abs, mouseState.Y.abs);
}

void PlayState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
 	 CEGUI::System::getSingleton().injectMouseButtonDown((CEGUI::MouseButton)id);
}

void PlayState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
}

PlayState* PlayState::getSingletonPtr( void ) {
    if( !mPlayState ) {
        mPlayState = new PlayState();
    }

    return mPlayState;
}

