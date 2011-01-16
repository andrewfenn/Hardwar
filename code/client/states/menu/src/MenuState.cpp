/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2010-2011  Andrew Fenn
    
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

#include <MyGUI.h>

#include "MenuState.h"
#include "LoadState.h"
#include "InputTask.h"
#include "GuiTask.h"

using namespace Client;

MenuState::MenuState() : GameState("menustate"){}

MenuState::~MenuState()
{
   InputTask* input = (InputTask*) mTasklist->get("Input");
   input->removeKeyListener(this->getName());
   input->removeMouseListener(this->getName());
   OGRE_DELETE mFade;
}

void MenuState::enter()
{
   mSceneMgr->clearScene();
   Ogre::SceneNode* sceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("menu");

   InputTask* input = (InputTask*) mTasklist->get("Input");
   input->addKeyListener(this, this->getName());
   input->addMouseListener(this, this->getName());

   mSceneMgr->setSkyBox(true, "Menu/MedResSkyBox", 5000);
   mFade = OGRE_NEW Helper::Fader("Menu/FadeInOut", "Materials/OverlayMaterial", this);
   mFade->startFadeIn(2);
   mStarted = false;
   
   if (mSceneMgr->hasLight("Sun"))
   {
      mSceneMgr->destroyLight("Sun");
   }

	Ogre::Light* light = mSceneMgr->createLight( "Sun" );
	light->setType( Ogre::Light::LT_SPOTLIGHT );
	light->setDiffuseColour( Ogre::ColourValue( .82, .81, .7 ) );
	light->setSpecularColour( Ogre::ColourValue( .82, .81, .7 ) );
	light->setPosition( Ogre::Vector3( 300, 0, -10000 ) ); 
   mSceneMgr->setAmbientLight(Ogre::ColourValue(0.1, 0.1, 0.1));
   
	Ogre::BillboardSet* bbs = mSceneMgr->createBillboardSet(1);
	bbs->setMaterialName("menu/sun");
	bbs->createBillboard(0, 0, 0, Ogre::ColourValue(1, 1, 1));
	Ogre::SceneNode* sunnode = sceneNode->createChildSceneNode("sun");
	sunnode->attachObject(bbs);
   sunnode->setScale(Ogre::Vector3(10.0,10.0,10.0));
	sunnode->setPosition(Ogre::Vector3( 300, 0, -10000 ));

   Ogre::Entity* ent = mSceneMgr->createEntity("planet", Ogre::SceneManager::PT_SPHERE);
   ent->setMeshLodBias(1.0, 0, 0);
   ent->setMaterialLodBias(1.0, 0, 0);
   ent->setMaterialName("menu/titan");

   Ogre::SceneNode* planetnode = sceneNode->createChildSceneNode("planet");
   planetnode->attachObject(ent);
   planetnode->setPosition(Ogre::Vector3(-200,0,-800));
   planetnode->setScale(Ogre::Vector3(1,1,1));
 
   ent = mSceneMgr->createEntity("planetclouds1", Ogre::SceneManager::PT_SPHERE);
   ent->setMeshLodBias(4, 0, 0);
   ent->setMaterialLodBias(4, 0, 0);
   ent->setMaterialName("menu/titan_clouds1");

   Ogre::SceneNode* planetcloudnode = planetnode->createChildSceneNode("planetclouds1");
   planetcloudnode->attachObject(ent);
   planetcloudnode->setScale(Ogre::Vector3(1.05,1.05,1.05));

   ent = mSceneMgr->createEntity("planetclouds2", Ogre::SceneManager::PT_SPHERE);
   ent->setMeshLodBias(1.0, 0, 0);
   ent->setMaterialLodBias(1.0, 0, 0);
   ent->setMaterialName("menu/titan_clouds2");

   planetcloudnode = planetnode->createChildSceneNode("planetclouds2");
   planetcloudnode->attachObject(ent);
   planetcloudnode->setScale(Ogre::Vector3(1.06,1.06,1.06));

   mCamera->setPosition(Ogre::Vector3(0, 0, 0));
   mCamera->setNearClipDistance( 0.2f );

   mCount = 0;
}

void MenuState::update(const unsigned long timeElapsed )
{
   if (mStarted)
   {
      // skip first frame to get accurate elapsed time
      mFade->fade(timeElapsed);
   }
   
   if (timeElapsed < 10)
   {
      mStarted = true;
   }

   if (mStarted && mFade->finished())
   {
      mCount += 1;
      if (mCount > 60*1)
      {
         this->replace(OGRE_NEW LoadState());
      }
   }

   return;

   Ogre::Vector3 translateVector = Ogre::Vector3::ZERO;
   float scale = 0.9f;

   if (mKeydownUp == 1)
     translateVector.z -= scale*timeElapsed;

   if (mKeydownDown == 1)
     translateVector.z += scale*timeElapsed;

   if (mKeydownLeft == 1)
     translateVector.x -= scale*timeElapsed;

   if (mKeydownRight == 1)
     translateVector.x += scale*timeElapsed;

   mCamera->yaw(mMouseRotX);
   mCamera->pitch(mMouseRotY);    
   translateVector = mCamera->getOrientation() * translateVector;
   mCamera->setPosition(mCamera->getPosition()+translateVector);

   mMouseRotX = mMouseRotY = 0;
}

bool MenuState::keyPressed(const OIS::KeyEvent &e)
{
   if (e.key == OIS::KC_W)
      mKeydownUp = 1;

   if (e.key == OIS::KC_S)
      mKeydownDown = 1;

   if (e.key == OIS::KC_A)
      mKeydownLeft = 1;

   if (e.key == OIS::KC_D)
      mKeydownRight = 1; 

   return true;
}

bool MenuState::keyReleased(const OIS::KeyEvent &e)
{
   if( e.key == OIS::KC_ESCAPE )
   {
      /* FIXME: Should go to main menu */
      this->shutdown();
   }
   
   if (e.key == OIS::KC_W)
      mKeydownUp = 0;

   if (e.key == OIS::KC_S)
      mKeydownDown = 0;

   if (e.key == OIS::KC_A)
      mKeydownLeft = 0;

   if (e.key == OIS::KC_D)
      mKeydownRight = 0;

   return true;
}

bool MenuState::mouseMoved(const OIS::MouseEvent &e)
{
//   ((GuiTask*) mTasklist->get("Gui"))->mGUI->injectMouseMove(e.state.X.abs, e.state.Y.abs, e.state.Z.abs);
   
   const OIS::MouseState &mouseState = e.state;
   mMouseRotX = Ogre::Degree(-mouseState.X.rel * 0.13);
   mMouseRotY = Ogre::Degree(-mouseState.Y.rel * 0.13);
   return true;
}

bool MenuState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ 
   return true;
}

bool MenuState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ 
   return true;
}
