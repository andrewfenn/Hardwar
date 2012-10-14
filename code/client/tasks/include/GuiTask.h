/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2010-2012  Andrew Fenn

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

#include "GameTask.h"

namespace Client
{

class GuiTask : public GameTask
{
   public:
       GuiTask(Ogre::RenderWindow*, Ogre::SceneManager*);
       void init();
       void shutdown();
       void update();
//       MyGUI::Gui* resource();
   private:
/*       MyGUI::Gui *mGUI;
       MyGUI::OgrePlatform  *mPlatform;*/
};

}

