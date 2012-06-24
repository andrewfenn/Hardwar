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

#include "ZoneTask.h"

using namespace Client;

void ZoneTask::init()
{
/*
   boost::filesystem::path lPath("../media/models/hangers");
   if (boost::filesystem::is_directory(lPath))
   {
	   for (boost::filesystem::directory_iterator itr(lPath); itr!=boost::filesystem::directory_iterator(); ++itr)
      {
	      Ogre::String temp = (Ogre::String) itr->path().leaf();
	      if (temp.substr(temp.length()-4, 4).compare("mesh") == 0)
         {
            Ogre::MeshManager::getSingleton().load((Ogre::String) itr->path().leaf(), "General");
	      }
	   }
	}
*/
}

void ZoneTask::shutdown()
{
   this->clear();
}

void ZoneTask::update() {}

void ZoneTask::create(const int zone, const Ogre::String filename)
{
   /* FIXME: Needs correct number */
   mZones.push_back(Zone(filename));
}

void ZoneTask::clear()
{
   mZones.clear();
}

int ZoneTask::getTotal()
{
   return mZones.size();
}

Zone* ZoneTask::getCurrent()
{
   /* FIXME: Needs to actually get the correct zone */
   return &mZones[0];
}
