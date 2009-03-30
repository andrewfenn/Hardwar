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

#include "Console.h"

Console::Console()
{
   mShow = false;
   mAlpha = 0.0f;
   mGUI = MyGUI::Gui::getInstancePtr();
   MyGUI::LayoutManager::getInstance().load("console.layout");
   mGUIConsole = mGUI->findWidget<MyGUI::Window>("console");
   mGUIConsole->setAlpha(mAlpha);
}

Console::~Console()
{

}

void Console::toggleShow()
{
   if (mShow)
   {
      mShow = false;
      mGUI->hidePointer();
   }
   else
   {
      mShow = true;
      mGUIConsole->setVisible(true);
      mGUI->showPointer();
   }
}

bool Console::isVisible()
{
   return mShow;
}

void Console::update()
{
   if (mShow && mAlpha < 0.9f)
   {
      mAlpha += 0.01f;
   }
   else if (!mShow && mAlpha > 0.01f)
   {
      mAlpha -= 0.01f;
      if (mAlpha < 0.02f)
         mGUIConsole->setVisible(false);
   }
   mGUIConsole->setAlpha(mAlpha);
}
