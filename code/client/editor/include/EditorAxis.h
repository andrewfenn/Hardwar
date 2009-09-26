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
#include <MyGUI.h>
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
      /**
         Looks for any buildings that are in the ray and selects the closest
      */
      void selectBuilding(const Ogre::Ray);
      /**
         Returns the axis that has been click dragged by the user
      */
      Ogre::Entity* getSelectedAxis(void);
      /**
         Clears the axis selected by the mouse pointer when moving buildings */
      void clearSelectedAxis(void);
      /**
         Moves a building to a specified point defined by the ray cast */
      void moveBuilding(const Ogre::Ray);
      /**
         Removes the axis from whatever building it is on */
      void remove(void);
      /**
         Reports if an object has been selected or not */
      bool objectSelected(void);
      /**
         Updates the information in the editor panel for a selected object */
      void updateSelectedUI(void);
      /**
         Toggles which axis to show. The position axis or rotation axis
         @param show
            If set to true it will show the rotation axis, otherwise the
         position axis is shown.
      */
      void showRotAxis(bool);
      /**
         Returns if the axis shown is the Rotation axis.
         @remarks
            If true the rotation axis is shown. If false then the
         position axis is currently displayed. If this function is
         called while an object isn't selected it will return false.
      */
      bool isRotAxis(void);
   private:
      void removeSelectedObj(void);
      /**
         Creates the position axis on the selected entity
         @param Entity
            The object to attach the axis to
         @remarks
            This function assumes that you have called destoryAxis
         before calling this one
      */
      void createPosAxis(Ogre::Entity*);
      /**
         Creates the rotation axis on the selected entity
         @param Entity
            The object to attach the axis to
         @remarks
            This function assumes that you have called destoryAxis
         before calling this one
      */
      void createRotAxis(Ogre::Entity*);
      void createPlane(void);
      void destoryAxis(Ogre::MovableObject*);
      MOC::CollisionTools* mCollision;
      bool mEditorObjSelected;
      bool mRotAxisShown;
      Ogre::MovableObject * mSelected;
      void setSelectedAxis(Ogre::Entity*);
      Ogre::Entity* mSelectedAxis;
      Ogre::uint32 mAxisFlag;
      Ogre::uint32 mCollisionFlag;
};
}
#endif /* __AxisEditor_H_ */
