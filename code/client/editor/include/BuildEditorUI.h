/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2010  Andrew Fenn
    
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

#ifndef __BuildEditorUI_H_
#define __BuildEditorUI_H_

#include <MyGUI.h>
#include <boost/filesystem.hpp>
#include <vector>
#include "GameSettings.h"

#include "ItemBoxMgr.h"
#include "GameManager.h"

namespace Client
{
   class BuildEditorUI
   {
      public:
         BuildEditorUI();
         ~BuildEditorUI();
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
         void mouseMoved(const OIS::MouseEvent &e);
         /** This method shows or hides the edit pane.
         @remarks
               If false then it shows the add building pane
         */
         void showEditPane(bool);
         /** This method adds a mesh to the list of available parts in the editor. */
         void addMesh(Ogre::UTFString);
         /** This method returns if the editor is visible. */
         bool isVisible();
         /** This method returns if there is an icon that is being placed on the screen. */
         bool hasPlaceableIcon();
         /** This method returns the active icon's coords if an active icon exists. */
         MyGUI::IntPoint getIconCoords();
         /** This method returns the active icon's mesh name. */
         Ogre::UTFString getIconName();
         /** Shows or hides the editor UI. */
         void show(bool);
         void update(unsigned long);
      private:
         Ogre::Root         *mRoot;
         Ogre::SceneManager *mSceneMgr;
         Ogre::SceneNode    *mEditorNode;
         Console            *mConsole;

         bool mShow;
         /** renders all the building icons in a building page and displays them in the UI */
         void renderBuildingList(unsigned short);
         /** renders a picture of the mesh for placement in the UI */
         void renderMesh(Ogre::UTFString, Ogre::UTFString);
         /** Checks which building page we're on and hide/shows the next/previous buttons */
         void checkUIButtons(void);

         Client::ItemBoxMgr mBoxMgr;
         MyGUI::Gui  *mGUI;
         MyGUI::StaticImagePtr mMenuBar;
         MyGUI::WidgetPtr mMenuPanelAdd;
         MyGUI::WidgetPtr mMenuPanelEdit;

         std::vector<Ogre::UTFString> mBuildingList;
         unsigned short mBuildingPage;
         unsigned short mBuildingMaxPage;
         /** Max number of building rows per page */
         unsigned short mMaxBuildingRows;
         /** Max number of building columns per page */
         unsigned short mMaxBuildingCols;
   };
}

#endif /* __BuildEditor_H_ */

