/*
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2016 Andrew Fenn

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

#include "GameRootOSX.h"

#include "Ogre.h"

#include <dirent.h>
#include <fstream>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

namespace Client
{

Ogre::UTFString GameRootOSX::getHomeDirectory()
{
    Ogre::UTFString homeDir;
    homeDir = Ogre::UTFString(getenv("HOME"));
    if (homeDir.empty())
    {
        struct passwd *pw = getpwuid(getuid());
        homeDir = Ogre::UTFString(pw->pw_dir);
    }
    return homeDir + Ogre::UTFString("/.hardwar");
}

bool GameRootOSX::isLocked()
{
    createHomeDir();

    Ogre::UTFString pidFile = this->getHomeDirectory()+Ogre::UTFString("/pid");
    std::fstream runfile;
    char* buf;
    int len;
    pid_t pid;
    runfile.open(pidFile.asUTF8_c_str(), std::fstream::in | std::fstream::out | std::fstream::app);

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

    // Make sure process doesn't exist as a zombie process
    while(waitpid(-1, 0, WNOHANG) > 0) {
        // Wait for defunct....
    }

    // Send a signal with kill 0 which just performs
    // the error checking that the process is running
    // without killing the process
    if (0 != kill(pid, 0))
        return false;

    return true;
}

void GameRootOSX::setLocked(const bool& locked)
{
    createHomeDir();

    Ogre::UTFString pidFile = this->getHomeDirectory() + Ogre::UTFString("/pid");
    std::fstream runfile;
    std::string buf;

    remove(pidFile.asUTF8_c_str());
    if (locked)
    {
        buf = Ogre::String(Ogre::StringConverter::toString(getpid()));
        runfile.open(pidFile.asUTF8_c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
        runfile.write(buf.c_str(),buf.size());
        runfile.close();
    }
}

void GameRootOSX::createHomeDir()
{
    Ogre::UTFString homeDir = this->getHomeDirectory();

    // check if the folder exists otherwise create it
    if (opendir(homeDir.asUTF8_c_str()) == nullptr)
    {
        if (mkdir(homeDir.asUTF8_c_str(), S_IRWXU|S_IRGRP|S_IXGRP) != 0)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "Can not create folder in home directory", "GameRootOSX::isLocked");
        }
    }
}

bool GameRootOSX::loadPlugins()
{
    bool error = false;
    bool loaded = false;
    if (opendir("/usr/lib/OGRE") != nullptr)
    {
        if (!this->loadPlugin("/usr/lib/OGRE/RenderSystem_GL"))
            error = true;

        if (!this->loadPlugin("/usr/lib/OGRE/Plugin_OctreeSceneManager"))
            error = true;

        if (!this->loadPlugin("/usr/lib/OGRE/Plugin_CgProgramManager"))
            error = true;

        loaded = true;
    }

    if ((!loaded || error) && opendir("/usr/local/lib/OGRE") != nullptr)
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

