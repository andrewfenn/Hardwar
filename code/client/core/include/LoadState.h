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

#ifndef __LOADSTATE_H_
#define __LOADSTATE_H_

#include <OgreRoot.h>
#include <libintl.h>

#include "GameState.h"
#include "srvstructs.h"
#include "hwstructs.h"
#include "PlayState.h"
#include "enet/enet.h"
#include "OgreMaxScene.hpp"

namespace Client
{
/** The Load State
        @remarks
            The Client::LoadState class is where a new game is loaded up from.
        This class deals with animation of the loading screen, starting up a
        thread from Client::Network and connecting to the server.
    */
class LoadState : public GameState
{
public:
    ~LoadState(void);

    void enter(void);
    void exit(void);

    void pause(void);
    void resume(void);
    void update(unsigned long lTimeElapsed);

    void keyPressed(const OIS::KeyEvent &e);
    void keyReleased(const OIS::KeyEvent &e);

    void mouseMoved(const OIS::MouseEvent &e );
    void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

    static LoadState* getSingletonPtr(void);
private:

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneMgr;
    Ogre::RenderWindow   *mWindow;
    Ogre::Viewport       *mViewport;
    static LoadState     *mLoadState;
    GameManager          *mGameMgr;
    clientStatus         mLoadStatus;

    MyGUI::VectorWidgetPtr mLayout;

    LoadState(void) { }
    LoadState(const LoadState&) { }
    LoadState & operator = (const LoadState&);

    void updateLoadbar(void);
    void killLoadbar(void);

    unsigned long mCounter; /* keeps track of time between connection attempts */

    unsigned short mGUIcount; /* used for GUI animation */
    bool mReverse; /* use for GUI animation */
    unsigned long mGUICounter; /* keeps track of time between animation updates */
    MyGUI::StaticTextPtr  mStatusText;
    bool mFilesLoaded;
    bool mDownloads;
};
}
#endif /* __LOADSTATE_H_ */

