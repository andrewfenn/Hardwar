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

#include <OIS.h>
#include <Ogre.h>
#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include <Rocket/Debugger.h>

#include "rocket/RenderInterfaceOgre3D.h"
#include "rocket/SystemInterfaceOgre3D.h"

#include "GameTask.h"

namespace Client
{

class GuiTask : public GameTask, public Ogre::RenderQueueListener
{
    public:
        GuiTask(Ogre::RenderWindow*, Ogre::SceneManager*);
        void init();
        void shutdown();
        void update();

        /// Called from Ogre before a queue group is rendered.
        virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
        /// Called from Ogre after a queue group is rendered.
        virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);
        void mouseMoved(const OIS::MouseState mouseState);
        void mousePressed(const OIS::MouseState mouseState, OIS::MouseButtonID id);
        void mouseReleased(const OIS::MouseState mouseState, OIS::MouseButtonID id);
        Rocket::Core::Context* getRocket();
        //int getKeyModifierState();

    private:
        // Configures Ogre's rendering system for rendering Rocket.
        void ConfigureRenderSystem();
        // Builds an OpenGL-style orthographic projection matrix.
        void BuildProjectionMatrix(Ogre::Matrix4& matrix);

        typedef std::map<OIS::KeyCode, Rocket::Core::Input::KeyIdentifier> KeyIdentifierMap;
        KeyIdentifierMap       mKeyIdentifiers;
        Ogre::RenderWindow*    mWindow;
        Rocket::Core::Context* mContext;
        SystemInterfaceOgre3D* mOgreSystem;
        RenderInterfaceOgre3D* mOgreRenderer;
};

}

