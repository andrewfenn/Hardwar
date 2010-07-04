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

#ifndef __BuildEditor_H_
#define __BuildEditor_H_

#include <Ogre.h>
#include <OgreRay.h>

#include <libintl.h>

#include "GameManager.h"
#include "Console.h"
#include "CollisionTools.h"
#include "DynamicLines.h"
#include "EditorAxis.h"
#include "BuildEditorUI.h"

#include "Building.h"
#include "srvstructs.h"

namespace Client
{
/** The Build Editor
        @remarks
            The Client::BuildEditor class deals with editing the levels in the
            game such as placing down buildings.
    */
class BuildEditor
{
public:
   BuildEditor();
   ~BuildEditor();
   void update(unsigned long);
   void keyPressed(const OIS::KeyEvent &e);
   void keyReleased(const OIS::KeyEvent &e);
   void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
   void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
   void mouseMoved(const OIS::MouseEvent &e);
   bool isVisible();
   void show(bool);
private:
   Ogre::Root         *mRoot;
   Ogre::SceneManager *mSceneMgr;
   Ogre::SceneNode    *mEditorNode;

   /** looks in the directory for all .mesh files and organises them into building pages
   @remarks
         A building page is an Ogre::String vector that can hold up to 16 mesh files names
    */
   void generateBuildingList();
   /** Generates an Ogre::Ray from the viewport
   @param x
           The x value usually taken from mouse x value
   @param y
           The y value usually taken from mouse y value
  */
   Ogre::Ray makeRay(const unsigned int, const unsigned int);
   void destroyEditorObj();
   Ogre::String mEditorObjMeshName;
   /** When true an object has been created and is about to be placed on the level. */
   bool mEditorObjCreated;
   /** When true a pre-existing building is being edited (moved). */
   bool mEditorObjEditing;
   MOC::CollisionTools* mCollision;
   EditorAxis mAxis;
   BuildEditorUI mUI;
};

}
#endif /* __BuildEditor_H_ */

