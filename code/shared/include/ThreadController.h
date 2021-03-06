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


#ifndef __THREAD_CONTROL_H_
#define __THREAD_CONTROL_H_

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

class ThreadController
{
   public:
      ThreadController (void) : mMutex(), mStop(false) {}

      void stop (void)
      {
        boost::mutex::scoped_lock lock(mMutex);
        mStop = true;
      }

      bool hasStopped (void) const
      {
        boost::mutex::scoped_lock lock(mMutex);
        return mStop;
      }

   private:
      mutable boost::mutex mMutex;
      bool mStop;
};

#endif /*__THREAD_CONTROL_H_ */
