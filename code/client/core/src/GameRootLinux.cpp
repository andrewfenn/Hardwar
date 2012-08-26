/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2012  Andrew Fenn
    
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

#include "GameRootLinux.h"
#include <sys/types.h>
#include <unistd.h>

namespace Client
{

bool GameRootPlatform::isLocked()
{
      std::fstream runfile;
      char* buf;
      int len, pid;
      runfile.open("/var/lock/hardwar", std::fstream::in | std::fstream::out | std::fstream::app);

      // No file, game not running
      if (!runfile.is_open())
         return false;
         
      runfile.seekg (0, std::ios::end);
      len = runfile.tellg();
      runfile.seekg (0, std::ios::beg);

      if (len > 20)
      {
         // should only store a number         
         runfile.close();
         return true;
      }
      buf = OGRE_NEW char[len];
      runfile.read(buf,len);
      runfile.close();

      pid = atoi(buf);

      OGRE_DELETE buf;
      buf = 0;

      if (pid < 1)
         return false;

      Ogre::String proc = "/proc/"+Ogre::StringConverter::toString(pid)+"/status";
      runfile.open(proc.c_str(), std::fstream::in);

      // No file, game not running
      if (!runfile.is_open())
         return false;
         
      runfile.close();
      return true;

   return false;
}

void GameRootPlatform::setLocked(const bool& locked)
{
      std::fstream runfile;
      std::string buf;

      remove("/var/lock/hardwar");
      if (locked)
      {
         buf = Ogre::String(Ogre::StringConverter::toString(getpid()));
         runfile.open("/var/lock/hardwar", std::fstream::in | std::fstream::out | std::fstream::app);
         runfile.write(buf.c_str(),buf.size());
         runfile.close();
      }
}

bool GameRootPlatform::loadPlugins()
{
    bool error = false;
    bool loaded = false;
    if (opendir("/usr/lib/OGRE") != 0)
    {
        if (!this->loadPlugin("/usr/lib/OGRE/RenderSystem_GL"))
            error = true;

        if (!this->loadPlugin("/usr/lib/OGRE/Plugin_OctreeSceneManager"))
            error = true;

        if (!this->loadPlugin("/usr/lib/OGRE/Plugin_CgProgramManager"))
            error = true;

        loaded = true;
    }

    if ((!loaded || error) && opendir("/usr/local/lib/OGRE") != 0)
    {
        error = false;

        if (!this->loadPlugin("/usr/local/lib/OGRE/RenderSystem_GL"))
            error = true;

        if (!this->loadPlugin("/usr/local/lib/OGRE/Plugin_OctreeSceneManager"))
            error = true;

        if (!this->loadPlugin("/usr/local/lib/OGRE/Plugin_CgProgramManager"))
            error = true;

        loaded = true;
    }

    if (!loaded || error)
    {
        if (!this->loadPlugin("RenderSystem_GL"))
            return false;

        if (!this->loadPlugin("Plugin_OctreeSceneManager"))
            return false;

        if (!this->loadPlugin("Plugin_CgProgramManager"))
            return false;
    }

    return true;
}

} /* namespace Client */

