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
   mCollision = new MOC::CollisionTools(Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr"));
}

EditorAxis::~EditorAxis(void)
{
   delete mCollision;
}

void EditorAxis::selectBuilding(const Ogre::Ray _ray)
{
   Ogre::Vector3 lResult = Ogre::Vector3::ZERO;
   Ogre::Entity *lTarget = 0;
   float lDistance = -1.0f;

   Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");

   if(mCollision->raycast(_ray, lResult, (unsigned long&)lTarget, lDistance))
   {
      if (lTarget != 0)
      {
         if (lTarget->getParentSceneNode()->getName() != "world" && lTarget->getParentSceneNode()->getName() != "EditorNode")
         {
            if (lTarget->getParentSceneNode()->getName() == "Axis")
            {
               Ogre::Entity * lEntity = static_cast<Ogre::Entity*>(lTarget);
               for(int i=0; i < lEntity->getNumSubEntities(); i++)
               {
                  if (mCollision->raycastSubEntityPolygons(lEntity->getSubEntity(i), _ray, lResult, lDistance))
                  {
                     std::cout << "SubEntity: " << lEntity->getSubEntity(i)->getMaterialName() << std::endl;
                  }
                  else
                  {
                     std::cout << "not found" << std::endl;
                  }
               }
            }
            else
            {
               if (mEditorObjSelected)
               {
                  mSelected->getParentSceneNode()->showBoundingBox(false);
                  mSelected->getParentSceneNode()->removeAndDestroyChild("Axis");
                  mEditorObjSelected = false;
                  if (lSceneMgr->hasEntity("Axis"))
                  {
                     lSceneMgr->destroyEntity("Axis");
                  }
               }

               Ogre::Entity *lEntity = lSceneMgr->createEntity("Axis", "axes.mesh");
               Ogre::SceneNode * lSceneNode = lTarget->getParentSceneNode()->createChildSceneNode("Axis");
               lSceneNode->attachObject(lEntity);
               lSceneNode->setScale(10,10,10);
               lTarget->getParentSceneNode()->showBoundingBox(true);

               mSelected = lTarget;
               mEditorObjSelected = true;
            }
      } // !world && !editornode
   } // collision
}

} /* namespace */
