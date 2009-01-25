/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2009  Andrew Fenn
    
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

#ifndef __LoadState_H_
#define __LoadState_H_

#include "GameState.h"
#include "srvstructs.h"
#include "PlayState.h"

#include <OgreRoot.h>
#include <OgreEntity.h>
#include <OgreSceneQuery.h>
#include <OgreSubMesh.h>
#include <OgreRay.h>
#include <OgreMeshManager.h>
#include <OgreRenderable.h>

#include "enet/enet.h"

class LoadState : public GameState
{
public:
    ~LoadState( void );

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

    static LoadState* getSingletonPtr( void );
private:

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneMgr;
    Ogre::RenderWindow   *mWindow;
    Ogre::Viewport       *mViewport;
    Ogre::OverlayManager *mOverlayMgr;
    static LoadState    *mLoadState;
    GameManager* mGameMgr;
    clientStatus         mLoadStatus;

    bool mReverse;
    int mConAttempts;
    int mRetryLimit;
    int mTimeout;

    LoadState( void ) { }
    LoadState( const LoadState& ) { }
    LoadState & operator = ( const LoadState& );

    void updateLoadbar(unsigned long);
    void killLoadbar(void);
    void connect(void);
    void waitForReply(void);
    unsigned long mCounter;
};
#endif /* __LoadState_H_ */

