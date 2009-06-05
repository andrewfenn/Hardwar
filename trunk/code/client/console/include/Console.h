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

#include "ConsoleCombo.h"
#include <MyGUI.h>
#include <libintl.h>

typedef MyGUI::delegates::CDelegate2<const Ogre::UTFString&, const Ogre::UTFString&> CommandDelegate;

namespace formats
{
   template<typename T> inline std::string format() { return MyGUI::utility::toString("[ ", std::numeric_limits<T>::min(), " | ", std::numeric_limits<T>::max(), " ]"); }
   template<> inline std::string format<bool>() { return "[ true | false ]"; }
   template<> inline std::string format<float>() { return MyGUI::utility::toString("[ ", -std::numeric_limits<float>::max(), " | ", std::numeric_limits<float>::max(), " ]"); }
   template<> inline std::string format<double>() { return MyGUI::utility::toString("[ ", -std::numeric_limits<double>::max(), " | ", std::numeric_limits<double>::max(), " ]"); }
}

namespace Client
{
/** The Game Console
        @remarks
            The Client::Console class deals with all the functionality required
            to start and run the player's console. It depends upon MyGUI so must
            be initialised afterward. 
    */
class Console
{
   public:
      ~Console();
      static Console* getSingletonPtr(void);
      /** The method either shows or hides the console from the screen */
      void toggleShow();
      /** The method is used for fade effects when the console is hidden or shown */
      void update();
      /** The method returns if the console is visible on the screen */
      bool isVisible();
      /** The method adds a command to the console.
      @param name
             The name of the console which is typed by the user.
      @param function
             The function which is called when the user executes the console
      command.
      */
      bool addCommand(const Ogre::UTFString&, CommandDelegate::IDelegate*);
      /** The method adds text to the console's window. */
      void addToConsole(const Ogre::UTFString&);
      /** The method adds formatted text to the console's window according to
      the UserString's in bin/media/core/console/console.layout
      @param reason
             The UserString for example getConsoleError()
      @param key
             The piece of text which describes value, for example the function
      name which was executed.
      @param value
             The result text displayed after the key text.
      */
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
               addToConsole(getConsoleFormat(), key, format.empty() ? formats::format<T>() : format);
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
      /** This method is called whenever a character is typed into the console's
      text box.
      @param widget
             A pointer to the textbox widget
      @param keycode
             The character typed into the textbox by keycode
      @param char
             The character typed into the textbox
      */
      void notifyCommandTyped(MyGUI::WidgetPtr, MyGUI::KeyCode, MyGUI::Char);
      void notifyCommandAccept(MyGUI::ComboBoxPtr, size_t);
      void notifySubmitButtonClick(MyGUI::WidgetPtr);

      static Console *mConsole;

      Console(void);
      Console(const Console&) { }
      Console & operator = (const Console&);

      bool mShow;
      bool mAutoCompleted;
      bool mKeptTempWord;

      MyGUI::Gui          *mGUI;
      MyGUI::WindowPtr     mGUIConsole;
      MyGUI::ConsoleComboBoxPtr   mCommandBox;
      MyGUI::EditPtr       mHistoryList;
      MyGUI::ButtonPtr     mSubmitButton;

      Ogre::UTFString mStringCurrent;
		Ogre::UTFString mStringError;
		Ogre::UTFString mStringSuccess;
		Ogre::UTFString mStringUnknown;
		Ogre::UTFString mStringFormat;

      typedef std::map<Ogre::UTFString,CommandDelegate> MapFunction;
      std::vector<Ogre::UTFString> mUsedCommands;
      unsigned short mListPos;
		MapFunction mFunctions;
};

}
#endif /* __Console_H_ */

