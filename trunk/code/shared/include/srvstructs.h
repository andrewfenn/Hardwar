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

typedef enum {
    NONE = 0,
    HAS_FTP_ADDRESS         = 0x0001,/* Send the client to download somewhere else */
    DOWNLOAD_FROM_SERVER    = 0x0002 /* Download level files from the server */
} serverDldMethod;

typedef enum {
    STATUS_DISCONNECTED = 0,
    STATUS_CONNECTING   = 0x0001, /* Joined server */
    STATUS_LISTENING    = 0x0002, /* Waiting for server response */
    STATUS_CONNECTED    = 0x0003, /* Connected */
    STATUS_FILECHECK    = 0x0004, /* Checking level files */
    STATUS_LOBBY_NAME   = 0x0005, /* Server login screen */
    STATUS_LOBBY_CLASS  = 0x0006, /* picking character class */
    STATUS_INGAME       = 0x0007  /* playing in the game */
} clientStatus;

#endif /* __SRVSTRUCTS_H_ */
