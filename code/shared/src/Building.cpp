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

#include "Building.h"

namespace Hardwar
{

   Building::Building() { }

   Building::Building(const Ogre::Vector3 position, const Ogre::Vector3 rotation, const Ogre::String mesh)
   {
      mPosition = position;
      mRotation = rotation;
      mMesh     = mesh;
   }

   const Ogre::Vector3 Building::getPosition(void)
   {
      return mPosition;
   }

   void Building::setPosition(const Ogre::Vector3 position)
   {
      mPosition = position;
   }

   const Ogre::Vector3 Building::getRotation(void)
   {
      return mRotation;
   }

   void Building::setRotation(const Ogre::Vector3 rotation)
   {
      mRotation = rotation;
   }

   const Ogre::String Building::getMeshName(void)
   {
      return mMesh;
   }

   void Building::setMeshName(const Ogre::String mesh)
   {
      mMesh = mesh;
   }
}
