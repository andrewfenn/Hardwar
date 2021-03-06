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

#ifndef __BUILDING_H_
#define __BUILDING_H_

#include <Ogre.h>

#include "DataPacket.h"

namespace Hardwar
{
   class Building
   {
      public:
         Building();
         Building(const Ogre::Vector3, const Ogre::Quaternion, const Ogre::String);
         const Ogre::Vector3 getPosition();
         const Ogre::Quaternion getRotation();
         const Ogre::String getMeshName();
         unsigned int getID();
         void setPosition(const Ogre::Vector3);
         void setRotation(const Ogre::Quaternion);
         void setMeshName(const Ogre::String);
         void setID(const unsigned int);
         dataPacket serialize(dataPacket);
         void unserialize(dataPacket);
      private:
         int mID;
         Ogre::Vector3 mPosition;
         Ogre::Quaternion mRotation;
         Ogre::String  mMesh;
   };

   typedef std::multimap<unsigned int,Building> Buildings;
}

#endif /* __BUILDING_H_ */
