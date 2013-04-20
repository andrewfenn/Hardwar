#include <UnitTest++.h>

#include "GameTask.h"

using namespace Client;
using namespace UnitTest;

class TestTask: public GameTask
{
    public:
        void init() {}
        void update() {}
        void shutdown() {}
};

TEST(GameTaskAddToTaskListReturnsTask)
{
    UNITTEST_TIME_CONSTRAINT(1);
    GameTaskList taskList;
    TestTask* testTask = OGRE_NEW TestTask;
    CHECK_EQUAL(testTask, taskList.add("testTask", testTask));
}

TEST(GameTaskAddToTaskListReturnsTaskWithList)
{
    UNITTEST_TIME_CONSTRAINT(1);
    GameTaskList taskList;
    TestTask* testTask = OGRE_NEW TestTask;

    CHECK(testTask->getTaskList() == nullptr);
    taskList.add("testTask", testTask);
    CHECK(testTask->getTaskList() != nullptr);
}

TEST(GameTaskAddToTaskListReturnsWithGetTask)
{
    UNITTEST_TIME_CONSTRAINT(1);
    GameTaskList taskList;
    TestTask* testTask = OGRE_NEW TestTask;
    taskList.add("testTask", testTask);

    CHECK(taskList.get("testTask") == testTask);
}

TEST(GameTaskAddToTaskListReturnsHasTask)
{
    UNITTEST_TIME_CONSTRAINT(1);
    GameTaskList taskList;
    TestTask* testTask = OGRE_NEW TestTask;
    taskList.add("testTask", testTask);

    CHECK(taskList.has("testTask"));
}

TEST(RemoveGameTaskWorks)
{
    UNITTEST_TIME_CONSTRAINT(1);
    GameTaskList taskList;
    TestTask* testTask = OGRE_NEW TestTask;
    taskList.add("testTask", testTask);

    CHECK(taskList.has("testTask"));
    CHECK(taskList.remove("testTask"));
    CHECK(!taskList.has("testTask"));
}

TEST(RemoveAllGameTaskWorks)
{
    UNITTEST_TIME_CONSTRAINT(1);
    GameTaskList taskList;
    TestTask* testTask = OGRE_NEW TestTask;
    taskList.add("testTask", testTask);

    CHECK(taskList.has("testTask"));
    taskList.removeAll();
    CHECK(!taskList.has("testTask"));
}

TEST(RemoveAllGameTaskReturnsFalseWithRemove)
{
    UNITTEST_TIME_CONSTRAINT(1);
    GameTaskList taskList;
    TestTask* testTask = OGRE_NEW TestTask;
    taskList.add("testTask", testTask);

    CHECK(taskList.has("testTask"));
    taskList.removeAll();
    CHECK(!taskList.has("testTask"));
    CHECK(!taskList.remove("testTask"));
}

TEST(CanRemoveThenAddGameTask)
{
    UNITTEST_TIME_CONSTRAINT(1);
    GameTaskList taskList;
    taskList.add("testTask", OGRE_NEW TestTask);
    taskList.removeAll();

    taskList.add("testTask", OGRE_NEW TestTask);
    CHECK(taskList.has("testTask"));
}
