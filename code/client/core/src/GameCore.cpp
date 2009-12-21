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

#include "GameCore.h"

using namespace Client;

GameCore::GameCore(void)
{
   Ogre::ConfigFile lconfig;
   lconfig.load("settings.cfg");

   mMaxFPS  = Ogre::StringConverter::parseInt(lconfig.getSetting("MaxFPS", "Game", "60"));
   mShowFPS = Ogre::StringConverter::parseBool(lconfig.getSetting("ShowFPS", "Debug", "false"));
   mShowNet = Ogre::StringConverter::parseBool(lconfig.getSetting("ShowNet", "Debug", "false"));

   mConsole = Console::getSingletonPtr();
   mConsole->addCommand(Ogre::UTFString("cl_showfps"), MyGUI::newDelegate(this, &GameCore::cmd_showFPS));
   mConsole->addCommand(Ogre::UTFString("cl_maxfps"), MyGUI::newDelegate(this, &GameCore::cmd_maxFPS));
   mConsole->addCommand(Ogre::UTFString("cl_shownet"), MyGUI::newDelegate(this, &GameCore::cmd_showNet));
   mConsole->addCommand(Ogre::UTFString("cl_showboundingbox"), MyGUI::newDelegate(this, &GameCore::cmd_showBoundingBox));
   mConsole->addCommand(Ogre::UTFString("cl_screenshot"), MyGUI::newDelegate(this, &GameCore::cmd_screenshot));
   mConsole->addCommand(Ogre::UTFString("rcon_password"), MyGUI::newDelegate(this, &GameCore::cmd_remoteConnect));
   mWaitTime = ceil((float)1000/mMaxFPS);
}

void GameCore::cmd_showFPS(const Ogre::UTFString &key, const Ogre::UTFString &value)
{
   bool show = false;
   if (!MyGUI::utility::parseComplex(value, show))
   {
      if (!value.empty())
      {
         mConsole->addToConsole(mConsole->getConsoleError(), key, value);
      }
      mConsole->addToConsole(mConsole->getConsoleFormat(), key, "[true|false] - "+Ogre::UTFString("Show debug information"));
   }
   else
   {
      mConsole->addToConsole(mConsole->getConsoleSuccess(), key, value);
      mShowFPS = show;
      if (!show)
      {
         /* hide the overlay */
         Ogre::Overlay* o = Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");
         o->hide();
      }
   }
}

void GameCore::cmd_maxFPS(const Ogre::UTFString &key, const Ogre::UTFString &value)
{
   unsigned short newfps = 60;
   if (!MyGUI::utility::parseComplex(value, newfps))
   {
      if (!value.empty())
      {
         mConsole->addToConsole(mConsole->getConsoleError(), key, value);
      }
      mConsole->addToConsole(mConsole->getConsoleFormat(), key, "[integer] - "+Ogre::UTFString("Set the max FPS limit"));
   }
   else
   {
      if (newfps > 25)
      {
         mWaitTime = ceil((float)1000/newfps);
         mMaxFPS = newfps;
         mConsole->addToConsole(mConsole->getConsoleSuccess(), key, value);
      }
   }
}

void GameCore::cmd_showBoundingBox(const Ogre::UTFString &key, const Ogre::UTFString &value)
{
   bool show = false;
   if (!MyGUI::utility::parseComplex(value, show))
   {
      if (!value.empty())
      {
         mConsole->addToConsole(mConsole->getConsoleError(), key, value);
      }
      mConsole->addToConsole(mConsole->getConsoleFormat(), key, "[true|false] - "+Ogre::UTFString("Show bounding boxes (for debug purposes)"));
   }
   else
   {
      mConsole->addToConsole(mConsole->getConsoleSuccess(), key, value);
      Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr")->showBoundingBoxes(show);
      Ogre::Root::getSingletonPtr()->getSceneManager("GameSceneMgr")->getRootSceneNode()->showBoundingBox(show);
   }
}

