#ifndef __AMOP_MOCKOBJECTEXECEPTION__HH
#define __AMOP_MOCKOBJECTEXECEPTION__HH

#include "Any.h"

namespace amop
{

    //------------------------------------------------------------------
    class TNotImplementedException
    {
    };

    //------------------------------------------------------------------
    class TNotPureVirtualException
    {
    };

    //------------------------------------------------------------------
    class TNotEqualException
    {
    public:
        TNotEqualException(size_t param, const any& expect, const any& actual) :
          mParam(param), mExpect(expect), mActual(actual){};

          any& GetExpect() { return mExpect; }
          any& GetActual() { return mActual; }

          size_t GetParam() { return mParam; }

    protected:
        any mExpect;
        any mActual;
        size_t mParam;

    };

    //------------------------------------------------------------------
    class TCallCountException
    {
    public:
        TCallCountException(size_t expect, size_t actual) :
          mExpect(expect), mActual(actual){};

          size_t GetExpect() { return mExpect; }
          size_t GetActual() { return mActual; }

    protected:
        size_t mExpect;
        size_t mActual;
    };

}

#endif //__AMOP_MOCKOBJECTEXECEPTION__HH
