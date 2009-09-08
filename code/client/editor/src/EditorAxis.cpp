/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2009  Andrew Fenn
    
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

#include "EditorAxis.h"

using namespace Client;

EditorAxis::EditorAxis(void)
{
   mEditorObjSelected = false;
   mSelected = 0;
   mSelectedAxis = 0;
   mCollision = new MOC::CollisionTools(Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr"));
   mAxisFlag = 100;
}

EditorAxis::~EditorAxis(void)
{
   delete mCollision;
}

void EditorAxis::remove(void)
{
   removeSelectedObj();
}

void EditorAxis::selectBuilding(const Ogre::Ray _ray)
{
   Ogre::Vector3 lResult = Ogre::Vector3::ZERO;
   Ogre::Entity *lTarget = 0;
   float lDistance = -1.0f;

   if(mCollision->raycast(_ray, lResult, (unsigned long&)lTarget, lDistance, mAxisFlag, false))
   {
      setSelectedAxis(lTarget);
      return;
   }

   if(mCollision->raycast(_ray, lResult, (unsigned long&)lTarget, lDistance))
   {
      if (lTarget != 0)
      {
         if (lTarget->getParentSceneNode()->getName() != "world" && lTarget->getParentSceneNode()->getName() != "EditorNode")
         {
            if (lTarget->getParentSceneNode()->getName() == "Axis")
            {
               setSelectedAxis(lTarget);
            }
            else
            {
               if (lTarget->getName() != "AxisX" && lTarget->getName() != "AxisY" && lTarget->getName() != "AxisZ")
               {
                  removeSelectedObj();

                  createAxis(lTarget);
                  lTarget->getParentSceneNode()->showBoundingBox(true);
                  mSelected = lTarget;
                  mEditorObjSelected = true;
               }
               else
               {
                  setSelectedAxis(lTarget);
               }
            }
         }
         else
         {
            removeSelectedObj();
         }
      } // !world && !editornode
   } // collision
}

void EditorAxis::removeSelectedObj(void)
{
   if (mEditorObjSelected)
   {
      destoryAxis(mSelected);
      mSelected->getParentSceneNode()->showBoundingBox(false);
      mSelected = 0;
      mEditorObjSelected = false;
   }
}

void EditorAxis::clearSelectedAxis(void)
{
   mSelectedAxis = 0;
}

void EditorAxis::moveBuilding(Ogre::Ray _ray)
{
   Ogre::Vector3 lResult = Ogre::Vector3::ZERO;
   Ogre::Entity *lTarget = 0;
   float lDistance = -1.0f;

   Ogre::SceneNode * lNode = mSelected->getParentSceneNode();
   Ogre::Vector3 lPosition = lNode->getPosition();

   if(mCollision->raycast(_ray, lResult, (unsigned long&)lTarget, lDistance))
   {
      if (mSelectedAxis->getName() == "AxisX")
      {
         lPosition.z = lResult.z;
      }
      else if (mSelectedAxis->getName() == "AxisY")
      {
         lPosition.x = lResult.x;
      }
      else if (mSelectedAxis->getName() == "AxisZ")
      {
         lPosition.y = lResult.y;
      }
      lNode->setPosition(lPosition);
   }
}

void EditorAxis::createAxis(Ogre::Entity * lTarget)
{
   if (lTarget != 0)
   {
      Ogre::Real lboundsPadding = Ogre::MeshManager::getSingleton().getBoundsPaddingFactor();
      Ogre::MeshManager::getSingleton().setBoundsPaddingFactor(1.0);

      Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");
      Ogre::Entity *lEntityX = lSceneMgr->createEntity("AxisX", "axis.mesh");
      lEntityX->setMaterialName("Axis/X");
      lEntityX->setQueryFlags(mAxisFlag);

      Ogre::Entity *lEntityY = lSceneMgr->createEntity("AxisY", "axis.mesh");
      lEntityY->setMaterialName("Axis/Y");
      lEntityY->setQueryFlags(mAxisFlag);

      Ogre::Entity *lEntityZ = lSceneMgr->createEntity("AxisZ", "axis.mesh");
      lEntityZ->setMaterialName("Axis/Z");
      lEntityZ->setQueryFlags(mAxisFlag);

      Ogre::MeshManager::getSingleton().setBoundsPaddingFactor(lboundsPadding);

      Ogre::SceneNode * lSceneNode = lTarget->getParentSceneNode()->createChildSceneNode("Axis");

      Ogre::SceneNode * lSceneNodeX = lSceneNode->createChildSceneNode("AxisX", Ogre::Vector3::ZERO, Ogre::Quaternion(0.5,0.5,0,0));
      lSceneNodeX->attachObject(lEntityX);
      lSceneNodeX->setScale(100,100,100);

      Ogre::SceneNode * lSceneNodeY = lSceneNode->createChildSceneNode("AxisY", Ogre::Vector3::ZERO, Ogre::Quaternion(0,0.5,0.5,0));
      lSceneNodeY->attachObject(lEntityY);
      lSceneNodeY->setScale(100,100,100);

      Ogre::SceneNode * lSceneNodeZ = lSceneNode->createChildSceneNode("AxisZ", Ogre::Vector3::ZERO, Ogre::Quaternion(0,0,0.5,0));
      lSceneNodeZ->attachObject(lEntityZ);
      lSceneNodeZ->setScale(100,100,100);
   }
}

void EditorAxis::destoryAxis(Ogre::MovableObject* lMoveable)
{
   Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");
   lMoveable->getParentSceneNode()->removeAndDestroyChild("Axis");

   if (lSceneMgr->hasEntity("AxisX"))
      lSceneMgr->destroyEntity("AxisX");
   if (lSceneMgr->hasEntity("AxisY"))
      lSceneMgr->destroyEntity("AxisY");
   if (lSceneMgr->hasEntity("AxisZ"))
      lSceneMgr->destroyEntity("AxisZ");
}

void EditorAxis::setSelectedAxis(Ogre::Entity* lAxis)
{
   mSelectedAxis = lAxis;
}

Ogre::Entity* EditorAxis::getSelectedAxis(void)
{
   return mSelectedAxis;
}
