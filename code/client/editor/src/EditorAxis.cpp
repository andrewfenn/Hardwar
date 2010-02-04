/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2009-2010  Andrew Fenn
    
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
   mCollisionFlag = 101;
   mRotAxisShown = false;
}

EditorAxis::~EditorAxis(void)
{
   delete mCollision;
}

void EditorAxis::remove(void)
{
   removeSelectedObj();
}

void EditorAxis::updateSelectedUI(void)
{
   MyGUI::EditPtr edit;
   edit =  MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::Edit>("TransX");
   edit->eraseText(0, edit->getTextLength());
   edit->addText(Ogre::StringConverter::toString(mSelected->getParentNode()->getPosition().z));

   edit =  MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::Edit>("TransY");
   edit->eraseText(0, edit->getTextLength());
   edit->addText(Ogre::StringConverter::toString(mSelected->getParentNode()->getPosition().x));

   edit =  MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::Edit>("TransZ");
   edit->eraseText(0, edit->getTextLength());
   edit->addText(Ogre::StringConverter::toString(mSelected->getParentNode()->getPosition().y));

   Ogre::Quaternion rot = mSelected->getParentNode()->getOrientation();

   edit =  MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::Edit>("RotX");
   edit->eraseText(0, edit->getTextLength());
   edit->addText(Ogre::StringConverter::toString(2*Math::ACos(rot.x)));

   edit =  MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::Edit>("RotY");
   edit->eraseText(0, edit->getTextLength());
   edit->addText(Ogre::StringConverter::toString(2*Math::ACos(rot.y)));

   edit =  MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::Edit>("RotZ");
   edit->eraseText(0, edit->getTextLength());
   edit->addText(Ogre::StringConverter::toString(2*Math::ACos(rot.z)));
}

bool EditorAxis::objectSelected(void)
{
   return mEditorObjSelected;
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
               if (lTarget->getName() != "AxisX" && lTarget->getName() != "AxisY" && 
                   lTarget->getName() != "AxisZ" && lTarget->getName() != "AxisRotX" &&
                   lTarget->getName() != "AxisRotY" && lTarget->getName() != "AxisRotZ")
               {
                  removeSelectedObj();
                  createPosAxis(lTarget);
                  lTarget->getParentSceneNode()->showBoundingBox(true);
                  mSelected = lTarget;
                  updateSelectedUI();
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
      mRotAxisShown = false;
   }
}

void EditorAxis::clearSelectedAxis(void)
{
   Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");
   
   if (mSelectedAxis)
   {
      if (mSelectedAxis->getName() == "AxisX" || mSelectedAxis->getName() == "AxisRotX")
      {
         mSelectedAxis->setMaterialName("Axis/X");
      }
      else if (mSelectedAxis->getName() == "AxisY" || mSelectedAxis->getName() == "AxisRotY")
      {
         mSelectedAxis->setMaterialName("Axis/Y");
      }
      else if (mSelectedAxis->getName() == "AxisZ" || mSelectedAxis->getName() == "AxisRotZ")
      {
         mSelectedAxis->setMaterialName("Axis/Z");
      }
   }
   mSelectedAxis = 0;

   if (lSceneMgr->hasEntity("collisonplane"))
   {
      lSceneMgr->getRootSceneNode()->removeAndDestroyChild("collisonplane");
      lSceneMgr->destroyEntity("collisonplane");
      MeshManager::getSingleton().remove("planeforcollision");
   }
}


void EditorAxis::createPlane(void)
{
   Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");
   Ogre::SceneNode * lNode = mSelected->getParentSceneNode();
   Ogre::Vector3 lPosition = lNode->getPosition();

   Ogre::Plane plane;
   Ogre::Vector3 lUpVec;
   if (mSelectedAxis->getName() == "AxisZ" || mSelectedAxis->getName() == "AxisRotZ")
   {
   	plane.normal = Vector3::UNIT_X;
      lUpVec = Vector3::UNIT_Y;
   }
   else if (mSelectedAxis->getName() == "AxisRotY")
   {
   	plane.normal = Vector3::UNIT_Z;
      lUpVec = Vector3::UNIT_X;
   }
   else
   {
   	plane.normal = Vector3::UNIT_Y;
      lUpVec = Vector3::UNIT_Z;
   }

	plane.d = 0;

   Ogre::MeshManager::getSingleton().createPlane("planeforcollision",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
			10000,10000,10,10,true,1,10,10, lUpVec);
	Entity* lEntity = lSceneMgr->createEntity( "collisonplane", "planeforcollision" );
	lEntity->setCastShadows(false);
   lEntity->setQueryFlags(mCollisionFlag);
   lEntity->setMaterialName("Editor/Grid");

	Ogre::SceneNode* node = lSceneMgr->getRootSceneNode()->createChildSceneNode("collisonplane");
   node->setPosition(lPosition);
   node->attachObject(lEntity);
}

