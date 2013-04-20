#ifndef __AMOP_MOCKOBJECT_HH
#define __AMOP_MOCKOBJECT_HH

#include "Config.h"
#include "MockObjectBase.h"
#include "MockObjectException.h"
#include "ReturnMatchBuilder.h"
#include "Destructor.h"

namespace amop
{

//------------------------------------------------------------------
template <class T>
class TMockObject : public Detail::TMockObjectBase
{
public:
	operator T*()
	{ 
		return (T*)GetVptr();
	}	

	template <class F>
	TReturnMatchBuilder<F> Method(F method)
	{
		typedef int ItIsNotPointerToMemberMethod[
			Detail::IsPointerToMethod<F>::Result ? 1 : -1];
		
		return CreateMatchBuilder<F, T>(method);
	}

    TReturnMatchBuilder<void (T::*)(void*)> Method(const Destructor& d)
	{
		return CreateMatchBuilder<void (T::*)(void*), T>(d);
	}



private:
	

};

}


#endif //__AMOP_MOCKOBJECT_HH

