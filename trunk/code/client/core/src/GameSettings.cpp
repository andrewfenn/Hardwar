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

#include "GameSettings.h"

using namespace Client;

GameSettings* GameSettings::mGameSettings;

GameSettings::GameSettings(void)
{
   mMaxFPS = 60;
   mShowFPS = true;
   mShowNet = true;
   /* TODO: Everything above should be configurable from file */
   mConsole = Console::getSingletonPtr();
   mConsole->addCommand(Ogre::UTFString("cl_showfps"), MyGUI::newDelegate(this, &GameSettings::cmd_showFPS));
   mConsole->addCommand(Ogre::UTFString("cl_maxfps"), MyGUI::newDelegate(this, &GameSettings::cmd_maxFPS));
   mConsole->addCommand(Ogre::UTFString("cl_shownet"), MyGUI::newDelegate(this, &GameSettings::cmd_showNet));
   mConsole->addCommand(Ogre::UTFString("rcon_password"), MyGUI::newDelegate(this, &GameSettings::cmd_remoteConnect));
   mWaitTime = ceil(1000/mMaxFPS);
}

GameSettings::~GameSettings(void)
{

}

void GameSettings::setOption(const Ogre::UTFString lName, Ogre::UTFString lValue)
{
   mOptions[lName] = lValue;
}

Ogre::UTFString GameSettings::getOption(const Ogre::UTFString lName)
{
   Option::iterator iter = mOptions.find(lName);
   if (iter != mOptions.end())
   {
      return mOptions[lName];
   }
   return Ogre::UTFString("");
}

void GameSettings::cmd_showFPS(const Ogre::UTFString &key, const Ogre::UTFString &value)
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

void GameSettings::cmd_maxFPS(const Ogre::UTFString &key, const Ogre::UTFString &value)
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
         mWaitTime = ceil(1000/newfps);
         mMaxFPS = newfps;
         mConsole->addToConsole(mConsole->getConsoleSuccess(), key, value);
      }
   }
}

void GameSettings::cmd_showNet(const Ogre::UTFString &key, const Ogre::UTFString &value)
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

void GameSettings::cmd_remoteConnect(const Ogre::UTFString &key, const Ogre::UTFString &value)
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
      Network::getSingletonPtr()->message("login", strlen("login")+1, SERVER_CHANNEL_ADMIN, ENET_PACKET_FLAG_RELIABLE);
      lString = md5(lString.c_str());
      Network::getSingletonPtr()->message(lString.c_str(), strlen(lString.c_str())+1, SERVER_CHANNEL_ADMIN, ENET_PACKET_FLAG_RELIABLE);
      /* reponse is dealt with in the Network thread */
   }
}

unsigned short GameSettings::getDelayTime(void)
{
   return mWaitTime;
}

void GameSettings::update(unsigned long lTimeElapsed)
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

void GameSettings::showNet(void)
{
   static Ogre::String lPing = "Ping: ";
   static Ogre::String lIncData = "Incomming Data: ";
   static Ogre::String lOutData = "Outgoing Data: ";
   static Ogre::String lPackSent = "Packets Sent: ";
   static Ogre::String lPackLost = "Packets Lost: ";
   static Ogre::String lTimeout = "Next Timeout: ";

   Ogre::OverlayManager *lOverlayMgr = Ogre::OverlayManager::getSingletonPtr();

   /* update stats when necessary */
   Ogre::OverlayElement* lguiPing   = lOverlayMgr->getOverlayElement("Hardwar/Ping");
   Ogre::OverlayElement* lguiIncData  = lOverlayMgr->getOverlayElement("Hardwar/Incomming");
   Ogre::OverlayElement* lguiOutData  = lOverlayMgr->getOverlayElement("Hardwar/Outgoing");
   Ogre::OverlayElement* lguiPackSent = lOverlayMgr->getOverlayElement("Hardwar/PacketsSent");
   Ogre::OverlayElement* lguiPackLost  = lOverlayMgr->getOverlayElement("Hardwar/PacketsLost");
   Ogre::OverlayElement* lguiTimeout = lOverlayMgr->getOverlayElement("Hardwar/Timeout");

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
      OGRE_EXCEPT( Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find overlay Hardwar/NetStats", "GameSettings::showNet" );
   }
   else
   {
      o->show();
   }
}

void GameSettings::showFPS(void)
{
   static Ogre::String lcurrFps = "Current FPS: ";
   static Ogre::String lavgFps = "Average FPS: ";
   static Ogre::String lworstFps = "Worst FPS: ";
   static Ogre::String lbestFps = "Best FPS: ";
   static Ogre::String ltris = "Triangle Count: ";
   static Ogre::String lbatch = "Batch Count: ";

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
      OGRE_EXCEPT( Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find overlay Core/DebugOverlay", "GameSettings::showFPS" );
   }
   else
   {
      o->show();
   }
}

GameSettings* GameSettings::getSingletonPtr(void)
{
   if(!mGameSettings)
   {
      mGameSettings = new GameSettings();
   }

   return mGameSettings;
}
