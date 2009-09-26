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

#ifndef __BuildEditor_H_
#define __BuildEditor_H_

#include <MyGUI.h>
#include <Ogre.h>
#include <OgreRay.h>

#include <libintl.h>
#include <boost/filesystem.hpp>
#include <vector>

#include "GameSettings.h"
#include "GameManager.h"
#include "Console.h"
#include "ItemBoxMgr.h"
#include "CollisionTools.h"
#include "DynamicLines.h"
#include "EditorAxis.h"

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
   ~BuildEditor(void);
   void update(unsigned long);
   /** Shows or hides the editor panel, however keeps the top menu bar visible. */
   void toggleMinimise(MyGUI::WidgetPtr);
   /** Tells the UI to display the next page of buildings in the menu */
   void buttonNext(MyGUI::WidgetPtr);
   /** Tells the UI to display the previous page of buildings in the menu */
   void buttonPrevious(MyGUI::WidgetPtr);
   /** This method is used by Client::Console to show or hide the editor menu.
   @remarks
          The editor can only be accessed in the client has used rcon_password
          to login to the server and gain admin privileges.
   @param key
          The command name typed in the console.
   @param value
           A boolean value that either shows or hides the window.
   */
   void cmd_showEditor(const Ogre::UTFString&, const Ogre::UTFString&);
   void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
   void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
   void keyReleased(const OIS::KeyEvent &e);
   void mouseMoved(const OIS::MouseEvent &e);
   bool isVisible();
   void show(bool);
private:
   Ogre::Root         *mRoot;
   Ogre::SceneManager *mSceneMgr;

   bool mShow;
   /** looks in the directory for all .mesh files and organises them into building pages
   @remarks
         A building page is an Ogre::String vector that can hold up to 16 mesh files names
    */
   void generateBuildingList();
   /** renders all the building icons in a building page and displays them in the UI */
   void renderBuildingList(unsigned short);
   /** renders a picture of the mesh for placement in the UI */
   void renderMesh(Ogre::UTFString, Ogre::UTFString);
   /** Checks which building page we're on and hide/shows the next/previous buttons */
   void checkUIButtons(void);
   /** Generates an Ogre::Ray from the viewport
   @param x
           The x value usually taken from mouse x value
   @param y
           The y value usually taken from mouse y value
  */
   Ogre::Ray makeRay(const unsigned int, const unsigned int);
   Client::ItemBoxMgr  mBoxMgr;
   MyGUI::Gui  *mGUI;
   MyGUI::StaticImagePtr mMenuBar;
   MyGUI::WidgetPtr mMenuPanelAdd;
   MyGUI::WidgetPtr mMenuPanelEdit;

   Ogre::String mEditorObjMeshName;
   bool mEditorObjCreated;
   Ogre::SceneNode* mEditorNode;
   MOC::CollisionTools* mCollision;
   typedef std::vector<Ogre::UTFString> BuildingPage;
   typedef std::map<unsigned short, BuildingPage> BuildingList;
   BuildingList mBuildingList;
   unsigned short mBuildingPage;
   unsigned short mBuildingMaxPage;
   EditorAxis mAxis;
};

}
#endif /* __BuildEditor_H_ */

