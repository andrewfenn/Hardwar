/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2009  Andrew Fenn
    
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

#ifndef __AxisEditor_H_
#define __AxisEditor_H_

#include <Ogre.h>
#include "CollisionTools.h"

namespace Client
{
/** EditorAxis
        @remarks
            Deals with using and displaying the Axis when moving around buildings or objects.
    */
class EditorAxis
{
   public:
      EditorAxis(void);
      ~EditorAxis(void);
      void selectBuilding(const Ogre::Ray);
      Ogre::Entity* getSelectedAxis(void);
      void clearSelectedAxis(void);
      void moveBuilding(const Ogre::Ray);
      void remove(void);
   private:
      void removeSelectedObj(void);
      void createAxis(Ogre::Entity*);
      void destoryAxis(Ogre::MovableObject*);
      MOC::CollisionTools* mCollision;
      bool mEditorObjSelected;
      Ogre::MovableObject * mSelected;
      void setSelectedAxis(Ogre::Entity*);
      Ogre::Entity* mSelectedAxis;
      Ogre::uint32 mAxisFlag;
};
}
#endif /* __AxisEditor_H_ */
