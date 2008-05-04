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

#ifndef IntroState_H
#define IntroState_H

#include "PlayState.h"
#include "GameState.h"

class IntroState : public GameState {
public:
    ~IntroState( void );

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

    static IntroState* getSingletonPtr( void );
private:
    IntroState( void ) { }
    IntroState( const IntroState& ) { }
    IntroState & operator = ( const IntroState& );

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneMgr;
    Ogre::Viewport       *mViewport;
    Ogre::OverlayManager *mOverlayMgr;
    Ogre::Overlay        *mIntroOverlay;
    Ogre::Overlay        *mMouseOverlay;
    Ogre::OverlayElement *mMousePointer;
    static IntroState    *mIntroState;
};
#endif

