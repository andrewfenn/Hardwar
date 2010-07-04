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

#include "Console.h"

using namespace Client;

Console::Console()
{
   mShow = false;
   mKeptTempWord = false;
   mListPos = 0;
}

Console::~Console()
{
}

void Console::load()
{
   mGUI = MyGUI::Gui::getInstancePtr();

   MyGUI::LayoutManager::getInstance().load("console.layout");

   mGUIConsole = mGUI->findWidget<MyGUI::Window>("console");
   mCommandBox = mGUI->findWidget<MyGUI::ComboBox>("commandbox");
   mHistoryList = mGUI->findWidget<MyGUI::Edit>("historylist");
   mHistoryList->setOverflowToTheLeft(true);
   mSubmitButton = mGUI->findWidget<MyGUI::Button>("submit");

   mStringCurrent = mGUIConsole->getUserString("Current");
   mStringError   = mGUIConsole->getUserString("Error");
   mStringSuccess = mGUIConsole->getUserString("Success");
   mStringUnknown = mGUIConsole->getUserString("Unknown");
   mStringFormat  = mGUIConsole->getUserString("Format");

   mCommandBox->eventComboAccept = newDelegate(this, &Console::notifyCommandAccept);
   mCommandBox->eventKeyButtonPressed = newDelegate(this, &Console::notifyCommandTyped);
   mSubmitButton->eventMouseButtonClick = newDelegate(this, &Console::notifySubmitButtonClick);

   mSubmitButton->setCaption(MyGUI::UString(gettext("submit")));
   mGUIConsole->setVisible(false);
   mGUIConsole->setEnabled(false);
}

bool Console::addCommand(const MyGUI::UString &name, CommandDelegate::IDelegate* function)
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
   mShow = !mShow;
   mGUI->setVisiblePointer(mShow);
   mGUIConsole->setVisible(mShow);
   mGUIConsole->setEnabled(mShow);

   if (mShow)
   {
       MyGUI::InputManager::getInstance().setKeyFocusWidget(mCommandBox);
   }
}

void Console::notifySubmitButtonClick(MyGUI::WidgetPtr sender)
{
   notifyCommandAccept(mCommandBox, MyGUI::ITEM_NONE);
}

void Console::notifyCommandTyped(MyGUI::WidgetPtr sender, MyGUI::KeyCode key, MyGUI::Char character)
{
   size_t len = sender->getCaption().length();
 
   MyGUI::EditPtr edit = sender->castType<MyGUI::Edit>();
   if ((key == MyGUI::KeyCode::Backspace) && (len > 0))
   {
	   edit->eraseText(len);
      return;
   }

   /* make tab key move cursor to the end */
   if ((key == MyGUI::KeyCode::Tab) && (len > 0) && (mAutoCompleted))
   {
      edit->setTextSelection(len, len);
   }
/*
   if (!mAutoCompleted && key == MyGUI::KeyCode::ArrowUp && mListPos < mUsedCommands.size())
   {
      if (mListPos==0 && sender->getCaption().length() > 0)
      {
         mKeptTempWord = true;
         mUsedCommands.push_back(sender->getCaption());
         mListPos++;
      }

      sender->setCaption(mUsedCommands.at((mUsedCommands.size()-1)-mListPos));
      len = sender->getCaption().length();
      edit->setTextSelection(len, len);
      mListPos++;
      return;
   }

   if (!mAutoCompleted && key == MyGUI::KeyCode::ArrowDown && (mListPos==0 || mListPos > 0))
   {
      if (mListPos == mUsedCommands.size()) mListPos--;
      sender->setCaption(mUsedCommands.at((mUsedCommands.size()-1)-mListPos));
      len = sender->getCaption().length();
      edit->setTextSelection(len, len);
      if (mListPos > 0) mListPos--;
      return;
   }
*/
   MyGUI::UString command = sender->getCaption();
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
	const MyGUI::UString & command = sender->getCaption();
	if (command == "") return;

	MyGUI::UString key = command;
	MyGUI::UString value;

	size_t pos = command.find(' ');
	if (pos != MyGUI::UString::npos)
   {
		key = command.substr(0, pos);
		value = command.substr(pos + 1);
	}

   if (mKeptTempWord)
   {
      mUsedCommands.pop_back();
      mKeptTempWord = false;
   }
   mUsedCommands.push_back(command);
   mListPos = 0;

   executeCommand(key, value);

	sender->setCaption("");
}

void Console::executeCommand(const MyGUI::UString command, const MyGUI::UString value)
{
   MapFunction::iterator iter = mFunctions.find(command);
	if (iter != mFunctions.end())
   {
		iter->second(command, value);
	}
	else
   {
		if (eventConsoleUnknownCommand.empty())
      {
			print(mStringUnknown + "'" + command + "'");
		}
		else
      {
			eventConsoleUnknownCommand(command, value);
		}
	}
}

void Console::print(const MyGUI::UString & line)
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

bool Console::isActive()
{
   return mGUIConsole->isEnabled();
}

void Console::update()
{
   
}
