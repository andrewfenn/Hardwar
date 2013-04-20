#include <UnitTest++.h>
#include "GameRoot.h"

using namespace Client;
using namespace UnitTest;

struct GameRootFixture
{
    GameRootFixture()
    {
        gameRoot.setLocked(false);        
    }
    
    ~GameRootFixture()
    {
        gameRoot.setLocked(false);
    }

    GameRootSystem gameRoot;
};

TEST_FIXTURE(GameRootFixture, GameRootSetLockedTruePasses)
{
    gameRoot.setLocked(true);
    CHECK(gameRoot.isLocked());
}

TEST_FIXTURE(GameRootFixture, GameRootSetLockedFalsePasses)
{
    gameRoot.setLocked(false);
    CHECK(!gameRoot.isLocked());
}

TEST_FIXTURE(GameRootFixture, GameRootLoadPluginThrowsException)
{
    CHECK_THROW(gameRoot.loadPlugin(""), Ogre::Exception);
}
