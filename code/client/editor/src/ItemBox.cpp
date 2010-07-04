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
#include "ItemBox.h"

namespace Client
{
   ItemBox::ItemBox(Ogre::UTFString _image, Ogre::UTFString _panel, Ogre::UTFString _meshName)
   {
      /* FIXME: Add checking to make sure both the image and panel exists */
      mImage = MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::StaticImage>(_panel);
      mImage->setImageTexture(_image);
      mImage->eventMouseDrag = MyGUI::newDelegate(this, &ItemBox::moveItemBox);
      mImage->eventMouseButtonReleased = MyGUI::newDelegate(this, &ItemBox::releasedItemBox);
      mNewCoord = mOrigCoord = mImage->getCoord();
      mDragged = false;
      mMeshName = _meshName;
   }

   ItemBox::~ItemBox()
   {
      mImage->deleteAllItems();
   }

   void ItemBox::moveItemBox(MyGUI::WidgetPtr sender, int _left, int _top)
   {
      mNewCoord.left = _left;
      mNewCoord.top  = _top;
      MyGUI::IntPoint pos;
      pos.left = _left;
      pos.top = _top;
      mImage->setPosition(pos);
      mDragged = true;
      MyGUI::Gui::getInstancePtr()->hidePointer();
   }

   MyGUI::IntCoord ItemBox::getCoord(void)
   {
      return mNewCoord;
   }

   Ogre::UTFString ItemBox::getName(void)
   {
      return mMeshName;
   }

   void ItemBox::releasedItemBox(MyGUI::WidgetPtr sender, int _left, int _top, MyGUI::MouseButton _id)
   {
      mImage->setCoord(mOrigCoord);
      mDragged = false;
      MyGUI::Gui::getInstancePtr()->showPointer();
   }

   bool ItemBox::isActive(void)
   {
      return mDragged;
   }
}
