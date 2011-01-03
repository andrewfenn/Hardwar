/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2008-2010  Andrew Fenn
    
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

#include <OgreRoot.h>
#include <libintl.h>

#include "srvstructs.h"
#include "hwstructs.h"
#include "PlayState.h"
#include "GuiTask.h"

using namespace Client;

void LoadState::enter()
{
   mSceneMgr->clearScene();

   GuiTask* gui = (GuiTask*) mTasklist->get("Gui");
   gui->resource()->setVisiblePointer(false);
   /* Get MyGUI loading layout */
   mLayout = MyGUI::LayoutManager::getInstance().load("loading.layout");

   mStatusText = gui->resource()->findWidget<MyGUI::StaticText>("status");

   mNetwork = (NetworkTask*) mTasklist->get("Network");
   mNetwork->connect();
   mNetwork->startThread();

   mLevel = (ZoneTask*) mTasklist->get("Zone");

   mGUIcount = 0;
   mReverse = false; /* for the load bar animation */
   mDownloads = false;
}

/* Destory everything we created when entering */
void LoadState::exit()
{
   /* Delete what we loaded */
   MyGUI::LayoutManager::getInstance().unloadLayout(mLayout);
}

void LoadState::update( unsigned long lTimeElapsed )
{
   mGUICounter += lTimeElapsed;
   mCounter += lTimeElapsed;

   this->updateLoadbar();

   switch(mNetwork->getConStatus())
   {
      case status_connecting:
         mStatusText->setCaption(Ogre::String(gettext("Connecting")));

         if (mNetwork->getRetryAttempts() > 0)
         {
            mStatusText->setCaption(MyGUI::UString(gettext("Retrying"))+MyGUI::UString(" (")+
                            MyGUI::UString(Ogre::StringConverter::toString(mNetwork->getRetryAttempts()))+MyGUI::UString(")"));
         }
      break;
      case status_listening:
         mStatusText->setCaption(gettext("Loading"));
      break;
      case status_filecheck:
         {
            if (mLevel->getTotal() == 0)
            {
               mLevel->init();
               mLevel->create(0, "../media/hardwar/non-free/world.scene");

               dataPacket packet = dataPacket(accepted);
               mNetwork->message(packet, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
            }
         }
      break;
      case status_downloading:
         {
            if (!mDownloads)
            {
               dataPacket packet = dataPacket(get_building_list);
               mNetwork->message(packet, SERVER_CHANNEL_GENERIC, ENET_PACKET_FLAG_RELIABLE);
               mDownloads = true;
            }
         }
      break;
      case status_ingame:
         {
            this->replace(OGRE_NEW PlayState());
         }
      break;
      case status_disconnected:
         /* the connection has failed */
         killLoadbar();
      break;
      default:
      break;
   }
}

/* Update the GUI animation */
void LoadState::updateLoadbar()
{
  
}

/* Stop the loading animation and stop failure message */
void LoadState::killLoadbar()
{

   /* TODO: make this a popup window? */
   mStatusText->setCaption(Ogre::String(gettext("Failed")));
}

bool LoadState::keyPressed(const OIS::KeyEvent &e)
{
   return true;
}

bool LoadState::keyReleased(const OIS::KeyEvent &e)
{
   if( e.key == OIS::KC_ESCAPE )
   {
      /* FIXME: Should go to main menu */
      this->shutdown();
   }
   return true;
}

bool LoadState::mouseMoved(const OIS::MouseEvent &e)
{
   return true;
}

bool LoadState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   return true;
}

bool LoadState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   return true;
}
