#include <UnitTest++.h>

#include "GameManager.h"

using namespace Client;
using namespace UnitTest;

struct GameManagerFixture
{
    GameManagerFixture()
    {
        // TODO: Set Ogre::Root mock and create game manager instance
    }
    
    ~GameManagerFixture()
    {

    }

    GameManager *gameManager;
};
