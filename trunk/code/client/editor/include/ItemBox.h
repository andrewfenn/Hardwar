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

#ifndef __ITEM_BOX_H__
#define __ITEM_BOX_H__

#include <Ogre.h>
#include <MyGUI.h>

namespace Client
{
   class ItemBox
   {
      public:
   /** This method is used to create a new building icon.
      @remarks
             It is important that you give this class the correct image and
             panel name otherwise it might screw up.
      @param ImageName
             Is the name of the image in Ogre's resources
      @param PanelName
             Is the name of the MyGUI::StaticImage panel where the image is
             being displayed to.
      */
         ItemBox(Ogre::UTFString, Ogre::UTFString, Ogre::UTFString);
         ~ItemBox();
         void moveItemBox(MyGUI::WidgetPtr, int, int);
         void releasedItemBox(MyGUI::WidgetPtr, int, int, MyGUI::MouseButton);
         MyGUI::IntPoint getPoint(void);
         bool isActive(void);
         Ogre::UTFString getName(void);
      private:
         MyGUI::StaticImage* mImage;
         MyGUI::IntCoord mOrigCoord;
         MyGUI::IntPoint mNewCoord;
         bool mDragged;
         Ogre::UTFString mMeshName;
	};
}
#endif // __ITEM_BOX_H__
