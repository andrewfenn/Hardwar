#include <UnitTest++.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
    #include "GameRootLinux.h"
#elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    #include "GameRootWindows.h"
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    // TODO: Add OSX implementation
#endif

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

    #if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
        GameRootLinux gameRoot;
    #elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        GameRootWindows gameRoot;
    #elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        GameRootOSX gameRoot;
    #endif
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
