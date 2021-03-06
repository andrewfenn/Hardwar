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

#include <Ogre.h>

namespace Client
{

class GameTaskList;

/** Game Task
        @remarks
            The Client::GameTask class is a representation for a piece of functionality
        that the game needs to run every update. This could be music, networkng or input
    */
class GameTask
{
    public:

        GameTask()
        {
            mTaskList = nullptr;
        }

        virtual ~GameTask() { };

        virtual void init() = 0;
        virtual void shutdown() = 0;
        virtual void update() = 0;

        virtual void changeSize(Ogre::RenderWindow*) { }
        virtual void changeFocus(Ogre::RenderWindow*) { }

        void setTaskList(GameTaskList* gametasks)
        {
            mTaskList = gametasks;
        }

        GameTaskList* getTaskList()
        {
            return mTaskList;
        }
    protected:
        GameTaskList* mTaskList;
};

typedef std::map<Ogre::String, GameTask*> TaskList;
class GameTaskList
{
    public:
        void* add(const Ogre::String& name, GameTask* task)
        {
            if (mList.find(name) != mList.end())
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
                "A task with the name " + name + " already exists",
                "GameTaskList::add" );
            }

            mList.insert(TaskList::value_type(name, task));
            task->setTaskList(this);
            task->init();
            return task;
        }

        void* get(const Ogre::String& name)
        {
            TaskList::const_iterator i = mList.find(name);
            if (i != mList.end())
            {
               return i->second;
            }

            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
            "Cannot find Task with name " + name,
            "GameTaskList::get");
        }

        bool has(const Ogre::String& name)
        {
            return (mList.find(name) != mList.end());
        }

        bool remove(const Ogre::String& name)
        {
            TaskList::iterator i = mList.find(name);
            if (i != mList.end())
            {
                i->second->shutdown();
                OGRE_DELETE i->second;
                mList.erase(i);
                return true;
            }
            return false;
        }

        void removeAll()
        {
            for(TaskList::iterator i = mList.begin(); i != mList.end(); i++)
            {
                i->second->shutdown();
                OGRE_DELETE i->second;
            }
            mList.clear();
        }

        TaskList* list()
        {
            return &mList;
        }
    private:
        TaskList mList;
};

}

