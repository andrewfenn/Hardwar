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

GameSettings::GameSettings(void)
{
   mShowFPS = false;
   mConsole = Console::getSingletonPtr();
   mConsole->addCommand(Ogre::UTFString("cl_showfps"), MyGUI::newDelegate(this, &GameSettings::cmd_showFPS));
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

void GameSettings::update(unsigned long lTimeElapsed)
{
   mFPStimer += lTimeElapsed;
   if (mFPStimer > 0.5)
   {
      /* Don't need to update this stuff every frame */
      mFPStimer = 0;
      if (mShowFPS) showFPS();
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
      OGRE_EXCEPT( Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find overlay Core/DebugOverlay", "showDebugOverlay" );
   }
   else
   {
      o->show();
   }
}
