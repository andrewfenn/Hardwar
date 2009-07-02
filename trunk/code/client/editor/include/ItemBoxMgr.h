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

#ifndef __ITEM_BOX_MGR_H__
#define __ITEM_BOX_MGR_H__

#include <Ogre.h>
#include <MyGUI.h>
#include "ItemBox.h"
#include <vector>

#include "Console.h"

namespace Client
{
   class ItemBoxMgr
   {
      public:
         ItemBoxMgr();
         ~ItemBoxMgr();
         void addItem(Client::ItemBox*);
         void clear(void);
         void update(void);
         bool isIconActive(void);
         bool isPlaceable(void);
         Ogre::UTFString getMeshName(void);
         MyGUI::IntPoint getPoint(void);
      private:
         typedef std::vector<Client::ItemBox*> VectorItemBox;
   		VectorItemBox mListItemBox;
         VectorItemBox::iterator mActive;
         bool mPlaceMode;
         MyGUI::IntPoint mPoint;
   };
}
#endif // __ITEM_BOX_MGR_H__
