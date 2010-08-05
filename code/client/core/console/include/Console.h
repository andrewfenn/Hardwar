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

#ifndef __CONSOLE_H_
#define __CONSOLE_H_

#include <MyGUI.h>
#include <Ogre.h>
#include <limits>
#include <libintl.h>

typedef MyGUI::delegates::CDelegate2<const MyGUI::UString&, const MyGUI::UString&> CommandDelegate;

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
      Console();
      ~Console();

      /** This method loads the assets needed for the console. */
      void load();
      /** The method either shows or hides the console from the screen */
      void toggleShow();
      /** The method is used for fade effects when the console is hidden or shown */
      void update();
      /** The method returns if the console is visible on the screen */
      bool isVisible();
      /** The method returns if the console is active (has keyboard focus) */
      bool isActive();
      /** The method adds a command to the console.
      @param name
             The name of the console which is typed by the user.
      @param function
             The function which is called when the user executes the console
      command.
      */
      bool addCommand(const MyGUI::UString&, CommandDelegate::IDelegate*);
      /** The method adds text to the console's window. */
      void print(const MyGUI::UString&);
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
      void print(const MyGUI::UString& reason, const MyGUI::UString& key, const MyGUI::UString& value)
      {
         print(MyGUI::utility::toString(reason, "'", key, " ", value, "'"));
      }

      const MyGUI::UString & getConsoleCurrent() { return mStringCurrent; }
      const MyGUI::UString & getConsoleError() { return mStringError; }
      const MyGUI::UString & getConsoleSuccess() { return mStringSuccess; }
      const MyGUI::UString & getConsoleUnknow() { return mStringUnknown; }
      const MyGUI::UString & getConsoleFormat() { return mStringFormat; }
      CommandDelegate eventConsoleUnknownCommand;

      template <typename T> bool isAction(T& result, const MyGUI::UString& key, const MyGUI::UString& value, const MyGUI::UString& format = "")
      {
         if (value.empty())
         {
            addToConsole(getConsoleCurrent(), key, MyGUI::utility::toString(result));
         }
         else
         {
            if (!MyGUI::utility::parseComplex(value, result)) {
               print(getConsoleError(), key, value);
               print(getConsoleFormat(), key, format.empty() ? formats::format<T>() : format);
            }
            else
            {
               print(getConsoleSuccess(), key, value);
               return true;
            }
	      }
	      return false;
      }

      void executeCommand(const MyGUI::UString, const MyGUI::UString=MyGUI::UString(""));

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

      bool mShow;
      bool mAutoCompleted;
      bool mKeptTempWord;

      MyGUI::Gui          *mGUI;
      MyGUI::WindowPtr     mGUIConsole;
      MyGUI::ComboBoxPtr   mCommandBox;
      MyGUI::EditPtr       mHistoryList;
      MyGUI::ButtonPtr     mSubmitButton;

      MyGUI::UString mStringCurrent;
      MyGUI::UString mStringError;
      MyGUI::UString mStringSuccess;
      MyGUI::UString mStringUnknown;
      MyGUI::UString mStringFormat;

      typedef std::map<MyGUI::UString,CommandDelegate> MapFunction;
      std::vector<MyGUI::UString> mUsedCommands;
      unsigned short mListPos;
      MapFunction mFunctions;
};

}
#endif /* __Console_H_ */

