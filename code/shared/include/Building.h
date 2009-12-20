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
         Building(const Ogre::Vector3, const Ogre::Vector3, const Ogre::String);
         const Ogre::Vector3 getPosition(void);
         const Ogre::Vector3 getRotation(void);
         const Ogre::String getMeshName(void);
         void setPosition(const Ogre::Vector3);
         void setRotation(const Ogre::Vector3);
         void setMeshName(const Ogre::String);
         dataPacket serialize(dataPacket);
      private:
         Ogre::Vector3 mPosition;
         Ogre::Vector3 mRotation;
         Ogre::String  mMesh;
   };
}

#endif /* __BUILDING_H_ */
