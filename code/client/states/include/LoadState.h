/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2009-2012  Andrew Fenn

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

#include <CEGUI.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include "GameState.h"
#include "NetworkTask.h"


namespace Client
{
/** The Load State
        @remarks
            The Client::LoadState class is where a new game is loaded up from.
        This class deals with animation of the loading screen, starting up a
        thread from Client::Network and connecting to the server.
    */
class LoadState : public GameState, OIS::KeyListener, OIS::MouseListener
{
public:
    LoadState() : GameState("loadstate") {}
    ~LoadState();

    void enter();

    void update(unsigned long timeElapsed);
    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);

    bool mouseMoved(const OIS::MouseEvent &e );
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

private:
    clientStatus mLoadStatus;

    NetworkTask* mNetwork;

//    MyGUI::VectorWidgetPtr mLayout;

    void updateLoadbar();
    void killLoadbar();

    unsigned long mCounter; /* keeps track of time between connection attempts */
    unsigned short mGUIcount; /* used for GUI animation */
    bool mReverse; /* use for GUI animation */
    unsigned long mGUICounter; /* keeps track of time between animation updates */
//    MyGUI::StaticTextPtr mStatusText;
    bool mDownloads;
};
}

