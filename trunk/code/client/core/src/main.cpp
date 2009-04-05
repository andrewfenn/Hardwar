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
#include "LoadState.h"
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
    lconfig.load("settings.cfg");

    Ogre::String address = lconfig.getSetting("local address", "Network Settings");
    unsigned int port = Ogre::StringConverter::parseInt(lconfig.getSetting("default port", "Network Settings"));

    /* FIXME: Horrible, use regular expression */
    for (int i=1; i < argc; i++)
    {
        cmdvar = std::string(argv[i]);
        if (cmdvar.substr(0, 2).compare("--") == 0)
        {
            cmdvar = cmdvar.substr(2, cmdvar.length()-2);

            if (cmdvar.compare("help") == 0)
            {
                printf(gettext("usage: hardwar [--connect[-ip|-port]] [--server[port]]\n\n"));
                printf(gettext("The full list of commands are:\n"));
                printf(gettext("\tconnect\t\tConnects to server running the same version.\n"));
                printf(gettext("\tserver\t\tStarts up a dedicated server running the game.\n"));
                printf(gettext("\thelp\t\tDisplays this help text.\n"));
            }
            else if (cmdvar.compare("connect") == 0)
            {
                cmd = 1;   
            }
            else if (cmdvar.compare("server") == 0)
            {
                cmd = 2;
            }
        } else {
            switch (cmd)
            {
                default:
                break;
                case 1: /* connect */
                case 2: /* server */
                    if (cmdvar.compare("-ip") == 0)
                    {
                        if (i+1 < argc)
                        {
                            i++;
                            address = std::string(argv[i]);
                        }
                        else
                        {
                            printf("Invalid commandline arguments\n");
                        }
                    }
                    else if (cmdvar.compare("-port") == 0)
                    {
                        if (i+1 < argc)
                        {
                            i++;
                            port = atoi(argv[i]);
                        }
                        else
                        {
                            printf("Invalid commandline arguments\n");
                        }
                    }
                break;
            }
        }
    }

    switch(cmd)
    {
        case 1: /* connect */
        {
            GameManager *gameManager = GameManager::getSingletonPtr();
            gameManager->mNetwork->setPort(port);
            gameManager->mNetwork->setAddress(address);
            gameManager->mSinglePlayer = false;

            try
            {                /* Initialise the game and switch to the first state */
                gameManager->startGame(LoadState::getSingletonPtr());
            }
            catch ( Ogre::Exception& ex )
            {
                #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
                    MessageBox( NULL, ex.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL );
                #else
                    std::cerr << "An exception has occured: " << ex.getFullDescription();
                #endif            }            delete gameManager;
        }
        break;
        case 2: /* server */
        {
            Server server = Server(lconfig);
        }
        break;
        default:
            /* start game normally */
        break;
    }
    return 0;
}
