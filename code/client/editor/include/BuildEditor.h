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

#include "GameSettings.h"
#include "GameManager.h"
#include "Console.h"
#include "ItemBoxMgr.h"
#include "CollisionTools.h"
#include "DynamicLines.h"

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
   bool isVisible();
private:
   Ogre::Root         *mRoot;
   Ogre::SceneManager *mSceneMgr;

   bool mShow;
   void toggleShow(bool);
   void renderBuildingList();
   void renderMesh(Ogre::UTFString, Ogre::UTFString);
   Client::ItemBoxMgr  mBoxMgr;
   MyGUI::Gui  *mGUI;
   MyGUI::StaticImagePtr mMenuBar;
   MyGUI::WidgetPtr mMenuPanel;

   bool mHasEditorObj;
   Ogre::SceneNode* mEditorNode;
   MOC::CollisionTools* mCollision;
};

}
#endif /* __BuildEditor_H_ */

