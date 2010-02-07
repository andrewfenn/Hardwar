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

#include "db.h"

namespace Hardwar
{
   Database::Database(const Ogre::String name)
   {
      mFilename = name;
   }

   Database::~Database()
   {

   }

   bool Database::exec(const Ogre::String sql)
   {
      mLastResult = sqlite3_exec(mSQLdb, sql.c_str(), 0, 0, 0);
      if (mLastResult != SQLITE_DONE)
      {
         std::cout << "SQLite Error: " << sqlite3_errmsg(mSQLdb) << std::endl;         
         return false;
      }
      return true;
   }

   bool Database::exec(sqlite3_stmt* statement)
   {
      mLastResult = sqlite3_step(statement);
      if (mLastResult != SQLITE_DONE)
      {
         std::cout << "SQLite Error: " << sqlite3_errmsg(mSQLdb) << std::endl;         
         return false;
      }
      return true;
   }

   sqlite3_stmt* Database::prepare(const Ogre::String sql)
   {
      sqlite3_stmt* statement;
      mLastResult = sqlite3_prepare_v2(mSQLdb, sql.c_str(), sql.size(), &statement, 0);
      if (mLastResult == SQLITE_OK)
      {
         return statement;
      }
      return 0;
   }

   bool Database::open()
   {
      mLastResult = sqlite3_open_v2(mFilename.c_str(), &mSQLdb, SQLITE_OPEN_NOMUTEX, 0);
      if(mLastResult)
      {
         /* couldn't load the file */
         std::cout << mFilename.c_str() << gettext(" could not be opened.") << std::endl;
         std::cout << "SQLite Error: " << sqlite3_errmsg(mSQLdb) << std::endl;
         sqlite3_close(mSQLdb);
         return false;
      }
      return true;
   }

   int Database::getresult()
   {
      return mLastResult;
   }
}
