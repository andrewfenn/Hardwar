/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2009-2010  Andrew Fenn
    
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
    none = 0,
    has_ftp_address         = 1, /* Send the client to download somewhere else */
    download_from_server    = 2  /* Download level files from the server */
} serverDldMethod;

typedef enum {
    status_disconnected = 0,
    status_connecting   = 1, /* Joined server */
    status_listening    = 2, /* Waiting for server response */
    status_filecheck    = 3, /* Checking level files and loading */
    status_downloading  = 4, /* Downloading building data, etc */
    status_lobby_name   = 5, /* Server login screen */
    status_lobby_class  = 6, /* picking character class */
    status_ingame       = 7  /* playing in the game */
} clientStatus;

typedef enum
{
   movement       = 0,
   accepted       = 1, /* Response to some command */
   rejected       = 2, /* Response to some command */
   join_game      = 3, /* Request to join game */
   status_changed = 4, /* Changed client status */

   get_building_list = 50, /* get all buildings in the client's list */
   add_building   = 51, /* Attempt to add a building */
   edit_building  = 52,
   delete_building= 53,

   save_world_data = 60, /* Save server side world related data */

   admin_login    = 100  /* Attempt to login as admin */
} packetMessage;

#endif /* __SRVSTRUCTS_H_ */
