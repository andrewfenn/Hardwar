/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2010-2011  Andrew Fenn
    
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


#include "GameRoot.h"

#include <fstream>
#include "OgreException.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
   #include <sys/types.h>
   #include <unistd.h>
#endif

namespace Client
{

GameRoot::GameRoot()
{
   mRoot = nullptr;
}

GameRoot::~GameRoot()
{
    Ogre::WindowEventUtilities::removeWindowEventListener(mRenderWindow, this);

/*
   if (mGameMgr)
   {
      OGRE_DELETE mGameMgr;
      mGameMgr = 0;
   }
  */
    if(mRoot)
    {
        mRoot->getAutoCreatedWindow()->removeAllViewports();
        mRoot->shutdown();
        OGRE_DELETE mRoot;
        mRoot = 0;
    }
}

bool GameRoot::loadPlugin(const Ogre::String dir)
{
    if (mRoot == nullptr)
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "mRoot not initialised", "GameRoot::loadPlugin");

    try
    {
        mRoot->loadPlugin(dir);
    }
    catch(Ogre::Exception& e)
    {
        Ogre::LogManager::getSingleton().logMessage(Ogre::String("Unable to load plugin: ") + e.getFullDescription());
        return false;
    }
    return true;
}

bool GameRoot::isLocked()
{
   /* FIXME: Add Windows and OSX implementation */
   #if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
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
   #endif

   return false;
}

void GameRoot::setLocked(const bool& locked)
{
   /* FIXME: Add Windows and OSX implementation */
   #if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
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
   #endif
}

bool GameRoot::loadPlugins()
{
   mRoot = OGRE_NEW Ogre::Root("", "game.cfg", "./logs/ogre.log");

   #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      HRESULT hr; 
      DWORD dwDirectXVersion = 0; 
      TCHAR strDirectXVersion[10]; 

      hr = GetDXVersion( &dwDirectXVersion, strDirectXVersion, 10 ); 

      if(!SUCCEEDED(hr))
         return false;

      ostringstream dxinfoStream; 
      dxinfoStream << "DirectX version: " << strDirectXVersion; 
      LogManager::getSingleton().logMessage(dxinfoStream.str()); 

      if(dwDirectXVersion < 0x00090000)
         return false;

      if (!this->loadPlugin("RenderSystem_Direct3D9"))
      {
         if (!this->loadPlugin("RenderSystem_GL"))
            return false;
      }

      if (!this->loadPlugin("Plugin_OctreeSceneManager"))
         return false;

      if (!this->loadPlugin("Plugin_CgProgramManager"))
         return false;
   #endif

   #if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
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
   #endif

   return true;
}

void GameRoot::init()
{
   if (this->isLocked())
      return;

   this->setLocked(true);

   if (!this->loadPlugins())
      return;

   this->configureGame();
 //  mGameMgr = OGRE_NEW GameManager(mRoot);
 //  mGameMgr->start();
   this->setLocked(false);
}

bool GameRoot::configureGame()
{
    /* Load config settings from ogre.cfg */
    if( !mRoot->restoreConfig())
    {
      // If there is no config file, show the configuration dialog
      if( !mRoot->showConfigDialog())
      {
         /* If game can't be configured, throw exception and quit application */
         throw Ogre::Exception( Ogre::Exception::ERR_INTERNAL_ERROR, "Error - Couldn't Configure Renderwindow", "Game Error");
      }
    }

    /* Initialise and create a default rendering window */
    mRenderWindow = mRoot->initialise( true, "Hardwar" );
    Ogre::WindowEventUtilities::addWindowEventListener(mRenderWindow, this);

    return true;
}

void GameRoot::windowResized(Ogre::RenderWindow *rw)
{
 //  mGameMgr->windowChangedSize(rw);
}

bool GameRoot::windowClosing(Ogre::RenderWindow *rw)
{
  // mGameMgr->shutdown();
   return true;
}

void GameRoot::windowFocusChange(Ogre::RenderWindow *rw)
{
  // mGameMgr->windowChangedFocus(rw);
}
} /* namespace Client */

