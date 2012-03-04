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

#include "PhysicsManager.h"

#include <libintl.h>

using namespace Server;

PhysicsManager::~PhysicsManager()
{
}

PhysicsManager::PhysicsManager()
{
}

void PhysicsManager::setup(Ogre::Root* root)
{
   mRoot = root;
   std::cout << gettext("Loading resources") << std::endl;

   this->load("../media/hardwar/non-free/world.scene");
}

void PhysicsManager::load(const Ogre::String file)
{
   std::cout << gettext("Loading scene: ") << file << std::endl;
}

void PhysicsManager::update(const Clients* clients)
{
   mClients = clients;
}

void PhysicsManager::process(const unsigned long timeElapsed)
{

}
