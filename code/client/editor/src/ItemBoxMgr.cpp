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
      mActive = mListItemBox.begin();
   }

   void ItemBoxMgr::addItem(Client::ItemBox* _itembox)
   {
      mListItemBox.push_back(_itembox);
   }
   
   void ItemBoxMgr::clear()
   {
      /* We have to destory the objects like this, otherwise the destructor
      is not called. However the item in the list remains so we need to clear
      that too. If you find a better way of doing this let me know. */
      for (mActive = mListItemBox.begin(); mActive != mListItemBox.end(); ++mActive)
      {
         delete (*mActive);
		}
      mListItemBox.clear();
   }

   bool ItemBoxMgr::isIconActive()
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

   bool ItemBoxMgr::isPlaceable()
   {
      return mPlaceMode;
   }

   Ogre::UTFString ItemBoxMgr::getMeshName()
   {
      return (*mActive)->getName();
   }

   MyGUI::IntCoord ItemBoxMgr::getCoord()
   {
      return mCoord;
   }

   void ItemBoxMgr::update()
   {
      MyGUI::Gui* mGUI = MyGUI::Gui::getInstancePtr();
      MyGUI::Window* editorWindow = mGUI->findWidget<MyGUI::Window>("BuildEditorWindow");
      MyGUI::IntSize size = editorWindow->getMaxSize();

      Ogre::Log* log = Ogre::LogManager::getSingletonPtr()->getDefaultLog();
      mPlaceMode = false;

      if (isIconActive())
      {
         mCoord = (*mActive)->getCoord();
         log->logMessage(Ogre::StringConverter::toString(mCoord.width)+" "+Ogre::StringConverter::toString(mCoord.height)+ " " +Ogre::StringConverter::toString(mCoord.top) + " "+Ogre::StringConverter::toString(mCoord.left)
         + "Size: "+Ogre::StringConverter::toString(size.width)+  " "+Ogre::StringConverter::toString(size.height)
         );

         if ((mCoord.left < 0 || mCoord.left > size.width)
            && (mCoord.top < 0 || mCoord.top > size.height))
         {
            mPlaceMode = true;
         }
      }
   }
}
