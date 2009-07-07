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


#ifndef __SRVSTRUCTS_H_
#define __SRVSTRUCTS_H_

/** How many channels we have 
   @remarks
      This is needed when it comes to opening Enet connections so if more
      channels are added update the number here.
*/
#define SERVER_MAX_CHANNELS      3

#define SERVER_CHANNEL_GENERIC   0
#define SERVER_CHANNEL_MOVEMENT  1
#define SERVER_CHANNEL_ADMIN     2

typedef enum {
    NONE = 0,
    HAS_FTP_ADDRESS         = 0x0001, /* Send the client to download somewhere else */
    DOWNLOAD_FROM_SERVER    = 0x0002  /* Download level files from the server */
} serverDldMethod;

typedef enum {
    STATUS_DISCONNECTED = 0,
    STATUS_CONNECTING   = 0x0001, /* Joined server */
    STATUS_LISTENING    = 0x0002, /* Waiting for server response */
    STATUS_FILECHECK    = 0x0003, /* Checking level files and loading */
    STATUS_DOWNLOADING  = 0x0004, /* Downloading building data, etc */
    STATUS_LOBBY_NAME   = 0x0005, /* Server login screen */
    STATUS_LOBBY_CLASS  = 0x0006, /* picking character class */
    STATUS_INGAME       = 0x0007  /* playing in the game */
} clientStatus;

#endif /* __SRVSTRUCTS_H_ */
