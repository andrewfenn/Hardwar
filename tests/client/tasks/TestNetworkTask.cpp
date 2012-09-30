#include <UnitTest++.h>

#include "NetworkTask.h"

using namespace Client;
using namespace UnitTest;

struct NetworkTaskFixture
{
    NetworkTaskFixture()
    {
    }

    ~NetworkTaskFixture()
    {
    }
};

TEST_FIXTURE(NetworkTaskFixture, NetworkTaskUnimplemented)
{
    std::cerr << ">> NetworkTask no unit tests <<" << std::endl;
}
