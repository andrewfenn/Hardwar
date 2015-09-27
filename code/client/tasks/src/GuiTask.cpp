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
#include "GameSettings.h"

namespace Client
{

/** Custom init to pipe the ogre data into the gui system */
GuiTask::GuiTask(Ogre::RenderWindow* rw, Ogre::SceneManager* sm)
{
    mOgreRenderer = new RenderInterfaceOgre3D(rw->getWidth(), rw->getHeight());
    Rocket::Core::SetRenderInterface(mOgreRenderer);

    mOgreSystem = new SystemInterfaceOgre3D();
    Rocket::Core::SetSystemInterface(mOgreSystem);

    Rocket::Core::Initialise();
    Rocket::Controls::Initialise();

    GameSettings* settings = GameSettings::getSingletonPtr();
    Ogre::String mediaDir = settings->getOption("DirectoryMedia");

    Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Rocket");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(mediaDir, "FileSystem", "Rocket");

    mContext = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(rw->getWidth(), rw->getHeight()));
    Rocket::Debugger::Initialise(mContext);
    Rocket::Debugger::SetVisible(true);

    mWindow = rw;

    // Add the application as a listener to Ogre's render queue so we can render during the overlay.
    sm->addRenderQueueListener((Ogre::RenderQueueListener*)this);

}

Rocket::Core::Context* GuiTask::getRocket()
{
    return mContext;
}

// Called from Ogre before a queue group is rendered.
void GuiTask::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& /*invocation*/, bool& /*skipThisInvocation*/)
{
    if (queueGroupId == Ogre::RENDER_QUEUE_OVERLAY && Ogre::Root::getSingleton().getRenderSystem()->_getViewport()->getOverlaysEnabled())
    {
        mContext->Update();

        ConfigureRenderSystem();
        mContext->Render();
    }
}

// Called from Ogre after a queue group is rendered.
void GuiTask::renderQueueEnded(Ogre::uint8 /*queueGroupId*/, const Ogre::String& /*invocation*/, bool& /*repeatThisInvocation*/)
{

}

// Configures Ogre's rendering system for rendering Rocket.
void GuiTask::ConfigureRenderSystem()
{
    Ogre::RenderSystem* renderSystem = Ogre::Root::getSingleton().getRenderSystem();

    // Set up the projection and view matrices.
    Ogre::Matrix4 projectionMatrix;
    BuildProjectionMatrix(projectionMatrix);
    renderSystem->_setProjectionMatrix(projectionMatrix);
    renderSystem->_setViewMatrix(Ogre::Matrix4::IDENTITY);

    // Disable lighting, as all of Rocket's geometry is unlit.
    renderSystem->setLightingEnabled(false);
    // Disable depth-buffering; all of the geometry is already depth-sorted.
    renderSystem->_setDepthBufferParams(false, false);
    // Rocket generates anti-clockwise geometry, so enable clockwise-culling.
    renderSystem->_setCullingMode(Ogre::CULL_CLOCKWISE);
    // Disable fogging.
    renderSystem->_setFog(Ogre::FOG_NONE);
    // Enable writing to all four channels.
    renderSystem->_setColourBufferWriteEnabled(true, true, true, true);
    // Unbind any vertex or fragment programs bound previously by the application.
    renderSystem->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
    renderSystem->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);

    // Set texture settings to clamp along both axes.
    Ogre::TextureUnitState::UVWAddressingMode addressing_mode;
    addressing_mode.u = Ogre::TextureUnitState::TAM_CLAMP;
    addressing_mode.v = Ogre::TextureUnitState::TAM_CLAMP;
    addressing_mode.w = Ogre::TextureUnitState::TAM_CLAMP;
    renderSystem->_setTextureAddressingMode(0, addressing_mode);

    // Set the texture coordinates for unit 0 to be read from unit 0.
    renderSystem->_setTextureCoordSet(0, 0);
    // Disable texture coordinate calculation.
    renderSystem->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
    // Enable linear filtering; images should be rendering 1 texel == 1 pixel, so point filtering could be used
    // except in the case of scaling tiled decorators.
    renderSystem->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);
    // Disable texture coordinate transforms.
    renderSystem->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
    // Reject pixels with an alpha of 0.
    renderSystem->_setAlphaRejectSettings(Ogre::CMPF_GREATER, 0, false);
    // Disable all texture units but the first.
    renderSystem->_disableTextureUnitsFrom(1);

    // Enable simple alpha blending.
    renderSystem->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    // Disable depth bias.
    renderSystem->_setDepthBias(0, 0);
}

// Builds an OpenGL-style orthographic projection matrix.
void GuiTask::BuildProjectionMatrix(Ogre::Matrix4& projectionMatrix)
{
    float z_near = -1;
    float z_far = 1;

    projectionMatrix = Ogre::Matrix4::ZERO;

    // Set up matrices.
    projectionMatrix[0][0] = 2.0f / mWindow->getWidth();
    projectionMatrix[0][3]= -1.0000000f;
    projectionMatrix[1][1]= -2.0f / mWindow->getHeight();
    projectionMatrix[1][3]= 1.0000000f;
    projectionMatrix[2][2]= -2.0f / (z_far - z_near);
    projectionMatrix[3][3]= 1.0000000f;
}

void GuiTask::init()
{

}

void GuiTask::shutdown()
{
    // Shutdown Rocket.
    mContext->RemoveReference();
    Rocket::Core::Shutdown();

    delete mOgreSystem;
    mOgreSystem = nullptr;

    delete mOgreRenderer;
    mOgreRenderer = nullptr;
}

void GuiTask::update()
{

}

void GuiTask::mouseMoved(const OIS::MouseState mouseState)
{
    int key_modifier_state = 0;

    mContext->ProcessMouseMove(mouseState.X.abs, mouseState.Y.abs, key_modifier_state);
    if (mouseState.Z.rel != 0) {
        mContext->ProcessMouseWheel(mouseState.Z.rel / -120, key_modifier_state);
    }
}

void GuiTask::mousePressed(const OIS::MouseState mouseState, OIS::MouseButtonID id)
{
    int key_modifier_state = 0;
    mContext->ProcessMouseButtonDown((int) id, key_modifier_state);
}

void GuiTask::mouseReleased(const OIS::MouseState mouseState, OIS::MouseButtonID id)
{
    int key_modifier_state = 0;
    mContext->ProcessMouseButtonUp((int) id, key_modifier_state);
}

}

