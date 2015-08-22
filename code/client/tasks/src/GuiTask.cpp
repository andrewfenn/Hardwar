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
    ogreRenderer = new RenderInterfaceOgre3D(rw->getWidth(), rw->getHeight());
    Rocket::Core::SetRenderInterface(ogreRenderer);

    ogreSystem = new SystemInterfaceOgre3D();
    Rocket::Core::SetSystemInterface(ogreSystem);

    Rocket::Core::Initialise();
    Rocket::Controls::Initialise();

//    Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Rocket");
//    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./media/menu", "FileSystem", "Rocket");

    Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Rocket");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("/home/andrew/programming/hardwar/build/bin/media/menu", "FileSystem", "Rocket");


    // Load the fonts from the path to the sample directory.
    Rocket::Core::FontDatabase::LoadFontFace("./media/menu/Delicious-Roman.otf");
    Rocket::Core::FontDatabase::LoadFontFace("./media/menu/Delicious-Bold.otf");
    Rocket::Core::FontDatabase::LoadFontFace("./media/menu/Delicious-Italic.otf");
    Rocket::Core::FontDatabase::LoadFontFace("./media/menu/Delicious-BoldItalic.otf");

    context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(rw->getWidth(), rw->getHeight()));
    Rocket::Debugger::Initialise(context);

    // Load the mouse cursor and release the caller's reference.
    //Rocket::Core::ElementDocument* cursor = context->LoadMouseCursor("./media/menu/cursor.rml");
    Rocket::Core::ElementDocument* cursor = context->LoadMouseCursor("/home/andrew/programming/hardwar/build/bin/media/menu/cursor.rml");
    if (cursor)
        cursor->RemoveReference();

    //Rocket::Core::ElementDocument* document = context->LoadDocument("./media/menu/demo.rml");
    Rocket::Core::ElementDocument* document = context->LoadDocument("/home/andrew/programming/hardwar/build/bin/media/menu/demo.rml");
    if (document)
    {
        document->Show();
        document->RemoveReference();
    }

    mWindow = rw;

    // Add the application as a listener to Ogre's render queue so we can render during the overlay.
    sm->addRenderQueueListener((Ogre::RenderQueueListener*)this);

    //Ogre::Root::getSingleton().addFrameListener(this);
}

// Called from Ogre before a queue group is rendered.
void GuiTask::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& /*invocation*/, bool& /*skipThisInvocation*/)
{
    if (queueGroupId == Ogre::RENDER_QUEUE_OVERLAY && Ogre::Root::getSingleton().getRenderSystem()->_getViewport()->getOverlaysEnabled())
    {
        context->Update();

        ConfigureRenderSystem();
        context->Render();
    }
}

// Called from Ogre after a queue group is rendered.
void GuiTask::renderQueueEnded(Ogre::uint8 /*queueGroupId*/, const Ogre::String& /*invocation*/, bool& /*repeatThisInvocation*/)
{

}

// Configures Ogre's rendering system for rendering Rocket.
void GuiTask::ConfigureRenderSystem()
{
    Ogre::RenderSystem* render_system = Ogre::Root::getSingleton().getRenderSystem();

    // Set up the projection and view matrices.
    Ogre::Matrix4 projection_matrix;
    BuildProjectionMatrix(projection_matrix);
    render_system->_setProjectionMatrix(projection_matrix);
    render_system->_setViewMatrix(Ogre::Matrix4::IDENTITY);

    // Disable lighting, as all of Rocket's geometry is unlit.
    render_system->setLightingEnabled(false);
    // Disable depth-buffering; all of the geometry is already depth-sorted.
    render_system->_setDepthBufferParams(false, false);
    // Rocket generates anti-clockwise geometry, so enable clockwise-culling.
    render_system->_setCullingMode(Ogre::CULL_CLOCKWISE);
    // Disable fogging.
    render_system->_setFog(Ogre::FOG_NONE);
    // Enable writing to all four channels.
    render_system->_setColourBufferWriteEnabled(true, true, true, true);
    // Unbind any vertex or fragment programs bound previously by the application.
    render_system->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
    render_system->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);

    // Set texture settings to clamp along both axes.
    Ogre::TextureUnitState::UVWAddressingMode addressing_mode;
    addressing_mode.u = Ogre::TextureUnitState::TAM_CLAMP;
    addressing_mode.v = Ogre::TextureUnitState::TAM_CLAMP;
    addressing_mode.w = Ogre::TextureUnitState::TAM_CLAMP;
    render_system->_setTextureAddressingMode(0, addressing_mode);

    // Set the texture coordinates for unit 0 to be read from unit 0.
    render_system->_setTextureCoordSet(0, 0);
    // Disable texture coordinate calculation.
    render_system->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
    // Enable linear filtering; images should be rendering 1 texel == 1 pixel, so point filtering could be used
    // except in the case of scaling tiled decorators.
    render_system->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);
    // Disable texture coordinate transforms.
    render_system->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
    // Reject pixels with an alpha of 0.
    render_system->_setAlphaRejectSettings(Ogre::CMPF_GREATER, 0, false);
    // Disable all texture units but the first.
    render_system->_disableTextureUnitsFrom(1);

    // Enable simple alpha blending.
    render_system->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

    // Disable depth bias.
    render_system->_setDepthBias(0, 0);
}

// Builds an OpenGL-style orthographic projection matrix.
void GuiTask::BuildProjectionMatrix(Ogre::Matrix4& projection_matrix)
{
    float z_near = -1;
    float z_far = 1;

    projection_matrix = Ogre::Matrix4::ZERO;

    // Set up matrices.
    projection_matrix[0][0] = 2.0f / mWindow->getWidth();
    projection_matrix[0][3]= -1.0000000f;
    projection_matrix[1][1]= -2.0f / mWindow->getHeight();
    projection_matrix[1][3]= 1.0000000f;
    projection_matrix[2][2]= -2.0f / (z_far - z_near);
    projection_matrix[3][3]= 1.0000000f;
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

void GuiTask::injectInput(const OIS::MouseState /*mouseState*/)
{
    /*mSystem->injectMouseMove(mouseState.X.rel, mouseState.Y.rel);
    mSystem->injectMouseWheelChange(mouseState.Z.rel);*/
}

}

