/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright © 2008-2009  Andrew Fenn
    
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

#include <OgreException.h>

#include "Server.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
char **GetArgvCommandLine(int *argc)
{
#ifdef UNICODE
    return GetCommandLineW(argc);
#else
    *argc = __argc;
    return  __argv;
#endif
}
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, int) {
	char  **argv;
    int    argc;

    // obtain command-line arguments in argv[] style array
    argv = GetArgvCommandLine(&argc);
#else
int main( int argc, char **argv ) {
#endif

    std::string cmdvar;
    int cmd = 0;

    Ogre::ConfigFile lconfig;
    lconfig.load("server.cfg");

    Ogre::String address = lconfig.getSetting("LocalAddress", "Network");
    unsigned int port = Ogre::StringConverter::parseInt(lconfig.getSetting("DefaultPort", "Network"));

    Server::ServerMain server = Server::ServerMain(lconfig);
    return 0;
}
