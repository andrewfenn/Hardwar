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

#include "LoadState.h"

using namespace Client;

LoadState* LoadState::mLoadState;

void LoadState::enter( void )
{
   mRoot            = Ogre::Root::getSingletonPtr();

   mGameMgr      = GameManager::getSingletonPtr();
   mWindow       = mRoot->getAutoCreatedWindow();
   mSceneMgr     = mRoot->getSceneManager("GameSceneMgr");
   mCamera       = mGameMgr->mCamera;
   mViewport     = mGameMgr->mViewport;

   mSceneMgr->clearScene();

   mGameMgr->mGUI->hidePointer();
   /* Get MyGUI loading layout */
   mLayout = MyGUI::LayoutManager::getInstance().load("loading.layout");
   mStatusText = MyGUI::Gui::getInstance().findWidget<MyGUI::StaticText>("status");

   /* Pre load all building models - Not doing so makes the network thread crash 
      when it tries to create buildings during connection.
    */
   boost::filesystem::path lPath("../media/models/hangers");
	if (boost::filesystem::is_directory(lPath))
   {
	   for (boost::filesystem::directory_iterator itr(lPath); itr!=boost::filesystem::directory_iterator(); ++itr)
      {
	      Ogre::String temp = (Ogre::String) itr->path().leaf();
	      if (temp.substr(temp.length()-4, 4).compare("mesh") == 0)
         {
            MeshManager::getSingleton().load((Ogre::String) itr->path().leaf(), "General");
	      }
	   }
	}
   

   mGameMgr->getNetwork()->connect();
   mGameMgr->getNetwork()->startThread();


   mGUIcount = 0;
   mReverse = false; /* for the load bar animation */
   mFilesLoaded = false;
   mDownloads = false;
}

/* Destory everything we created when entering */
void LoadState::exit( void )
{
   /* Delete what we loaded */
   MyGUI::LayoutManager::getInstance().unloadLayout(mLayout);
}

void LoadState::pause(void)
{}

void LoadState::resume(void)
{}

void LoadState::update( unsigned long lTimeElapsed )
{
   mGUICounter += lTimeElapsed;
   mCounter += lTimeElapsed;

   updateLoadbar();

   switch(mGameMgr->getNetwork()->getConStatus())
   {
      case STATUS_CONNECTING:
         mStatusText->setCaption(Ogre::String(gettext("Connecting")));

         if (mGameMgr->getNetwork()->getRetryAttempts() > 0)
         {
            mStatusText->setCaption(Ogre::UTFString(gettext("Retrying"))+Ogre::UTFString(" (")+
                            Ogre::StringConverter::toString(mGameMgr->getNetwork()->getRetryAttempts())+Ogre::UTFString(")"));
         }
      break;
      case STATUS_LISTENING:
         mStatusText->setCaption(Ogre::String(gettext("Loading")));
      break;
      case STATUS_FILECHECK:
         {
            if (!mFilesLoaded)
            {
               /* load world */
               OgreMax::OgreMaxScene *lOgreMax = new OgreMax::OgreMaxScene;
               const Ogre::String filename = Ogre::String("../media/hardwar/non-free/world.scene");
               lOgreMax->Load(filename, mWindow, 0, mSceneMgr, mSceneMgr->getRootSceneNode());

               /* turn it into static geometry */
               Ogre::StaticGeometry *lStatic = mSceneMgr->createStaticGeometry("world");
               lStatic->addSceneNode(mSceneMgr->getSceneNode("world"));

               mGameMgr->getNetwork()->message("ok", strlen("ok")+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
               mFilesLoaded = true;
            }
         }
      break;
      case STATUS_DOWNLOADING:
         if (!mDownloads)
         {
            mGameMgr->getNetwork()->message("ok", strlen("ok")+1, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
            mDownloads = true;
         }
      break;
      case STATUS_INGAME:
         this->changeState(PlayState::getSingletonPtr());
      break;
      case STATUS_DISCONNECTED:
         /* the connection has failed */
         killLoadbar();
      break;
      default:
      break;
   }
}

/* Update the GUI animation */
void LoadState::updateLoadbar(void)
{
   MyGUI::types::TCoord<Ogre::Real> coord;
   if (mGUICounter*0.001 > 0.5)
   {
      mGUICounter = 0;
      MyGUI::StaticImagePtr lstatusImage;

      if (mGUIcount > 0)
      {
         /* set the size of the last big dot to small */
         lstatusImage= MyGUI::Gui::getInstance().findWidget<MyGUI::StaticImage> (
                                                          Ogre::UTFString("dot")+
                                       Ogre::StringConverter::toString(mGUIcount)
                                                                               );
         coord = lstatusImage->getCoord();
         lstatusImage->setCoord(coord.left+3, coord.top +3, coord.width -5, coord.height -5);
      }

      if (mReverse)
      {
         mGUIcount--;
      }
      else
      {
         mGUIcount++;
      }

      if (mGUIcount < 2)
      {
         mGUIcount = 1;
         mReverse = false;
      }
      if (mGUIcount > 2)
      {
         mGUIcount = 3;
         mReverse = true;
      }

      lstatusImage= MyGUI::Gui::getInstance().findWidget<MyGUI::StaticImage> (
                                                          Ogre::UTFString("dot")+
                                       Ogre::StringConverter::toString(mGUIcount)
                                                                               );
      coord = lstatusImage->getCoord();
      lstatusImage->setCoord(coord.left-3, coord.top -3, coord.width +5, coord.height +5);
   }
}

/* Stop the loading animation and stop failure message */
void LoadState::killLoadbar()
{
   MyGUI::StaticImagePtr lstatusImage;
   lstatusImage= MyGUI::Gui::getInstance().findWidget<MyGUI::StaticImage> ("dot1");
   lstatusImage->setVisible(false);
   lstatusImage= MyGUI::Gui::getInstance().findWidget<MyGUI::StaticImage> ("dot2");
   lstatusImage->setVisible(false);
   lstatusImage= MyGUI::Gui::getInstance().findWidget<MyGUI::StaticImage> ("dot3");
   lstatusImage->setVisible(false);
   /* TODO: make this a popup window? */
   mStatusText->setCaption(Ogre::String(gettext("Failed")));
}

void LoadState::keyPressed(const OIS::KeyEvent &e)
{ }

void LoadState::keyReleased(const OIS::KeyEvent &e)
{
   if( e.key == OIS::KC_ESCAPE )
   {
      this->requestShutdown(); /* FIXME: Should go to main menu */
   } 
}

void LoadState::mouseMoved(const OIS::MouseEvent &e)
{ }

void LoadState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ }

void LoadState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ }

LoadState* LoadState::getSingletonPtr(void)
{
   if(!mLoadState)
   {
      mLoadState = new LoadState();
   }
   return mLoadState;
}
