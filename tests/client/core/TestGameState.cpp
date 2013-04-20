#include <UnitTest++.h>

#include "GameState.h"
#include "GameRoot.h"
#include "Ogre.h"

using namespace Client;
using namespace UnitTest;

class TestState : public GameState { public: TestState() : GameState("TestState") {} };
class ParentTestState : public GameState { public: ParentTestState() : GameState("ParentTestState") {} };
class TestState2 : public GameState
{
    public:
    bool mUpdating;
    TestState2() : GameState("TestState2")
    {
        mUpdating = false;
    }

    void update(unsigned long timeElapsed)
    {
        mUpdating = true;
    }
};

struct GameStateFixture
{
    GameStateFixture()
    {
        testState2.mUpdating = false;
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

TEST_FIXTURE(GameStateFixture, GameStateSetParentReturnsParents)
{
    std::cerr << ">> GameStateSetParentReturnsParents Unimplemented <<" << std::endl;

    /* Can not test this due to the fact that creating the camera needed for the functon
    call requires starting up a render window for Ogre, that's something I'm not interested
    in doing for the unit tests. */
    //testState.setParent((Client::GameState*)&parentTestState, &taskList, root, viewport);
}

TEST_FIXTURE(GameStateFixture, GameStateShutdownRemovesAllParents)
{
    std::cerr << ">> GameStateShutdownRemovesAllParents Unimplemented <<" << std::endl;
    /* Can not test this due to the fact that creating the camera needed for the functon
    call requires starting up a render window for Ogre, that's something I'm not interested
    in doing for the unit tests. */
}

TEST_FIXTURE(GameStateFixture, GameStateAddReturnsParents)
{
    std::cerr << ">> GameStateAddReturnsParents Unimplemented <<" << std::endl;
    /* Can not test this due to the fact that creating the camera needed for the functon
    call requires starting up a render window for Ogre, that's something I'm not interested
    in doing for the unit tests. */
}

TEST_FIXTURE(GameStateFixture, GameStateupdateAllChildrenUpdatesState)
{
    std::cerr << ">> GameStateupdateAllChildrenUpdatesState Unimplemented <<" << std::endl;
    /* Need to be able to use the setParent function, see above messages */
}

TEST_FIXTURE(GameStateFixture, GameStateshouldRemoveDeletesOnUpdate)
{
    std::cerr << ">> GameStateshouldRemoveDeletesOnUpdate Unimplemented <<" << std::endl;
    /* Need to be able to use the setParent function, see above messages */
}

TEST_FIXTURE(GameStateFixture, GameStatePauseWillNotRunUpdate)
{
    std::cerr << ">> GameStatePauseWillNotRunUpdate Unimplemented <<" << std::endl;
    /* Need to be able to use the setParent function, see above messages */
}

TEST_FIXTURE(GameStateFixture, GameStateResumeWillResume)
{
    std::cerr << ">> GameStateResumeWillResume Unimplemented <<" << std::endl;
    /* Need to be able to use the setParent function, see above messages */
}