void GameCore::cmd_screenshot(const Ogre::UTFString &key, const Ogre::UTFString &value)
{
   Ogre::String filename;
   if (!MyGUI::utility::parseComplex(value, filename))
   {
      if (!value.empty())
      {
         mConsole->addToConsole(mConsole->getConsoleError(), key, value);
      }
   }
   /* FIXME: Should add regex here to stop the user from putting in stupid
      directory locations */
   /* TODO: Make the screenshot path configurable */
   boost::filesystem::path lPath("./screenshots");
   Ogre::StringUtil::trim(filename);
   unsigned short total = 0;
   if (filename.empty())
   {
      if (boost::filesystem::is_directory(lPath))
      {
         for (boost::filesystem::directory_iterator itr(lPath); itr!=boost::filesystem::directory_iterator(); ++itr)
         {
            Ogre::String temp = (Ogre::String) itr->path().leaf();
            if (temp.substr(temp.length()-3, 3).compare("png") == 0)
            {
               total++;
            }
         }
      }
      filename = Ogre::UTFString("screenshot_")+Ogre::StringConverter::toString(total)+Ogre::UTFString(".png");
   }

   mConsole->addToConsole(mConsole->getConsoleFormat(), key, Ogre::UTFString("Taking Screenshot."));
   filename = Ogre::UTFString("./screenshots/")+filename;
   Ogre::Root::getSingletonPtr()->getAutoCreatedWindow()->writeContentsToFile(filename.c_str());
}

void GameCore::cmd_showNet(const Ogre::UTFString &key, const Ogre::UTFString &value)
{
   bool show = false;
   if (!MyGUI::utility::parseComplex(value, show))
   {
      if (!value.empty())
      {
         mConsole->addToConsole(mConsole->getConsoleError(), key, value);
      }
      mConsole->addToConsole(mConsole->getConsoleFormat(), key, "[true|false] - "+Ogre::UTFString("Show network information"));
   }
   else
   {
      mConsole->addToConsole(mConsole->getConsoleSuccess(), key, value);
      mShowNet = show;
      if (!show)
      {
         /* hide the overlay */
         Ogre::Overlay* o = Ogre::OverlayManager::getSingleton().getByName("Hardwar/NetStats");
         o->hide();
      }
   }
}

void GameCore::cmd_remoteConnect(const Ogre::UTFString &key, const Ogre::UTFString &value)
{
   Ogre::String lString;
   if (!MyGUI::utility::parseComplex(value, lString))
   {
      if (!value.empty())
      {
         mConsole->addToConsole(mConsole->getConsoleError(), key, Ogre::UTFString("****"));
      }
      mConsole->addToConsole(mConsole->getConsoleFormat(), key, "[password] - "+Ogre::UTFString("Gain administrator privileges on the remote server"));
   }
   else
   {
      dataPacket packet = dataPacket(admin_login);
      lString = md5(lString.c_str());
      packet.appendString(lString);
      Network::getSingletonPtr()->message(packet, SERVER_CHANNEL_ADMIN, ENET_PACKET_FLAG_RELIABLE);
      /* reponse is dealt with in the Network thread */
   }
}

unsigned short GameCore::getDelayTime(void)
{
   return mWaitTime;
}

void GameCore::update(unsigned long lTimeElapsed)
{
   mFPStimer += lTimeElapsed;
   if (mFPStimer > 0.5)
   {
      /* Don't need to update this stuff every frame */
      mFPStimer = 0;
      if (mShowFPS) showFPS();
      if (mShowNet) showNet();
   }
}

