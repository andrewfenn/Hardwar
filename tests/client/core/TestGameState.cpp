#include <UnitTest++.h>

#include "GameState.h"

using namespace Client;
using namespace UnitTest;

/*
   void setParent(GameState*, GameTaskList*, Ogre::Root*, Ogre::Viewport*);
   const Ogre::String getName();
   void* getParent();
   void shutdown();
   void* add(GameState*);
   void* get(const Ogre::String&);
   void replace(GameState*);
   bool has(const Ogre::String&);
   bool has(GameState&);
   void remove(const Ogre::String&);
   void markRemoval();
   void removeAllChildren();
   void pause();
   void resume();
   bool shouldRemove();
   void updateAllChildren(unsigned long timeElapsed);
*/

class TestState : public GameState { public: TestState() : GameState("TestState") {} };
class ParentTestState : public GameState { public: ParentTestState() : GameState("ParentTestState") {} };
class TestState2 : public GameState { public: TestState2() : GameState("TestState2") {} };

struct GameStateFixture
{
    GameStateFixture()
    {
    }
    
    ~GameStateFixture()
    {

    }

    TestState testState;
    ParentTestState parentTestState;
    TestState2 testState2;
};

TEST_FIXTURE(GameStateFixture, GameStateGetNameReturnsCorrectString)
{
    CHECK_EQUAL("TestState", testState.getName());
    CHECK_EQUAL("ParentTestState", parentTestState.getName());
    CHECK_EQUAL("TestState2", testState2.getName());
}

TEST_FIXTURE(GameStateFixture, GameStateSetParentReturnsErrorWithNullPointers)
{
    std::cerr << ">> GameStateSetParentReturnsErrorWithNullPointers Unimplemented <<" << std::endl;
/*    Ogre::Root root;
    GameTaskList taskList;
    Ogre::Viewport* viewport = nullptr;

    //void setParent(GameState*, GameTaskList*, Ogre::Root*, Ogre::Viewport*);
    CHECK_THROW(testState.setParent(parentTestState, taskList, root, nullptr), Ogre::Exception);
    CHECK_THROW(testState.setParent(parentTestState, taskList, nullptr, viewport), Ogre::Exception);
    CHECK_THROW(testState.setParent(parentTestState, nullptr, root, viewport), Ogre::Exception);
    CHECK_THROW(testState.setParent(nullptr, taskList, root, viewport), Ogre::Exception);*/
}

