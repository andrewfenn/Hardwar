/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2010  Andrew Fenn
    
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

#include "GuiTask.h"

namespace Client
{

/** Custom init to pipe the ogre data into the gui system */
GuiTask::GuiTask(Ogre::RenderWindow* rw, Ogre::SceneManager* sm)
{
   mPlatform = OGRE_NEW MyGUI::OgrePlatform();
   mPlatform->initialise(rw, sm, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "");
   mGUI = OGRE_NEW MyGUI::Gui();
   mGUI->initialise("core.xml", "./logs/mygui.log");
}

void GuiTask::init()
{
}

void GuiTask::shutdown()
{
   /* Delete MyGUI */
   mGUI->destroyAllChildWidget();
   mGUI->shutdown();

   mPlatform->shutdown();

   OGRE_DELETE mGUI;
   mGUI = 0;
   
   OGRE_DELETE mPlatform;
   mPlatform = 0;
}

void GuiTask::update()
{

}

}

