/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2010  Andrew Fenn
    
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

#include "Zone.h"

using namespace Client;

Zone::Zone(const Ogre::String filename)
{
   Ogre::Root* lRoot             = Ogre::Root::getSingletonPtr();
   Ogre::SceneManager* lSceneMgr  = lRoot->getSceneManager("GameSceneMgr");
   Ogre::RenderWindow* lWindow    = lRoot->getAutoCreatedWindow();
   /* load world file */
   OgreMax::OgreMaxScene *lOgreMax = OGRE_NEW OgreMax::OgreMaxScene;
   lOgreMax->Load(filename, lWindow, 0, lSceneMgr, lSceneMgr->getRootSceneNode());

   /* turn it into static geometry */
   /* FIXME: Need to change this so that it creates with different name */
   Ogre::StaticGeometry *lStatic = lSceneMgr->createStaticGeometry("world");
   lStatic->addSceneNode(lSceneMgr->getSceneNode("world"));

   OGRE_DELETE lOgreMax;
}

Zone::~Zone()
{

}

bool Zone::addBuilding(Hardwar::Building building)
{
   mBuildings.push_back(building);
   return drawBuilding(building);
}

Hardwar::Building Zone::getBuildingByName(Ogre::String name)
{
   Ogre::StringVector arr = Ogre::StringUtil::split(name, "/");
   if (arr.size() != 2)
      throw;

   unsigned int id = Ogre::StringConverter::parseInt(arr[1]);

   for (std::vector<Hardwar::Building>::iterator buildItr=mBuildings.begin(); buildItr != mBuildings.end(); buildItr++)
   {
      if (buildItr->getID() == id)
      {
         return *buildItr;
      }
   }

   OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
         "No building with the name " + name + " found",
         "Zone::getBuildingByName" );
}

bool Zone::drawBuilding(Hardwar::Building building)
{
   Ogre::SceneManager* lSceneMgr = Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr");

   try
   {
      /* We create an entity with the name of the buildings id in the database serverside. */
      Ogre::Entity *lEntity = lSceneMgr->createEntity(Ogre::String("Building/")+Ogre::StringConverter::toString(building.getID()), building.getMeshName());
      Ogre::SceneNode * lSceneNode = lSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::String("Building/")+Ogre::StringConverter::toString(building.getID()));
      lSceneNode->attachObject(lEntity);
      lSceneNode->setPosition(building.getPosition());
      if (building.getRotation() != Ogre::Quaternion::ZERO)
      {
         lSceneNode->setOrientation(building.getRotation());
      }
      return true;
   }
   catch(Ogre::Exception& e)
   {
      // FIXME: Add error to log
   }

   return false;
}
