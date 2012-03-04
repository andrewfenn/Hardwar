/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2011  Andrew Fenn
    
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

#pragma once

#include "enet/enet.h"
#include <Ogre.h>
#include "Client.h"

namespace Server
{
   class PhysicsManager
   {
      public:

         PhysicsManager();
         ~PhysicsManager();

         void setup(Ogre::Root* root);
         /** Loads a dot scene file up and stores the data for collision detection */
         void load(const Ogre::String);
         void update(const Clients*);
         void process(const unsigned long);
      private:
         const Clients* mClients;
         Ogre::Root* mRoot;
   };
}

