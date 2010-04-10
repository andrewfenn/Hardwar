/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2008-2010  Andrew Fenn
    
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

#include "BuildEditor.h"

using namespace Client;

BuildEditor::BuildEditor(void)
{
   mCollision = new MOC::CollisionTools(Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr"));
   mEditorObjCreated = mEditorObjEditing = false;
   mRoot = Ogre::Root::getSingletonPtr();
   generateBuildingList();
}

BuildEditor::~BuildEditor(void)
{
   /* TODO: unload build editor resources */
   delete mCollision;
}


void BuildEditor::generateBuildingList(void)
{
   boost::filesystem::path lPath("../media/models/hangers");
   /* Loop over all files in the directory and if they are .mesh place them in our building list. */
	if (boost::filesystem::is_directory(lPath))
   {
	   for (boost::filesystem::directory_iterator itr(lPath); itr!=boost::filesystem::directory_iterator(); ++itr)
      {
	      Ogre::String temp = (Ogre::UTFString) itr->path().leaf();
	      if (temp.substr(temp.length()-4, 4).compare("mesh") == 0)
         {
            mUI.addMesh(itr->path().leaf());
	      }
	   }
	}
}

bool BuildEditor::isVisible()
{
   return mUI.isVisible();
}

void BuildEditor::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   mUI.mousePressed(e,id);
   if (id == OIS::MB_Left)
   {
      const OIS::MouseState &mouseState = e.state;
      if ((mouseState.X.abs > 360 || mouseState.Y.abs > 570) && mUI.isVisible())
      {
         mAxis.selectBuilding(makeRay(mouseState.X.abs, mouseState.Y.abs));
      }
   }
}

void BuildEditor::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   if (mEditorObjCreated && mUI.hasPlaceableIcon())
   {
      /* We have just let go of an object and placed it. */
      Hardwar::Building building;
      building.setPosition(mEditorNode->getPosition());
      building.setMeshName(mEditorObjMeshName);

      /* Tell the server to place down a new building */
      dataPacket lPacket = dataPacket(add_building);
      lPacket = building.serialize(lPacket);

      GameManager::getSingletonPtr()->getNetwork()->message(lPacket, SERVER_CHANNEL_ADMIN, ENET_PACKET_FLAG_RELIABLE);
   }

   mUI.mouseReleased(e, id);

   if (mAxis.axisSelected())
   {
      Ogre::MovableObject* lObj = mAxis.getSelectedObj();

      if (lObj)
      {
         GameManager* lGameMgr = GameManager::getSingletonPtr();

         // Building possibly moved
         Ogre::SceneNode * lNode = lObj->getParentSceneNode();
         Ogre::Vector3 lPosition = lNode->getPosition();
         Ogre::Quaternion lRotation = lNode->getOrientation();
         Zone* zone = lGameMgr->getZoneMgr()->getCurrent();

         Hardwar::Building building = zone->getBuildingByName(lNode->getName());

         // Send new location
         building.setPosition(lNode->getPosition());
         building.setRotation(lNode->getOrientation());
         building.setMeshName(lNode->getName());

         /* Tell the server to place down a new building */
         dataPacket lPacket = dataPacket(edit_building);
         lPacket = building.serialize(lPacket);
         lGameMgr->getNetwork()->message(lPacket, SERVER_CHANNEL_ADMIN, ENET_PACKET_FLAG_RELIABLE);
      }

      mAxis.clearSelectedAxis();
   }
}

void BuildEditor::mouseMoved(const OIS::MouseEvent &e)
{
   mUI.mouseMoved(e);
   if (mAxis.getSelectedAxis() != 0 && mUI.isVisible())
   {
      const OIS::MouseState &mouseState = e.state;
      mAxis.moveBuilding(makeRay(mouseState.X.abs, mouseState.Y.abs));
   }
}

void BuildEditor::keyReleased(const OIS::KeyEvent &e)
{
   if (e.key == OIS::KC_LSHIFT)
   {
      mAxis.showRotAxis(!mAxis.isRotAxis());
   }
}

Ogre::Ray BuildEditor::makeRay(const unsigned int x, const unsigned int y)
{
   Ogre::Camera* lCamera = GameManager::getSingletonPtr()->getCamera();
   Ogre::Viewport* lViewport = GameManager::getSingletonPtr()->getViewport();
   return lCamera->getCameraToViewportRay(x / Ogre::Real(lViewport->getActualWidth()), 
                                          y / Ogre::Real(lViewport->getActualHeight()));
}

void BuildEditor::update(unsigned long lTimeElapsed)
{
   mUI.update(lTimeElapsed);
   if (mUI.isVisible())
   {
      if (mAxis.objectSelected())
      {
         mUI.showEditPane(true);
      }
      else
      {
         mUI.showEditPane(false);
         if (mUI.hasPlaceableIcon())
         {
            Ogre::Vector3 lResult = Ogre::Vector3::ZERO;
            unsigned long lTarget = 0;
            float lDistance = -1.0f;

            GameManager * lGameMgr = GameManager::getSingletonPtr();
            Ogre::Ray lmouseRay = lGameMgr->getCamera()->getCameraToViewportRay(mUI.getIconCoords().left / Ogre::Real(lGameMgr->getViewport()->getActualWidth()),
                                                                            mUI.getIconCoords().top  / Ogre::Real(lGameMgr->getViewport()->getActualHeight()));

            Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");

            if(mCollision->raycast(lmouseRay, lResult, (unsigned long&)lTarget, lDistance))
            {
               Ogre::Entity* lEntity;
               if (mEditorObjCreated)
               {
                  lEntity = (Ogre::Entity*) lSceneMgr->getSceneNode("EditorNode")->getAttachedObject("EditorObject");
               }
               else
               {
                  mEditorNode = lSceneMgr->getRootSceneNode()->createChildSceneNode("EditorNode");
                  /* The object we're placing hasn't been loaded yet. */
                  mEditorObjMeshName = mUI.getIconName();
                  lEntity = lSceneMgr->createEntity("EditorObject", mEditorObjMeshName);
                  lEntity->setQueryFlags(0x00000000);
                  mEditorNode->attachObject(lEntity);
                  mEditorObjCreated = true;
               }

               if (lTarget != 0)
               {
                  Ogre::Entity* lTarg;
                  lTarg = (Ogre::Entity*)lTarget;
                  /* If we're positioned over a building then place our placing object over the top of it */
                  if (Ogre::StringUtil::startsWith(lTarg->getName(), Ogre::String("Building"), false))
                  {
                     lResult = lTarg->getParentSceneNode()->getPosition();
                     lResult.y += lTarg->getBoundingBox().getSize().y;
                  }
               }

               /* We want ints because that's what is going over the network */
               lResult.x = Ogre::Math::Floor(lResult.x);
               lResult.z = Ogre::Math::Floor(lResult.z);
               lResult.y = Ogre::Math::Floor(lResult.y);
               mEditorNode->setPosition(lResult);
            }
         }
         else
         {
            if (mEditorObjCreated)
            {
               destroyEditorObj();
            }
         }
      }
   }
}

void BuildEditor::destroyEditorObj()
{
   /* Destroy the editor object */
   mEditorNode->detachObject("EditorObject");
   Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr")->destroyEntity("EditorObject");
   Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr")->destroySceneNode(mEditorNode);
   mEditorObjCreated = false;
}

void BuildEditor::show(bool lShow)
{
   mUI.show(lShow);
   if (!lShow)
   {
      mAxis.remove();
   }
}
