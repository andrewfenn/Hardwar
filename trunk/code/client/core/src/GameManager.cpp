/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2008  Andrew Fenn
    
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

#include "GameState.h"
#include "PlayState.h"
#include "LoadState.h"
#include "GameManager.h"

using namespace Client;

GameManager* GameManager::mGameManager;

GameManager::GameManager(void) : mRoot(0), mInputMgr(0), mLoadState(0), 
                                                 mPlayState(0), bShutdown(false)
{
   mSinglePlayer = false;
   mNetwork = Network::getSingletonPtr();
}

GameManager::~GameManager( void )
{
   delete mNetwork;

   /* Clean up all the states */
   while(!mStates.empty())
   {
      mStates.back()->exit();
      mStates.pop_back();
   }
   if(mInputMgr)
   {
      delete mInputMgr;
      mInputMgr = 0;
   }
   if(mLoadState)
   {
      delete mLoadState;
      mLoadState = 0;
   }	
   if(mPlayState)
   {
      delete mPlayState;
      mPlayState  = 0;
   }

   /* Remove all listeners from console */
   Ogre::LogManager::getSingletonPtr()->getDefaultLog()->removeListener(mOgreLogListener);
   delete mOgreLogListener;

   /* Delete settings manager */
   delete mSettings;

   /* Delete MyGUI */
   mGUI->destroyAllChildWidget();
   mGUI->shutdown();
   delete mGUI;
   mGUI = 0;


   mSceneMgr->clearScene();
   mSceneMgr->destroyAllCameras();
   mRoot->getAutoCreatedWindow()->removeAllViewports();

   if(mRoot)
   {
      delete mRoot;
      mRoot = 0;
   }
}

void GameManager::startGame( GameState *gameState )
{
   #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
   mRoot = new Ogre::Root("plugins.cfg", "game.cfg", "./logs/ogre.log");
   #else
   /* Unix */
   FILE *file;
   if ((file = fopen ("/etc/OGRE/plugins.cfg","r")))
   {
      /* check for Debian plugins.cfg in /etc/OGRE/ */
      fclose(file);
      mRoot = new Ogre::Root("/etc/OGRE/plugins.cfg", "game.cfg", "./logs/ogre.log");
   }
   else
   {
      /* Look for the plugins we want manually */
      mRoot = new Ogre::Root("", "game.cfg", "./logs/ogre.log");

      if (opendir ("/usr/lib/OGRE") != NULL)
      {
         mRoot->loadPlugin("/usr/lib/OGRE/Plugin_OctreeSceneManager");
         mRoot->loadPlugin("/usr/lib/OGRE/RenderSystem_GL");
      }
      else if (opendir ("/usr/local/lib/OGRE") != NULL)
      {
         mRoot->loadPlugin("/usr/local/lib/OGRE/Plugin_OctreeSceneManager");
         mRoot->loadPlugin("/usr/local/lib/OGRE/RenderSystem_GL");
      }
      else
      {
         /* Can't find where Ogre is, just start and hope for the best */
         mRoot = new Ogre::Root("plugins.cfg", "game.cfg", "./logs/ogre.log");
      }
   }
   #endif

   this->setupResources();
   if( !this->configureGame() )
   {
      /* If game can't be configured, throw exception and quit application */
      throw Ogre::Exception( Ogre::Exception::ERR_INTERNAL_ERROR,
                                      "Error - Couldn't Configure Renderwindow",
                                      "Game Error" );
      return;
   }

   /* Setup open input system */
   mInputMgr = InputManager::getSingletonPtr();
   mInputMgr->initialise(mRenderWindow);
   mInputMgr->addKeyListener(this, "GameManager");
   mInputMgr->addMouseListener(this, "GameManager");
   mInputMgr->getJoystick(1);

   /* Create just one MyGUI instance rather then within the states */
   mGUI = new MyGUI::Gui();

   mRoot->createSceneManager(Ogre::ST_GENERIC,"GameSceneMgr");
   mSceneMgr     = mRoot->getSceneManager("GameSceneMgr");
   mCamera       = mSceneMgr->createCamera("GameCamera");
   mViewport     = mRenderWindow->addViewport(mCamera,1);

   mGUI->initialise(mRoot->getAutoCreatedWindow(), "core.xml", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "logs/mygui.log");
   mGUI->setSceneManager(mSceneMgr);

   /* Add Console */
   mConsole = Console::getSingletonPtr();

   /* Add log listeners */
   mOgreLogListener = new OgreLogListener;
   Ogre::LogManager::getSingletonPtr()->getDefaultLog()->addListener(mOgreLogListener);

   /* Add Game Settings */
   mSettings = GameSettings::getSingletonPtr();

   /* Setup states */
   mLoadState = LoadState::getSingletonPtr();
   mPlayState = PlayState::getSingletonPtr();
   /* Go to the first state */
   this->changeState( gameState );

   /* 
    * lTimeLastFrame remembers the last time that it was checked
    * We use it to calculate the time since last frame
    */
   unsigned long lTimeLastFrame = 0;
   unsigned long lDelay = 0;

   while( !bShutdown )
   {
      /* 
       * Calculate time since last frame and remember current time 
       * for next frame 
       */
      unsigned long lTimeCurrentFrame = mRoot->getTimer()->getMilliseconds();
      unsigned long lTimeSinceLastFrame = lTimeCurrentFrame - lTimeLastFrame;
      lTimeLastFrame = lTimeCurrentFrame;

      /* update input */
      mInputMgr->capture();

      /* update the current state */
      mStates.back()->update(lTimeSinceLastFrame);

      lDelay += lTimeSinceLastFrame;
      if (lDelay > mSettings->getDelayTime())
      {
         /* render the next frame */
         mRoot->renderOneFrame();
         mSettings->update(lTimeSinceLastFrame);
         mGUI->injectFrameEntered((Ogre::Real) lTimeSinceLastFrame/1000);
         lDelay = 0;
      }
      mConsole->update();
      /* Deal with platform specific issues */
      Ogre::WindowEventUtilities::messagePump();
   }
}

