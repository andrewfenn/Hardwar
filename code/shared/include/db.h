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

#ifndef __SQLDBABSTR_H_
#define __SQLDBABSTR_H_

#include <Ogre.h>
#include <sqlite3.h>
#include <libintl.h>

namespace Hardwar
{
   class Database
   {
      public:
         Database(const Ogre::String name);
         ~Database();
         
         bool exec(const Ogre::String sql);
         bool exec(sqlite3_stmt*);

         sqlite3_stmt* prepare(const Ogre::String sql);
         int getresult();
      private:
         sqlite3 *mSQLdb;
         bool mOpened;
         Ogre::String mFilename;
         int mLastResult;

         bool open();
   };
}

#endif /* __SQLDBABSTR_H_ */