void EditorAxis::moveBuilding(Ogre::Ray _ray)
{
   Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");

   if (!lSceneMgr->hasEntity("collisonplane"))
      createPlane();

   Ogre::Vector3 lResult = Ogre::Vector3::ZERO;
   Ogre::Entity *lTarget = 0;
   float lDistance = -1.0f;

   Ogre::SceneNode * lNode = mSelected->getParentSceneNode();
   Ogre::Vector3 lPosition = lNode->getPosition();
   Ogre::Quaternion lRotation = lNode->getOrientation();

   if(mCollision->raycast(_ray, lResult, (unsigned long&)lTarget, lDistance, mCollisionFlag, false))
   {
      if (mSelectedAxis->getName() == "AxisX")
      {
         lPosition.z = Ogre::Math::Ceil(lResult.z);
         lNode->setPosition(lPosition);
      }
      else if (mSelectedAxis->getName() == "AxisY")
      {
         lPosition.x = Ogre::Math::Ceil(lResult.x);
         lNode->setPosition(lPosition);
      }
      else if (mSelectedAxis->getName() == "AxisZ")
      {
         lPosition.y = Ogre::Math::Ceil(lResult.y);
         lNode->setPosition(lPosition);
      }
      else if (mSelectedAxis->getName() == "AxisRotX")
      {
         lNode->lookAt(lResult, Ogre::Node::TS_PARENT, Ogre::Vector3::NEGATIVE_UNIT_X);
      }
      else if (mSelectedAxis->getName() == "AxisRotZ")
      {
         lNode->lookAt(lResult, Ogre::Node::TS_PARENT, Ogre::Vector3::NEGATIVE_UNIT_Z);
      }
      else if (mSelectedAxis->getName() == "AxisRotY")
      {
         lNode->lookAt(lResult, Ogre::Node::TS_PARENT, Ogre::Vector3::NEGATIVE_UNIT_Y);
      }

      updateSelectedUI();
   }
}

void EditorAxis::createRotAxis(Ogre::Entity * lTarget)
{
   if (lTarget == 0)
      return;

   destoryAxis(lTarget);

   Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");
   Ogre::SceneNode * lSceneNode = lTarget->getParentSceneNode()->createChildSceneNode("Axis");

   Ogre::Entity *lEntityRotX = lSceneMgr->createEntity("AxisRotX", "axisrot.mesh");
   lEntityRotX->setMaterialName("Axis/X");
   lEntityRotX->setQueryFlags(mAxisFlag);

   Ogre::Entity *lEntityRotY = lSceneMgr->createEntity("AxisRotY", "axisrot.mesh");
   lEntityRotY->setMaterialName("Axis/Y");
   lEntityRotY->setQueryFlags(mAxisFlag);

   Ogre::Entity *lEntityRotZ = lSceneMgr->createEntity("AxisRotZ", "axisrot.mesh");
   lEntityRotZ->setMaterialName("Axis/Z");
   lEntityRotZ->setQueryFlags(mAxisFlag);

   Ogre::SceneNode * lSceneNodeRotX = lSceneNode->createChildSceneNode("AxisRotX", Ogre::Vector3::ZERO, Ogre::Quaternion(0,0,0.5,0));
   lSceneNodeRotX->attachObject(lEntityRotX);
   lSceneNodeRotX->setScale(100,100,100);
   lSceneNodeRotX->setInheritOrientation(false);

   Ogre::SceneNode * lSceneNodeRotY = lSceneNode->createChildSceneNode("AxisRotY", Ogre::Vector3::ZERO, Ogre::Quaternion(0.5,0.5,0,0));
   lSceneNodeRotY->attachObject(lEntityRotY);
   lSceneNodeRotY->setScale(100,100,100);
   lSceneNodeRotY->setInheritOrientation(false);

   Ogre::SceneNode * lSceneNodeRotZ = lSceneNode->createChildSceneNode("AxisRotZ", Ogre::Vector3::ZERO, Ogre::Quaternion(0,0.5,0.5,0));
   lSceneNodeRotZ->attachObject(lEntityRotZ);
   lSceneNodeRotZ->setScale(100,100,100);
   lSceneNodeRotZ->setInheritOrientation(false);   
}

