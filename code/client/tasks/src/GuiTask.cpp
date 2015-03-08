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
    ogreSystem = new SystemInterfaceOgre3D();
    Rocket::Core::SetSystemInterface(ogreSystem);

    Rocket::Core::Initialise();
    Rocket::Controls::Initialise();

    ogreRenderer = new RenderInterfaceOgre3D(rw->getWidth(), rw->getHeight());
    Rocket::Core::SetRenderInterface(ogreRenderer);


/*
    // Load the fonts from the path to the sample directory.
    Rocket::Core::FontDatabase::LoadFontFace(sample_path + "../../assets/Delicious-Roman.otf");
    Rocket::Core::FontDatabase::LoadFontFace(sample_path + "../../assets/Delicious-Bold.otf");
    Rocket::Core::FontDatabase::LoadFontFace(sample_path + "../../assets/Delicious-Italic.otf");
    Rocket::Core::FontDatabase::LoadFontFace(sample_path + "../../assets/Delicious-BoldItalic.otf");
*/
    context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(rw->getWidth(), rw->getHeight()));
    Rocket::Debugger::Initialise(context);

/*
    // Load the mouse cursor and release the caller's reference.
    Rocket::Core::ElementDocument* cursor = context->LoadMouseCursor(sample_path + "../../assets/cursor.rml");
    if (cursor)
        cursor->RemoveReference();

    Rocket::Core::ElementDocument* document = context->LoadDocument(sample_path + "../../assets/demo.rml");
    if (document)
    {
        document->Show();
        document->RemoveReference();
    }
*/
    // Add the application as a listener to Ogre's render queue so we can render during the overlay.
    sm->addRenderQueueListener((Ogre::RenderQueueListener*)this);
}

void GuiTask::init()
{
 
}

void GuiTask::shutdown()
{
    // Shutdown Rocket.
    context->RemoveReference();
    Rocket::Core::Shutdown();

    delete ogreSystem;
    ogreSystem = nullptr;

    delete ogreRenderer;
    ogreRenderer = nullptr;
}

void GuiTask::update()
{

}

void GuiTask::injectInput(const OIS::MouseState mouseState)
{
    /*mSystem->injectMouseMove(mouseState.X.rel, mouseState.Y.rel);
    mSystem->injectMouseWheelChange(mouseState.Z.rel);*/
}

}

