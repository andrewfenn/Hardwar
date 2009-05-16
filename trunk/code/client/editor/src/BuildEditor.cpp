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

#include "BuildEditor.h"

using namespace Client;

BuildEditor::BuildEditor(void)
{
   mShow = false;
   mGUI = MyGUI::Gui::getInstancePtr();
   MyGUI::LayoutManager::getInstance().load("build_editor.layout");
   mMenuBar = mGUI->findWidget<MyGUI::StaticImage>("BuildEditorMenuTop");
   mMenuBar->setVisible(false);
   mMenuPanel = mGUI->findWidget<MyGUI::Widget>("BuildEditorMenuBottom");
   mMenuPanel->setVisible(false);
}

BuildEditor::~BuildEditor(void)
{
   /* TODO: unload build editor resources */
}

void BuildEditor::toggleShow(void)
{
   if (mShow)
   {
      mShow = false;
      mMenuPanel->setVisible(false);
      mMenuBar->setVisible(false);
      mGUI->hidePointer();
   }
   else
   {
      mShow = true;
      mMenuPanel->setVisible(true);
      mMenuBar->setVisible(true);
      mGUI->showPointer();
   }
}
