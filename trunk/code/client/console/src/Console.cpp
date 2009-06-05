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

using namespace Client;

Console* Console::mConsole;

Console::Console()
{
   mShow = false;
   mKeptTempWord = false;
   mListPos = 0;
   MyGUI::WidgetManager::getInstancePtr()->registerFactory(new MyGUI::factory::ConsoleComboBoxFactory());
   mGUI = MyGUI::Gui::getInstancePtr();
   MyGUI::LayoutManager::getInstance().load("console.layout");

   mGUIConsole = mGUI->findWidget<MyGUI::Window>("console");
   mCommandBox = mGUI->findWidget<MyGUI::ConsoleComboBox>("commandbox");
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

   mSubmitButton->setCaption(Ogre::UTFString(gettext("submit")));
   mGUIConsole->setVisible(false);
}

Console::~Console()
{}

bool Console::addCommand(const Ogre::UTFString &name, CommandDelegate::IDelegate* function)
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
      MyGUI::ControllerFadeAlpha * controller = new MyGUI::ControllerFadeAlpha(0, 100, true);
      MyGUI::ControllerManager::getInstance().addItem(mGUIConsole, controller);
      mGUI->hidePointer();
   }
   else
   {
      mShow = true;
      MyGUI::ControllerFadeAlpha * controller = new MyGUI::ControllerFadeAlpha(1, 100, true);
      MyGUI::ControllerManager::getInstance().addItem(mGUIConsole, controller);
      mGUI->showPointer();
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

   MyGUI::ConsoleComboBoxPtr combo = sender->castType<MyGUI::ConsoleComboBox>();
   if (len > 0)
   {

   }
   
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

   if (mKeptTempWord)
   {
      mUsedCommands.pop_back();
      mKeptTempWord = false;
   }
   mUsedCommands.push_back(command);
   mListPos = 0;

	MapFunction::iterator iter = mFunctions.find(key);
	if (iter != mFunctions.end())
   {
		iter->second(key, value);
	}
	else
   {
		if (eventConsoleUnknownCommand.empty())
      {
			addToConsole(mStringUnknown + "'" + key + "'");
		}
		else
      {
			eventConsoleUnknownCommand(key, value);
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
   
}

Console* Console::getSingletonPtr(void)
{
   if(!mConsole)
   {
      mConsole = new Console();
   }

   return mConsole;
}
