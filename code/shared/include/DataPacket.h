/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2009  Andrew Fenn
    
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

#ifndef __DATAPACKET_H_
#define __DATAPACKET_H_

#include "srvstructs.h"

class dataPacket
{
   public:
      dataPacket() { }

      dataPacket(const packetMessage header)
      {
         append(&header, sizeof(packetMessage));
      }

      dataPacket(void* data, unsigned int len)
      {
         mBuffer.clear();
         append(data, len);

         unsigned char* msgData = 0;
         for(unsigned int i=0; i < sizeof(packetMessage); i++)
         {
            msgData += mBuffer.at(i);
         }

         std::memcpy(&mMessage, &msgData, sizeof(packetMessage));
         mBuffer.erase(mBuffer.begin(), mBuffer.begin()+sizeof(packetMessage));
      }

     	void append(const void* mem, int len)
	   {
		   int i;
		   mBuffer.reserve(mBuffer.size() + len);
		   for(i = 0 ; i < len ; i++)
         {
			   mBuffer.push_back(((unsigned char*) mem)[i]);
         }
	   }

      packetMessage getMessage(void)
      {
         return mMessage;
      }

      unsigned char* getContents(void)
      {
         return &mBuffer[0];
      }

      size_t size(void)
      {
         return mBuffer.size();
      }

   private:
      packetMessage mMessage;
     	std::vector<unsigned char> mBuffer;
};

#endif /* __DATAPACKET_H_ */
