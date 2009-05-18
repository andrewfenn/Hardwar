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
#include <OgreRoot.h>

#include "GameSettings.h"
#include "Console.h"

namespace Client
{

class BuildEditor
{
public:
   BuildEditor();
   ~BuildEditor(void);
   void update(unsigned long);

   void cmd_showEditor(const Ogre::UTFString&, const Ogre::UTFString&);
private:
   bool mShow;
   void toggleShow(bool);
   MyGUI::Gui  *mGUI;
   MyGUI::StaticImagePtr mMenuBar;
   MyGUI::WidgetPtr mMenuPanel;
};

}
#endif /* __BuildEditor_H_ */

