#ifndef __AMOP_VIRTUALTABLE_HH
#define __AMOP_VIRTUALTABLE_HH

#include "Config.h"
#include "MockObjectException.h"
#include "ObjectHolder.h"
#include <assert.h>

namespace amop
{
namespace Detail
{

//------------------------------------------------------------------
class TNotImplemented
{
public:
	void Func()
	{
		throw TNotImplementedException();
	}
};

//------------------------------------------------------------------
static TFunctionAddress GetNotImplementedFunc()
{
	typedef void (TNotImplemented::*TNotImplementedFuncPtr)();
	
	TNotImplementedFuncPtr _ptr = &TNotImplemented::Func;
	TFunctionAddress p = HorribleCast<TFunctionAddress>(_ptr);
	
	return p; 
}

//------------------------------------------------------------------
class TVirtualTable
{
public:
	void* mVtable[MAX_NUM_VIRTUAL_FUNCTIONS];
	void* mVtpr;
	TObjectHolder* mThis;

	TVirtualTable(TObjectHolder* object)
	{
		mVtpr = &mVtable[0];
		mThis = object;
	}

	static TObjectHolder* GetThis(void* callerThisAddres)
	{
		void* callerThis = *((void**)callerThisAddres);
		
		// _this pointer should point to &mVtable[0]
		TVirtualTable* _this = (TVirtualTable*)callerThis;

		assert(callerThis == &_this->mVtable[0]);

		return _this->mThis;        
	}

    static TVirtualTable* CreateVirtualTable(TObjectHolder* object)
    {
        TVirtualTable* vtable = new TVirtualTable(object);

        for(size_t i = 0 ; i < MAX_NUM_VIRTUAL_FUNCTIONS; ++i)
            vtable->mVtable[i] = GetNotImplementedFunc();

        return vtable;
    }
};

}

}

#endif //__AMOP_VIRTUALTABLE_HH

