/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2008-2011  Andrew Fenn
    
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

#include "ConsoleState.h"

#include <libintl.h>
#include <limits>
#include <Ogre.h>

#include "InputTask.h"
#include "GuiTask.h"

using namespace Client;

ConsoleState::ConsoleState(): GameState("Console")
{
   mShow = false;
   mKeptTempWord = false;
   mListPos = 0;
}

void ConsoleState::enter()
{
   // register input listener for console
   InputTask* input = (InputTask*) mTasklist->get("Input");
   input->addKeyListener(this, this->getName());
   input->addMouseListener(this, this->getName());

   mGUI = MyGUI::Gui::getInstancePtr();

   MyGUI::LayoutManager::getInstance().load("console.layout");

   mWindow = mGUI->findWidget<MyGUI::Window>("console");
   mCommandBox = mGUI->findWidget<MyGUI::ComboBox>("commandbox");
   mHistoryList = mGUI->findWidget<MyGUI::Edit>("historylist");
   mHistoryList->setOverflowToTheLeft(true);
   mSubmitButton = mGUI->findWidget<MyGUI::Button>("submit");

   mStringCurrent = mWindow->getUserString("Current");
   mStringError   = mWindow->getUserString("Error");
   mStringSuccess = mWindow->getUserString("Success");
   mStringUnknown = mWindow->getUserString("Unknown");
   mStringFormat  = mWindow->getUserString("Format");

   mCommandBox->eventComboAccept = newDelegate(this, &ConsoleState::notifyCommandAccept);
   mCommandBox->eventKeyButtonPressed = newDelegate(this, &ConsoleState::notifyCommandTyped);
   mSubmitButton->eventMouseButtonClick = newDelegate(this, &ConsoleState::notifySubmitButtonClick);

   mSubmitButton->setCaption(MyGUI::UString(gettext("submit")));
   mWindow->setVisible(false);
   mWindow->setEnabled(false);
}

ConsoleState::~ConsoleState()
{
   InputTask* input = (InputTask*) mTasklist->get("Input");
   input->removeKeyListener(this->getName());
   input->removeMouseListener(this->getName());
}

bool ConsoleState::addCommand(const MyGUI::UString &name, CommandDelegate::IDelegate* function)
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

void ConsoleState::setVisible(const bool visible)
{
   mShow = visible;
   mGUI->setVisiblePointer(mShow);
   mWindow->setVisible(mShow);
   mWindow->setEnabled(mShow);

   InputTask* input = (InputTask*) mTasklist->get("Input");
   if (mShow)
   {
       input->disableAll();
       input->enable(this->getName());
       MyGUI::InputManager::getInstance().setKeyFocusWidget(mCommandBox);
   }
   else
   {
      input->enableAll();
   }
}

void ConsoleState::notifySubmitButtonClick(MyGUI::WidgetPtr sender)
{
   notifyCommandAccept(mCommandBox, MyGUI::ITEM_NONE);
}

void ConsoleState::notifyCommandTyped(MyGUI::WidgetPtr sender, MyGUI::KeyCode key, MyGUI::Char character)
{
    return;
   size_t len; // = sender->getCaption().length();
 
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
   MyGUI::UString command; // = sender->getCaption();
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

void ConsoleState::notifyCommandAccept(MyGUI::ComboBoxPtr sender, size_t index)
{
    return;
	const MyGUI::UString command;// = sender->getCaption();
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

void ConsoleState::executeCommand(const MyGUI::UString command, const MyGUI::UString value)
{
   MapFunction::iterator iter = mFunctions.find(command);
	if (iter != mFunctions.end())
   {
		iter->second(command, value);
	}
	else
   {
		if (eventUnknownCommand.empty())
      {
			print(mStringUnknown + "'" + command + "'");
		}
		else
      {
			eventUnknownCommand(command, value);
		}
	}
}

void ConsoleState::print(const MyGUI::UString & line)
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

bool ConsoleState::isVisible()
{
   return mShow;
}

bool ConsoleState::isActive()
{
   return mWindow->isEnabled();
}

void ConsoleState::update(unsigned long timeElapsed)
{
   
}

bool ConsoleState::keyPressed(const OIS::KeyEvent &e)
{
   if (e.key != OIS::KC_GRAVE)
   {
      if (this->isVisible())
      {
         ((GuiTask*) mTasklist->get("Gui"))->resource()->injectKeyPress(MyGUI::KeyCode::Enum(e.key), e.text);
      }
   }
   return true;
}

bool ConsoleState::keyReleased(const OIS::KeyEvent &e)
{
   if (e.key == OIS::KC_GRAVE)
   {
      this->setVisible(!this->isVisible());
   }
   else if (this->isVisible())
   {
      ((GuiTask*) mTasklist->get("Gui"))->resource()->injectKeyRelease(MyGUI::KeyCode::Enum(e.key));
   }
   return true;
}

bool ConsoleState::mouseMoved(const OIS::MouseEvent &e)
{
   if (this->isVisible())
   {
      ((GuiTask*) mTasklist->get("Gui"))->resource()->injectMouseMove(e.state.X.abs, e.state.Y.abs, e.state.Z.abs);
   }
   return true;
}

bool ConsoleState::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   if (this->isVisible())
   {
      ((GuiTask*) mTasklist->get("Gui"))->resource()->injectMousePress(e.state.X.abs, e.state.Y.abs, MyGUI::MouseButton::Enum(id));
   }
   return true;
}

bool ConsoleState::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   if (this->isVisible())
   {
      ((GuiTask*) mTasklist->get("Gui"))->resource()->injectMouseRelease(e.state.X.abs, e.state.Y.abs, MyGUI::MouseButton::Enum(id));
   }
   return true;
}
