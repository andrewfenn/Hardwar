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
    HAS_FTP_ADDRESS         = 0x0001,
    DOWNLOAD_FROM_SERVER    = 0x0002
} serverDldMethod;

typedef enum {
    STATUS_CONNECTED    = 0,
    STATUS_FILECHECK    = 0x0001,
    STATUS_LOBBY        = 0x0002,
    STATUS_INGAME       = 0x0003
} clientStatus;

#endif /* __SRVSTRUCTS_H_ */