void GameCore::showNet(void)
{
   static Ogre::String lPing     = gettext("Ping: ");
   static Ogre::String lIncData  = gettext("Incomming Data: ");
   static Ogre::String lOutData  = gettext("Outgoing Data: ");
   static Ogre::String lPackSent = gettext("Packets Sent: ");
   static Ogre::String lPackLost = gettext("Packets Lost: ");
   static Ogre::String lTimeout  = gettext("Next Timeout: ");

   Ogre::OverlayManager *lOverlayMgr = Ogre::OverlayManager::getSingletonPtr();

   /* update stats when necessary */
   Ogre::OverlayElement* lguiPing     = lOverlayMgr->getOverlayElement("Hardwar/Ping");
   Ogre::OverlayElement* lguiIncData  = lOverlayMgr->getOverlayElement("Hardwar/Incomming");
   Ogre::OverlayElement* lguiOutData  = lOverlayMgr->getOverlayElement("Hardwar/Outgoing");
   Ogre::OverlayElement* lguiPackSent = lOverlayMgr->getOverlayElement("Hardwar/PacketsSent");
   Ogre::OverlayElement* lguiPackLost = lOverlayMgr->getOverlayElement("Hardwar/PacketsLost");
   Ogre::OverlayElement* lguiTimeout  = lOverlayMgr->getOverlayElement("Hardwar/Timeout");

   ENetHost *lHost = Network::getSingletonPtr()->mNetHost;

   lguiPing->setCaption(lPing + Ogre::StringConverter::toString(lHost->lastServicedPeer->roundTripTime) + Ogre::String(" ms"));
   lguiIncData->setCaption(lIncData + Ogre::StringConverter::toString(Ogre::Real(lHost->incomingBandwidth/1000)) + Ogre::String(" Kib"));
   lguiOutData->setCaption(lOutData + Ogre::StringConverter::toString(Ogre::Real(lHost->outgoingBandwidth/1000)) + Ogre::String(" Kib"));
   lguiPackSent->setCaption(lPackSent + Ogre::StringConverter::toString(lHost->lastServicedPeer->packetsSent));
   lguiPackLost->setCaption(lPackLost + Ogre::StringConverter::toString(lHost->lastServicedPeer->packetsLost));
   lguiTimeout->setCaption("");

   Ogre::Overlay* o = Ogre::OverlayManager::getSingleton().getByName("Hardwar/NetStats");

   if (!o)
   {
      OGRE_EXCEPT( Ogre::Exception::ERR_ITEM_NOT_FOUND, gettext("Could not find overlay Hardwar/NetStats"), "GameCore::showNet" );
   }
   else
   {
      o->show();
   }
}

void GameCore::showFPS(void)
{
   static Ogre::String lcurrFps  = gettext("Current FPS: ");
   static Ogre::String lavgFps   = gettext("Average FPS: ");
   static Ogre::String lworstFps = gettext("Worst FPS: ");
   static Ogre::String lbestFps  = gettext("Best FPS: ");
   static Ogre::String ltris     = gettext("Triangle Count: ");
   static Ogre::String lbatch    = gettext("Batch Count: ");

   Ogre::OverlayManager *lOverlayMgr = Ogre::OverlayManager::getSingletonPtr();

   /* update stats when necessary */
   Ogre::OverlayElement* lguiAvg   = lOverlayMgr->getOverlayElement("Core/AverageFps");
   Ogre::OverlayElement* lguiCurr  = lOverlayMgr->getOverlayElement("Core/CurrFps");
   Ogre::OverlayElement* lguiBest  = lOverlayMgr->getOverlayElement("Core/WorstFps");
   Ogre::OverlayElement* lguiWorst = lOverlayMgr->getOverlayElement("Core/BestFps");
   Ogre::OverlayElement* lguiTris  = lOverlayMgr->getOverlayElement("Core/NumTris");
   Ogre::OverlayElement* lguiBatch = lOverlayMgr->getOverlayElement("Core/NumBatches");
   Ogre::OverlayElement* lguiImage = lOverlayMgr->getOverlayElement("Core/LogoPanel");
   lguiImage->hide();

   Ogre::RenderWindow *lWindow = Ogre::Root::getSingletonPtr()->getAutoCreatedWindow();

   lguiAvg->setCaption(lavgFps + Ogre::StringConverter::toString(lWindow->getAverageFPS()));
   lguiCurr->setCaption(lcurrFps + Ogre::StringConverter::toString(lWindow->getLastFPS()));
   lguiBest->setCaption(lbestFps + Ogre::StringConverter::toString(lWindow->getBestFPS()));
   lguiWorst->setCaption(lworstFps + Ogre::StringConverter::toString(lWindow->getWorstFPS()));
   lguiTris->setCaption(ltris + Ogre::StringConverter::toString(lWindow->getTriangleCount()));
   lguiBatch->setCaption(lbatch + Ogre::StringConverter::toString(lWindow->getBatchCount()));
   Ogre::Overlay* o = Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");

   if (!o)
   {
      OGRE_EXCEPT( Ogre::Exception::ERR_ITEM_NOT_FOUND, gettext("Could not find overlay Core/DebugOverlay"), "GameCore::showFPS" );
   }
   else
   {
      o->show();
   }
}

