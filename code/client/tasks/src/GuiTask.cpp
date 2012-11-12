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

#include "GuiTask.h"

namespace Client
{

/** Custom init to pipe the ogre data into the gui system */
GuiTask::GuiTask(Ogre::RenderWindow* rw, Ogre::SceneManager* sm)
{
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    mSystem = CEGUI::System::getSingletonPtr();
}

void GuiTask::init()
{
}

void GuiTask::shutdown()
{

}

void GuiTask::update()
{

}

void GuiTask::injectInput(const OIS::MouseState mouseState)
{
    mSystem->injectMouseMove(mouseState.X.rel, mouseState.Y.rel);
    mSystem->injectMouseWheelChange(mouseState.Z.rel);
}

}

