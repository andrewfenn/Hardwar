/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2011  Andrew Fenn
    
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

#include <OgreNode.h>

namespace Server
{
   class FlightModel : public Ogre::Node
   {
      public:

         FlightModel() : Ogre::Node()
         {
            mThrottleLimit = 100;
            mThrottle = 0;
            mMass = 100;
            mAcceleration = 3;
            mSpeed = 0;
            mSpeedLimit = 560;
            mThrust = 0;
         };

         ~FlightModel();
         void update(const unsigned long);

      private:
         int mThrottleLimit;
         int mThrottle;
         int mMass;
         int mAcceleration;
         int mSpeed;
         int mSpeedLimit;
         int mThrust;
   };
}

