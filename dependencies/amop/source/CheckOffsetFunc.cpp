#include "CheckOffsetFunc.h"

namespace amop
{

namespace Detail
{

//------------------------------------------------------------------
    bool Inner::TCheck::mIsCheckCall = false;

    size_t Inner::TCheck::mDestructorOffset = 0xFFFFFFFF;
}

}

