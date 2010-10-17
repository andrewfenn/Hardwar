/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2010  Andrew Fenn
    
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

#pragma once

namespace Client
{

class GameTaskList;

class GameTask
{
   public:
       virtual void init();
       virtual void shutdown();
       virtual void update();

       virtual void changeSize(Ogre::RenderWindow*);
       virtual void changeFocus(Ogre::RenderWindow*);

       void setTaskList(GameTaskList* gametasks)
       {
         mTaskList = gametasks;
       }
   protected:
      GameTaskList* mTaskList;
};

typedef std::map<Ogre::String, GameTask*> TaskList;
class GameTaskList
{
   public:
      GameTask* add(const Ogre::String& name, GameTask* task)
      {
         if (mList.find(name) != mList.end())
         {
            OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
               "A task with the name " + name + " already exists",
               "GameTaskList::add" );
         }
         task->setTaskList(this);
         mList.insert(TaskList::value_type(name, task));
         return task;
      }

      GameTask* get(const Ogre::String& name)
      {
         TaskList::const_iterator i = mList.find(name);
         if (i != mList.end())
         {
            return (GameTask*) &i->second;
         }
         OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
            "Cannot find Task with name " + name,
            "GameTaskList::get");
      }

      bool has(const Ogre::String& name)
      {
         return (mList.find(name) != mList.end());
      }

      void remove(const Ogre::String& name)
      {
         TaskList::iterator i = mList.find(name);
         if (i != mList.end())
         {
            OGRE_DELETE &i->second;
            mList.erase(i);
         }
      }

      void removeAll()
      {
         for(TaskList::iterator i = mList.begin(); i != mList.end(); i++)
         {
            OGRE_DELETE &i->second;
            mList.erase(i);
         }
      }

      TaskList* list()
      {
         return &mList;
      }
   private:
      TaskList mList;
};

}

