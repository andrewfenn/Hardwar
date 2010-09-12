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
class GameTask;
typedef std::map<Ogre::String, GameTask*> GameTaskList;

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
}

