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
   mCommandBox = mGUI->findWidget<MyGUI::ComboBox>("commandbox");
   mHistoryList = mGUI->findWidget<MyGUI::Edit>("historylist");
   mSubmitButton = mGUI->findWidget<MyGUI::Button>("submit");

   mStringCurrent = mGUIConsole->getUserString("Current");
   mStringError   = mGUIConsole->getUserString("Error");
   mStringSuccess = mGUIConsole->getUserString("Success");
   mStringUnknown = mGUIConsole->getUserString("Unknown");
   mStringFormat  = mGUIConsole->getUserString("Format");

	mCommandBox->eventComboAccept = newDelegate(this, &Console::notifyCommandAccept);
	mCommandBox->eventKeyButtonPressed = newDelegate(this, &Console::notifyCommandTyped);
	//mButtonSubmit->eventMouseButtonClick = newDelegate(this, &Console::notifyMouseButtonClick);

   mSubmitButton->setCaption(Ogre::UTFString(gettext("submit")));
   mGUIConsole->setAlpha(mAlpha);
}

Console::~Console()
{

}

/*
 * Function: addCommand
 * Description: Adds functions to the console so they can be used
 *
 */
bool Console::addCommand(const Ogre::UTFString &name, CommandDelegate::IDelegate* function, const Ogre::UTFString &description)
{
   mCommandBox->addItem(name);
   MapFunction::iterator iter = mFunctions.find(name);
   if (iter != mFunctions.end()) {
      /* already exists */
      return false;
   }
   mFunctions[name] = function;
   return true;
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

void Console::notifyCommandTyped(MyGUI::WidgetPtr sender, MyGUI::KeyCode key, MyGUI::Char character)
{
   size_t len = sender->getCaption().length();
   MyGUI::EditPtr edit = sender->castType<MyGUI::Edit>();
   if ((key == MyGUI::KeyCode::Backspace) && (len > 0) && (mAutoCompleted))
   {
	   edit->deleteTextSelection();
	   len = sender->getCaption().length();
	   edit->eraseText(len-1);
   }

   Ogre::UTFString command = sender->getCaption();
   if (command.length() == 0) return;

   for (MapFunction::iterator iter = mFunctions.begin(); iter != mFunctions.end(); ++iter)
   {
	   if (iter->first.find(command) == 0)
	   {
		   if (command == iter->first) break;
		   edit->setCaption(iter->first);
		   edit->setTextSelection(command.length(), iter->first.length());
		   mAutoCompleted = true;
		   return;
	   }
   }
   mAutoCompleted = false;
}

void Console::notifyCommandAccept(MyGUI::ComboBoxPtr sender, size_t index)
{
	const Ogre::UTFString & command = sender->getCaption();
	if (command == "") return;

	Ogre::UTFString key = command;
	Ogre::UTFString value;

	size_t pos = command.find(' ');
	if (pos != Ogre::UTFString::npos)
   {
		key = command.substr(0, pos);
		value = command.substr(pos + 1);
	}

	MapFunction::iterator iter = mFunctions.find(key);
	if (iter != mFunctions.end())
   {
		iter->second(key, value);
	}
	else
   {
		if (eventConsoleUnknowCommand.empty())
      {
			addToConsole(mStringUnknown + "'" + key + "'");
		}
		else
      {
			eventConsoleUnknowCommand(key, value);
		}
	}
	sender->setCaption("");
}

void Console::addToConsole(const Ogre::UTFString & line)
{
   if (mHistoryList->getCaption().empty())
   {
      mHistoryList->addText(line);
   }
   else
   {
      mHistoryList->addText("\n" + line);
   }
   mHistoryList->setTextSelection(mHistoryList->getTextLength(), mHistoryList->getTextLength());
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
