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
#include "ItemBoxMgr.h"

namespace Client
{
   ItemBoxMgr::ItemBoxMgr()
   {
      mPlaceMode = false;
   }

   ItemBoxMgr::~ItemBoxMgr()
   {
      mListItemBox.clear();
   }

   void ItemBoxMgr::addItem(Client::ItemBox* _itembox)
   {
      mListItemBox.push_back(_itembox);
   }
   
   void ItemBoxMgr::clear(void)
   {
      mListItemBox.clear();
   }

   bool ItemBoxMgr::isIconActive(void)
   {
      for (mActive = mListItemBox.begin(); mActive != mListItemBox.end(); ++mActive)
      {
         if ((*mActive)->isActive())
         {
            return true;
         }
		}
      mPlaceMode = false;
      return false;
   }

   bool ItemBoxMgr::isPlaceable(void)
   {
      return mPlaceMode;
   }

   Ogre::UTFString ItemBoxMgr::getMeshName(void)
   {
      return (*mActive)->getName();
   }

   MyGUI::IntPoint ItemBoxMgr::getPoint(void)
   {
      return mPoint;
   }

   void ItemBoxMgr::update(void)
   {
      mPlaceMode = false;
      if (isIconActive())
      {
         mPoint = (*mActive)->getPoint();
         mPoint.left += 40;
         mPoint.top += 40;
         if (!mPlaceMode)
         {
            if (mPoint.left > 360 || mPoint.top > 570)
            {
               mPlaceMode = true;
            }
         }
      }
   }
}
