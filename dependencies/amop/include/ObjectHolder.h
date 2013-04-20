#ifndef __AMOP_OBJECTHOLDER_HH
#define __AMOP_OBJECTHOLDER_HH

#include "Any.h"
#include "Functor.h"

namespace amop
{
namespace Detail
{

class TComparable;

//------------------------------------------------------------------
class TObjectHolder
{
public:
	TObjectHolder()
	{
	}

	virtual ~TObjectHolder(){}

	template <class R>
	R Return(size_t idx)
	{
		return any_cast<R>(GetReturn(idx));
	}

	template <class T>
	void Actual(size_t funcIdx, size_t idx, T t)
	{
		SetActual(funcIdx, idx, t);
	}

	template <class F>
	typename Detail::Functor<F>::FunctorType Redirector(size_t i)
	{
		any& redirector = GetRedirect(i);
		typedef typename Detail::Functor<F>::FunctorType FunctorType;
		
		if(!redirector.empty())
		{
			return any_cast<FunctorType>(redirector);
		}

		return FunctorType();
	}
	
	virtual any& GetRedirect(size_t idx) = 0;
	virtual void SetRedirect(size_t idx, const any& redirector) = 0;

	virtual void SetReturn(size_t idx, const any& result) = 0;
	virtual void SetReturnDefault(size_t idx, const any& result) = 0;
	
	virtual any& GetReturn(size_t idx) = 0;
	virtual void SetActual(size_t idx, size_t paramId, const any& param) = 0;
	
	virtual void SetExpectDefault(size_t idx, size_t paramId, const TComparable& param) = 0;
	virtual void SetExpect(size_t idx, size_t paramId, const TComparable& param) = 0;


	virtual void AddCallCounter(size_t idx) = 0;
	virtual size_t GetCallCounter(size_t idx) = 0;

    virtual void SetExpectCallCounter(size_t idx, size_t c) = 0;
};

}

}

#endif //__AMOP_OBJECTHOLDER_HH

