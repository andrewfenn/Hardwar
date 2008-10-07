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

//GUIConsole *Singleton<GUIConsole>::ms_Singleton=0;

#define CONSOLE_LINE_LENGTH 150
#define CONSOLE_LINE_COUNT 24

/* defines how many commands should be stored in history */
#define CONSOLE_HISTORY_LENGTH 5

GUIConsole::GUIConsole(){
   mStartLine=0;
   mCursorPos=0;
}

GUIConsole::~GUIConsole(){
   
}

void GUIConsole::init(Ogre::Root *root){
    if(!root->getSceneManagerIterator().hasMoreElements())
        OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "No scene manager found!", "init" );

    mRoot=root;
    mScene=mRoot->getSceneManagerIterator().getNext();
    mRoot->addFrameListener(this);

    mHeight=1;

    // Create background rectangle covering the whole screen
    mRect = new Rectangle2D(true);
    mRect->setCorners(-1, 1, 1, 1-mHeight);
    mRect->setMaterial("console/background");
    mRect->setRenderQueueGroup(RENDER_QUEUE_OVERLAY);
    mRect->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE));
    mNode = mScene->getRootSceneNode()->createChildSceneNode("#Console");
    mNode->attachObject(mRect);

    mTextbox=OverlayManager::getSingleton().createOverlayElement("TextArea","ConsoleText");
    mTextbox->setMetricsMode(GMM_RELATIVE);
    mTextbox->setPosition(0,0);
    mTextbox->setParameter("font_name","Console");
    mTextbox->setParameter("colour_top","1 1 1");
    mTextbox->setParameter("colour_bottom","1 1 1");
    mTextbox->setParameter("char_height","0.02");
   
    mOverlay=OverlayManager::getSingleton().create("Console");   
    mOverlay->add2D((OverlayContainer*)mTextbox);
    mNotShown = true;
    mVisible = false;
    LogManager::getSingleton().getDefaultLog()->addListener(this);
}

void GUIConsole::shutdown(){
   if(!mInitialized)
      return;
   delete mRect;
   delete mNode;
   delete mTextbox;
   delete mOverlay;
}

