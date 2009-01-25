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


#ifndef Console_H
#define Console_H

#include <OgreFrameListener.h>
#include <Ogre.h>
#include <OIS/OIS.h>
#include <list>
#include <vector>

using namespace Ogre;
using namespace std;

class GUIConsole: FrameListener, LogListener
{
public:
   GUIConsole();
   ~GUIConsole();

   void   init(Ogre::Root *root);
   void   shutdown();

   void   setVisible(bool visible);
   bool   isVisible(){ return mVisible; }

   void   print(const String &text);

   virtual bool frameStarted(const Ogre::FrameEvent &evt);
   virtual bool frameEnded(const Ogre::FrameEvent &evt);

   void onKeyPressed(const OIS::KeyEvent &arg);

   void addCommand(const String &command, void (*)(vector<String>&));
   void removeCommand(const String &command);

   //log
   void messageLogged( const String& message, LogMessageLevel lml, bool maskDebug, const String &logName ) {print(logName+": "+message);}
private:
   bool             mVisible;
   bool             mInitialized;
   Root             *mRoot;
   SceneManager     *mScene;
   Rectangle2D      *mRect;
   SceneNode        *mNode;
   OverlayElement   *mTextbox;
   Overlay          *mOverlay;

   float            mHeight;
   bool             mUpdateOverlay;
   bool             mNotShown;
   unsigned int     mStartLine;
   list<String>     mLines;
   String           mPrompt;
   String           mTempPrompt;
   bool             mIsBlink;
   Real             mTimeBlink;
   unsigned int     mCursorPos;
   unsigned int       mHistoryPos;
   map<String,void (*)(vector<String>&)>  mCommands;
   vector<String>   mPromptHistory;
};

#endif // Console_H

