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

#ifndef __CONSOLE_H_
#define __CONSOLE_H_

#include <MyGUI.h>

typedef MyGUI::delegates::CDelegate2<const Ogre::UTFString&, const Ogre::UTFString&> CommandDelegate;

namespace formates
{
   template<typename T> inline std::string format() { return MyGUI::utility::toString("[ ", std::numeric_limits<T>::min(), " | ", std::numeric_limits<T>::max(), " ]"); }
   template<> inline std::string format<bool>() { return "[ true | false ]"; }
   template<> inline std::string format<float>() { return MyGUI::utility::toString("[ ", -std::numeric_limits<float>::max(), " | ", std::numeric_limits<float>::max(), " ]"); }
   template<> inline std::string format<double>() { return MyGUI::utility::toString("[ ", -std::numeric_limits<double>::max(), " | ", std::numeric_limits<double>::max(), " ]"); }
}

class Console
{
   public:
      Console();
      ~Console();
      void toggleShow();
      void update();
      bool isVisible();
      bool addCommand(const Ogre::UTFString&, CommandDelegate::IDelegate*);
      void addToConsole(const Ogre::UTFString&);
      void addToConsole(const Ogre::UTFString& reason, const Ogre::UTFString& key, const Ogre::UTFString& value)
      {
         addToConsole(MyGUI::utility::toString(reason, "'", key, " ", value, "'"));
      }

      const Ogre::UTFString & getConsoleCurrent() { return mStringCurrent; }
      const Ogre::UTFString & getConsoleError() { return mStringError; }
      const Ogre::UTFString & getConsoleSuccess() { return mStringSuccess; }
      const Ogre::UTFString & getConsoleUnknow() { return mStringUnknown; }
      const Ogre::UTFString & getConsoleFormat() { return mStringFormat; }
      CommandDelegate eventConsoleUnknownCommand;

      template <typename T> bool isAction(T& result, const Ogre::UTFString& key, const Ogre::UTFString& value, const Ogre::UTFString& format = "")
      {
         if (value.empty())
         {
            addToConsole(getConsoleCurrent(), key, MyGUI::utility::toString(result));
         }
         else
         {
            if (!MyGUI::utility::parseComplex(value, result)) {
               addToConsole(getConsoleError(), key, value);
               addToConsole(getConsoleFormat(), key, format.empty() ? formates::format<T>() : format);
            }
            else
            {
               addToConsole(getConsoleSuccess(), key, value);
               return true;
            }
	      }
	      return false;
      }

   private:
      void draw();
      void notifyCommandTyped(MyGUI::WidgetPtr, MyGUI::KeyCode, MyGUI::Char);
      void notifyCommandAccept(MyGUI::ComboBoxPtr, size_t);
      void notifySubmitButtonClick(MyGUI::WidgetPtr);

      bool mShow;
      bool mAutoCompleted;

      MyGUI::Gui  *mGUI;
      MyGUI::WindowPtr mGUIConsole;
      MyGUI::ComboBoxPtr   mCommandBox;
      MyGUI::EditPtr       mHistoryList;
      MyGUI::ButtonPtr     mSubmitButton;

      Ogre::UTFString mStringCurrent;
		Ogre::UTFString mStringError;
		Ogre::UTFString mStringSuccess;
		Ogre::UTFString mStringUnknown;
		Ogre::UTFString mStringFormat;

      float mAlpha;
      typedef std::map<Ogre::UTFString,CommandDelegate> MapFunction;
      std::vector<Ogre::UTFString> mUsedCommands;
      signed short mListPos;
		MapFunction mFunctions;
};
#endif /* __Console_H_ */

