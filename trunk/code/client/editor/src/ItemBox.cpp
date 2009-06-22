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
#include "ItemBox.h"

namespace Client
{
   ItemBox::ItemBox(Ogre::UTFString _image, Ogre::UTFString _panel)
   {
      /* FIXME: Add checking to make sure both the image and panel exists */
      mImage = MyGUI::Gui::getInstancePtr()->findWidget<MyGUI::StaticImage>(_panel);
      mImage->setImageTexture(_image);
      mImage->eventMouseDrag = MyGUI::newDelegate(this, &ItemBox::moveItemBox);
      mImage->eventMouseButtonReleased = MyGUI::newDelegate(this, &ItemBox::releasedItemBox);
      mOrigCoord = mImage->getCoord();
      mDragged = false;
   }

   ItemBox::~ItemBox()
   {

   }

   void ItemBox::moveItemBox(MyGUI::WidgetPtr sender, int _left, int _top)
   {
      mNewCoord.left = _left-40;
      mNewCoord.top  = _top-60;
      mImage->setPosition(mNewCoord);
      mDragged = true;
      MyGUI::Gui::getInstancePtr()->hidePointer();
   }

   void ItemBox::releasedItemBox(MyGUI::WidgetPtr sender, int _left, int _top, MyGUI::MouseButton _id)
   {
      mImage->setCoord(mOrigCoord);
      mDragged = false;
      MyGUI::Gui::getInstancePtr()->showPointer();
   }

   bool ItemBox::isActive()
   {
      return mDragged;
   }
}
