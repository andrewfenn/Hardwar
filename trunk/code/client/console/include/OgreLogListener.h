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

#ifndef __OGRELOGLISTENER_H_
#define __OGRELOGLISTENER_H_

#include "Ogre.h"
#include "Console.h"

namespace Client
{
   class OgreLogListener : public Ogre::LogListener
   {
      public:
         OgreLogListener();
         void messageLogged(const Ogre::String&, Ogre::LogMessageLevel, bool, const Ogre::String&);
      private:
         Console *mConsole;
   };
}

#endif /* __OGRELOGLISTENER_H_ */