bool GameManager::configureGame( void )
{
    /* Load config settings from ogre.cfg */
    if( !mRoot->restoreConfig())
    {
      // If there is no config file, show the configuration dialog
      if( !mRoot->showConfigDialog())
      {
         return false;
      }
    }

    /* Initialise and create a default rendering window */
    mRenderWindow = mRoot->initialise( true, "Hardwar" );

    /* Initialise resources */
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    return true;
}

void GameManager::setupResources(void)
{
   /* Load resource paths from config file */
   Ogre::ConfigFile cf;   /* Create just one MyGUI instance rather then within the states */
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
                         
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(sArch,
                                                               sType, sSection);

         ++itSetting;
      }
   }
}

void GameManager::changeState(GameState *gameState)
{
   /* cleanup the current state */
   if(!mStates.empty())
   {
      mStates.back()->exit();
      mStates.pop_back();
   }
   /* store and init the new state */
   mStates.push_back( gameState );
   mStates.back()->enter();
}

void GameManager::pushState(GameState *gameState)
{
   /* pause current state */
   if(!mStates.empty())
   {
      mStates.back()->pause();
   }

   /* Store and init the new state */
   mStates.push_back( gameState );
   mStates.back()->enter();
}

void GameManager::popState()
{
   /* Cleanup the current state */
   if(!mStates.empty())
   {
      mStates.back()->exit();
      mStates.pop_back();
   }

   /* Resume previous state */
   if(!mStates.empty())
   {
      mStates.back()->resume();
   }
}

void GameManager::requestShutdown(void)
{
   bShutdown = true;
}

bool GameManager::keyPressed(const OIS::KeyEvent &e)
{
   /* Call keyPressed of current state */
   mStates.back()->keyPressed( e );

   return true;
}

bool GameManager::keyReleased(const OIS::KeyEvent &e)
{
   /* call keyReleased of current state */
   mStates.back()->keyReleased(e);

   return true;
}

bool GameManager::mouseMoved(const OIS::MouseEvent &e)
{
   /* call mouseMoved of current state */
   mStates.back()->mouseMoved( e );

   return true;
}

bool GameManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   /* Call mousePressed of current state */
   mStates.back()->mousePressed( e, id );

   return true;
}

bool GameManager::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   /* Call mouseReleased of current state */
   mStates.back()->mouseReleased( e, id );

   return true;
}

GameManager* GameManager::getSingletonPtr(void)
{
   if(!mGameManager)
   {
      mGameManager = new GameManager();
   }

   return mGameManager;
}

