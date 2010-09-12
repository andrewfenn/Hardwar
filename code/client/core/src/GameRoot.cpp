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
   if(mRoot)
   {
      delete mRoot;
      mRoot = 0;
   }
}

void GameRoot::loadPlugins()
{
   #if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
   if (opendir("/usr/lib/OGRE") != 0)
   {
      mRoot->loadPlugin("/usr/lib/OGRE/RenderSystem_GL");
      mRoot->loadPlugin("/usr/lib/OGRE/Plugin_OctreeSceneManager");
      mRoot->loadPlugin("/usr/lib/OGRE/Plugin_CgProgramManager");
   }
   else if (opendir("/usr/local/lib/OGRE") != 0)
   {
      mRoot->loadPlugin("/usr/local/lib/OGRE/RenderSystem_GL");
      mRoot->loadPlugin("/usr/local/lib/OGRE/Plugin_OctreeSceneManager");
      mRoot->loadPlugin("/usr/local/lib/OGRE/Plugin_CgProgramManager");
   }
   else
   {
      /* Can't find where Ogre is, just start and hope for the best */
      mRoot = new Ogre::Root("plugins.cfg", "game.cfg", str);
   }
   #endif
}

void GameRoot::init()
{

   Ogre::String str = Ogre::String("./logs/ogre.log");

   #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
   mRoot = new Ogre::Root("plugins.cfg", "game.cfg", str);
   #else
   /* Unix */
   mRoot = new Ogre::Root("", "game.cfg", str);
   #endif

   this->loadPlugins();
   this->setupResources();
   this->configureGame();
   mGameMgr = new GameManager();
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
   mGameMgr.windowChangedSize(rw);
}

bool GameRoot::windowClosing(Ogre::RenderWindow *rw)
{
   mGameMgr.shutdown();
}

void GameRoot::windowFocusChange(Ogre::RenderWindow *rw)
{
   mGameMgr.windowChangedFocus(rw);
}
} /* namespace Client */