void GUIConsole::onKeyPressed(const OIS::KeyEvent &e){
   if(!mVisible)
      return;
   if (e.key == OIS::KC_RETURN)
   {
      //split the parameter list
      const char *str=mPrompt.c_str();
      vector<String> params;
      String param="";
      for(int c=0;c<mPrompt.length();c++){
         if(str[c]==' '){
            if(param.length())
               params.push_back(param);
            param="";
         }
         else
            param+=str[c];
      }
      if(param.length())
         params.push_back(param);

      //try to execute the command
      map<String,void(*)(vector<String>&)>::iterator i;
      for(i=mCommands.begin();i!=mCommands.end();i++){
         if((*i).first==params[0]){
            if((*i).second)
               (*i).second(params);
            break;
         }
      }

      /* delete old commands from history */
      if (mPromptHistory.size() > CONSOLE_HISTORY_LENGTH) {
            mPromptHistory.erase(mPromptHistory.begin());
      }

      /* add the new one */
      mPromptHistory.push_back(mPrompt);

      mPrompt="";
      mCursorPos=0;
      mHistoryPos=-1;
   }

   if (e.key == OIS::KC_BACK) {
      mPrompt = mPrompt.substr(0,mPrompt.length()-mCursorPos-1) + mPrompt.substr(mPrompt.length()-mCursorPos, mPrompt.length());
   }

   if (e.key == OIS::KC_PGUP)
   {
      if(mStartLine>0)
         mStartLine--;
   }

   if (e.key == OIS::KC_PGDOWN)
   {
      if(mStartLine<mLines.size())
         mStartLine++;
   }

   if (e.key == OIS::KC_LEFT) {
        /* Move cursor left */
        if (mPrompt.length() > mCursorPos) {
            mCursorPos++;
        }
   }

   if (e.key == OIS::KC_RIGHT) {
        /* Move cursor right */
        if (mCursorPos > 0) {
            mCursorPos--;
        }
   }

   if (e.key == OIS::KC_UP) {
        if (mHistoryPos == -1) {
            /* save the old value */
            mTempPrompt = mPrompt;
        }

        if (mHistoryPos+1 < mPromptHistory.size()) {
            mHistoryPos++;
            mPrompt = mPromptHistory.at(mPromptHistory.size()-1-mHistoryPos);
            mCursorPos = 0;
        }
   }

   if (e.key == OIS::KC_DOWN) {
        if (mHistoryPos > -1) {
            mHistoryPos--;
            if (mHistoryPos == -1) {
                /* Put old value back */
                mPrompt = mTempPrompt;
            } else {
                mPrompt = mPromptHistory.at(mPromptHistory.size()-1-mHistoryPos);
                mCursorPos = 0;
            }
        }
   }

   if (e.key == OIS::KC_LSHIFT || e.key == OIS::KC_RSHIFT || e.key > OIS::KC_CAPITAL || e.key == OIS::KC_LMENU) {
        /* Do nothing, otherwise it adds tabs into the console */
   } else {

      char legalchars[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/()=?[]\\*-_.:,; ";
      for(int c=0;c<sizeof(legalchars);c++){
         if(legalchars[c]==e.text){
            mPrompt = mPrompt.substr(0,mPrompt.length()-mCursorPos) + e.text + mPrompt.substr(mPrompt.length()-mCursorPos, mPrompt.length());
            break;
         }
      }
   }
   mUpdateOverlay=true;
}
bool GUIConsole::frameStarted(const Ogre::FrameEvent &evt){
   if(mVisible&&mHeight<1){
      mHeight+=evt.timeSinceLastFrame*2;
      mTextbox->show();
      if(mHeight>=1){
         mHeight=1;
      }
   }
   else if(!mVisible&&mHeight>0){
      mHeight-=evt.timeSinceLastFrame*2;
      if(mHeight<=0){
         mHeight=0;
         mTextbox->hide();
      }
   }

   mTextbox->setPosition(0,(mHeight-1)*0.5);
   mRect->setCorners(-1,1+mHeight,1,1-mHeight);

   if(mUpdateOverlay){
      String text;
      list<String>::iterator i,start,end;
      
      //make sure is in range
      if(mStartLine>mLines.size())
         mStartLine=mLines.size();

      int lcount=0;
      start=mLines.begin();
      for(int c=0;c<mStartLine;c++)
         start++;
      end=start;
      for(int c=0;c<CONSOLE_LINE_COUNT;c++){
         if(end==mLines.end())
            break;
         end++;
      }
      for(i=start;i!=end;i++)
         text+=(*i)+"\n";
      
      //add the prompt
      text += "> " + mPrompt.substr(0,mPrompt.length()-mCursorPos);
    
      // make the cursor blink
      if (mIsBlink || mCursorPos > 0) {
        text+= "|";
      }

      text += mPrompt.substr(mPrompt.length()-mCursorPos, mPrompt.length());

      mTextbox->setCaption(text);
      mUpdateOverlay=false;
   }

   if (mTimeBlink > 0.5 && mIsBlink) {
       mUpdateOverlay=true;
       mIsBlink = false;
       mTimeBlink = 0;
   } else {
       if (mTimeBlink > 0.5 && !mIsBlink) {
           mIsBlink = true;
           mUpdateOverlay=true;
           mTimeBlink = 0;
       }
   }
   mTimeBlink += evt.timeSinceLastFrame;
   

   return true;
}

void GUIConsole::print(const String &text){
   //subdivide it into lines
   const char *str=text.c_str();
   int start=0,count=0;
   int len=text.length();
   String line;
   for(int c=0;c<len;c++){
      if(str[c]=='\n'||line.length()>=CONSOLE_LINE_LENGTH){
         mLines.push_back(line);
         line="";
      }
      if(str[c]!='\n')
         line+=str[c];
   }
   if(line.length())
      mLines.push_back(line);
   if(mLines.size()>CONSOLE_LINE_COUNT)
      mStartLine=mLines.size()-CONSOLE_LINE_COUNT;
   else
      mStartLine=0;
   mUpdateOverlay=true;
}

bool GUIConsole::frameEnded(const Ogre::FrameEvent &evt){

   return true;
}

void GUIConsole::setVisible(bool visible){
    if (visible && mNotShown) {
        mOverlay->show();
        mNotShown = false;
    }
    mVisible=visible;
}

void GUIConsole::addCommand(const String &command, void (*func)(vector<String>&)){
   mCommands[command]=func;
}

void GUIConsole::removeCommand(const String &command){
   mCommands.erase(mCommands.find(command));
}

