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


#include "GameRoot.h"

namespace Client
{

GameRoot::GameRoot()
{
   mRoot = 0;
}

GameRoot::~GameRoot()
{
   Ogre::WindowEventUtilities::removeWindowEventListener(mRenderWindow, this);
   mRoot->getAutoCreatedWindow()->removeAllViewports();

   if (mGameMgr)
   {
      OGRE_DELETE mGameMgr;
      mGameMgr = 0;
   }
   
   if(mRoot)
   {
      mRoot->shutdown();
      OGRE_DELETE mRoot;
      mRoot = 0;
   }
}

bool GameRoot::loadPlugin(const Ogre::String dir)
{
   if (!mRoot)
      return false;

   try
   {
      mRoot->loadPlugin(dir);
   }
   catch(Ogre::Exception& e)
   {
      Ogre::LogManager::getSingleton().logMessage(Ogre::String("Unable to create D3D9 RenderSystem: ") + e.getFullDescription());
      return false;
   }
   return true;
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
   if (!this->loadPlugins())
      return;

   this->setupResources();
   this->configureGame();
   mGameMgr = OGRE_NEW GameManager(mRoot);
   mGameMgr->start();
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

    /* Initialise resources */
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    return true;
}

void GameRoot::setupResources()
{
   /* Load resource paths from config file */
   Ogre::ConfigFile cf;
   cf.load("resources.cfg");

   /* go through all settings in the file */
   Ogre::ConfigFile::SectionIterator itSection = cf.getSectionIterator();

   Ogre::String sSection, sType, sArch;
   while(itSection.hasMoreElements())
   {
      sSection = itSection.peekNextKey();

      Ogre::ConfigFile::SettingsMultiMap *mapSettings = itSection.getNext();
      Ogre::ConfigFile::SettingsMultiMap::iterator itSetting = mapSettings->begin();
      while(itSetting != mapSettings->end())
      {
         sType = itSetting->first;
         sArch = itSetting->second;
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(sArch, sType, sSection);
         ++itSetting;
      }
   }
}

void GameRoot::windowResized(Ogre::RenderWindow *rw)
{
   mGameMgr->windowChangedSize(rw);
}

bool GameRoot::windowClosing(Ogre::RenderWindow *rw)
{
   mGameMgr->shutdown();
   return true;
}

void GameRoot::windowFocusChange(Ogre::RenderWindow *rw)
{
   mGameMgr->windowChangedFocus(rw);
}
} /* namespace Client */

