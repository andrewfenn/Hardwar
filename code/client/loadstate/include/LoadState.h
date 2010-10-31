/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2009-2010  Andrew Fenn
    
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

#pragma once

#include <OgreRoot.h>
#include <libintl.h>

#include "srvstructs.h"
#include "hwstructs.h"

#include "GameState.h"
#include "PlayState.h"

#include "NetworkTask.h"
#include "ZoneTask.h"

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
    LoadState() : GameState("loadstate") {}
    ~LoadState();

    void enter();
    void exit();

    void update(unsigned long lTimeElapsed);
    void keyPressed(const OIS::KeyEvent &e);
    void keyReleased(const OIS::KeyEvent &e);

    void mouseMoved(const OIS::MouseEvent &e );
    void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

private:
    clientStatus mLoadStatus;

    NetworkTask* mNetwork;
    ZoneTask* mLevel;

    MyGUI::VectorWidgetPtr mLayout;

    void updateLoadbar();
    void killLoadbar();

    unsigned long mCounter; /* keeps track of time between connection attempts */
    unsigned short mGUIcount; /* used for GUI animation */
    bool mReverse; /* use for GUI animation */
    unsigned long mGUICounter; /* keeps track of time between animation updates */
    MyGUI::StaticTextPtr mStatusText;
    bool mDownloads;
};
}

