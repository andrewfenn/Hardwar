/* 
    This file is part of Hardwar - A remake of the classic flight sim shooter
    Copyright (C) 2008  Andrew Fenn
    
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

#include "GameManager.h"
#include "PlayState.h"

#include "Server.h"
#include "Client.h"

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
   
   bool serverStarted = false;
   for (int i=1; i < argc; i++) {
        if (std::string(argv[i]).compare("--dedicated") == 0) {
          // start a server
          serverStarted = true; 
          Server server;
        }
        if (std::string(argv[i]).compare("--clienttest") == 0) {
          // start a server
          serverStarted = true; 
          Client client;
        }
   }
   
   if (!serverStarted) {
       // If a dedicated server hasn't started then load up the client
          
       GameManager *gameManager = GameManager::getSingletonPtr();

       try {
           // Initialise the game and switch to the first state
           gameManager->startGame( PlayState::getSingletonPtr() );
       }
       catch ( Ogre::Exception& ex ) {
           #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
                   MessageBox( NULL, ex.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL );
           #else
                   std::cerr << "An exception has occured: " << ex.getFullDescription();
           #endif
       }

      delete gameManager;
   }
    
    return 0;
}
