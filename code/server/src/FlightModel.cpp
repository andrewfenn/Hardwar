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

#include "FlightModel.h"

#include <libintl.h>

using namespace Server;

FlightModel::~FlightModel()
{
}

void FlightModel::update(const unsigned long timeElapsed)
{
   Ogre::Vector3 translateVector = Ogre::Vector3::ZERO;

   if (mSpeed > 0)
   {
      mAcceleration = 1 + mSpeed * 0.001;
   } else {
      mAcceleration = 1 + (mSpeed*-1) * 0.001;
   }

   mThrust = ceil(mMass*(((float)mAcceleration/1000)*mThrottle));
   mSpeed += mThrust;

   if (mSpeed > mSpeedLimit)
   {
      mSpeed = mSpeedLimit;
   }

   if (mSpeed < (mSpeedLimit/2)*-1)
   {
      mSpeed = (mSpeedLimit/2)*-1;
   }

   // stupid simple drag
   mSpeed -= mSpeed*0.01;
   
   translateVector.z -= (float) mSpeed/60;
   translateVector = this->getOrientation() * translateVector;
   this->setPosition(this->getPosition()+translateVector);
}