void EditorAxis::createPosAxis(Ogre::Entity * lTarget)
{
   if (lTarget == 0)
      return;

   destoryAxis(lTarget);

   Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");
   Ogre::SceneNode * lSceneNode = lTarget->getParentSceneNode()->createChildSceneNode("Axis");

   Ogre::Entity *lEntityX = lSceneMgr->createEntity("AxisX", "axis.mesh");
   lEntityX->setMaterialName("Axis/X");
   lEntityX->setQueryFlags(mAxisFlag);

   Ogre::Entity *lEntityY = lSceneMgr->createEntity("AxisY", "axis.mesh");
   lEntityY->setMaterialName("Axis/Y");
   lEntityY->setQueryFlags(mAxisFlag);

   Ogre::Entity *lEntityZ = lSceneMgr->createEntity("AxisZ", "axis.mesh");
   lEntityZ->setMaterialName("Axis/Z");
   lEntityZ->setQueryFlags(mAxisFlag);

   Ogre::SceneNode * lSceneNodeX = lSceneNode->createChildSceneNode("AxisX", Ogre::Vector3::ZERO, Ogre::Quaternion(0.5,0.5,0,0));
   lSceneNodeX->attachObject(lEntityX);
   lSceneNodeX->setScale(1000,1000,1000);
   lSceneNodeX->setInheritOrientation(false);

   Ogre::SceneNode * lSceneNodeY = lSceneNode->createChildSceneNode("AxisY", Ogre::Vector3::ZERO, Ogre::Quaternion(0,0.5,0.5,0));
   lSceneNodeY->attachObject(lEntityY);
   lSceneNodeY->setScale(1000,1000,1000);
   lSceneNodeY->setInheritOrientation(false);

   Ogre::SceneNode * lSceneNodeZ = lSceneNode->createChildSceneNode("AxisZ", Ogre::Vector3::ZERO, Ogre::Quaternion(0,0,0.5,0));
   lSceneNodeZ->attachObject(lEntityZ);
   lSceneNodeZ->setScale(1000,1000,1000);
   lSceneNodeZ->setInheritOrientation(false);
}

bool EditorAxis::isRotAxis(void)
{
   return mRotAxisShown;
}

void EditorAxis::showRotAxis(bool show)
{
   Ogre::Entity* lEnt = (Ogre::Entity*) mSelected;

   if (lEnt == 0)
      return;

   if (show)
   {
      createRotAxis(lEnt);
   }
   else
   {
      createPosAxis(lEnt);
   }
   mRotAxisShown = show;
}

void EditorAxis::destoryAxis(Ogre::MovableObject* lMoveable)
{
   Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");

   try
   {
      lMoveable->getParentSceneNode()->removeAndDestroyChild("Axis");
   }
   catch(Ogre::Exception e)
   {
      /* Axis doesn't exist */
      return;
   }

   if (lSceneMgr->hasEntity("AxisX"))
      lSceneMgr->destroyEntity("AxisX");
   if (lSceneMgr->hasEntity("AxisY"))
      lSceneMgr->destroyEntity("AxisY");
   if (lSceneMgr->hasEntity("AxisZ"))
      lSceneMgr->destroyEntity("AxisZ");
   if (lSceneMgr->hasEntity("AxisRotX"))
      lSceneMgr->destroyEntity("AxisRotX");
   if (lSceneMgr->hasEntity("AxisRotY"))
      lSceneMgr->destroyEntity("AxisRotY");
   if (lSceneMgr->hasEntity("AxisRotZ"))
      lSceneMgr->destroyEntity("AxisRotZ");
}

void EditorAxis::setSelectedAxis(Ogre::Entity* lAxis)
{
   lAxis->setMaterialName("Axis/Selected");
   mSelectedAxis = lAxis;
}

Ogre::Entity* EditorAxis::getSelectedAxis(void)
{
   return mSelectedAxis;
}
