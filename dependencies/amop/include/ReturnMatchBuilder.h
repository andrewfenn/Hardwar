#ifndef __AMOP_RETURNMATCHBUILDER_HH
#define __AMOP_RETURNMATCHBUILDER_HH

#include "ObjectHolder.h"
#include "Functor.h"
#include "Comparable.h"

namespace amop
{

//------------------------------------------------------------------
template <typename F>
class TReturnMatchBuilder
{
public:
	TReturnMatchBuilder(Detail::TObjectHolder* objectHolder,
		size_t offset) : mObjectHolder(objectHolder),
			mOffset(offset)
	{
	}

	template<class T>
	TReturnMatchBuilder Will(T result)
	{
		typedef typename Detail::Functor<F>::ReturnType R;
		typedef typename Detail::RemoveReference<R>::Type ToType;

		//****************************************/
		//** If you see this, that's mean your  **/
		//** expect type is not convertiable to **/
		//** actual type.						**/
		//****************************************/
		typedef int ItIsNotConvertible[
			Detail::IsConvertible<T, ToType>::Result ? 1 : -1];
				
		mObjectHolder->SetReturnDefault(mOffset, (ToType)result);
		return *this;
	}

	template<class T>
	TReturnMatchBuilder Wills(T result)
	{
		typedef typename Detail::RemoveReference<typename Detail::Functor<F>::ReturnType>::Type ToType;

		//****************************************/
		//** If you see this, that's mean your  **/
		//** expect type is not convertiable to **/
		//** actual type.						**/
		//****************************************/
		typedef int ItIsNotConvertible[
			Detail::IsConvertible<T, ToType>::Result ? 1 : -1];
				
		mObjectHolder->SetReturn(mOffset, (ToType)result);
		return *this;
	}

	template<int I, class T>
	TReturnMatchBuilder Expect(T expect)
	{
		typedef typename Detail::Get< typename Detail::Functor<F>::ParameterTypes, I>::Type ToTypeRef;
		typedef typename Detail::RemoveReference<ToTypeRef>::Type ToType;

		//****************************************/
		//** If you see this, that's mean your  **/
		//** expect type is not convertiable to **/
		//** actual type.						**/
		//****************************************/
		typedef int ItIsNotConvertible[
			Detail::IsConvertible<T, ToType>::Result ? 1 : -1];
		
		Detail::TComparable compare = Detail::TComparable::MakeCompare<ToType>(expect);

		mObjectHolder->SetExpectDefault(mOffset, I, compare);
		return *this;
	}

	template<int I, class T>
	TReturnMatchBuilder Expects(T expect)
	{
		typedef typename Detail::Get< typename Detail::Functor<F>::ParameterTypes, I>::Type ToTypeRef;
		typedef typename Detail::RemoveReference<ToTypeRef>::Type ToType;

		//****************************************/
		//** If you see this, that's mean your  **/
		//** expect type is not convertiable to **/
		//** actual type.						**/
		//****************************************/
		typedef int ItIsNotConvertible[
			Detail::IsConvertible<T, ToType>::Result ? 1 : -1];
		
		Detail::TComparable compare = Detail::TComparable::MakeCompare<ToType>(expect);

		mObjectHolder->SetExpect(mOffset, I, compare);
		return *this;
	}

	template <typename T>
	TReturnMatchBuilder Redirect(T freeFunc)
	{
		typename Detail::Functor<F>::FunctorType functor(freeFunc);

		mObjectHolder->SetRedirect(mOffset, functor);

		return *this;
	}

	template <typename C, typename T>
	TReturnMatchBuilder Redirect(C* pointer, T func)
	{
		typename Detail::Functor<F>::FunctorType functor(pointer, func);

		mObjectHolder->SetRedirect(mOffset, functor);

		return *this;
	}

	size_t Count()
	{
		return mObjectHolder->GetCallCounter(mOffset);
	}

    void Count(size_t counter)
    {
        return mObjectHolder->SetExpectCallCounter(mOffset, counter);
    }

private:
	size_t mOffset;
	Detail::TObjectHolder* mObjectHolder;
};

}


#endif //__AMOP_RETURNMATCHBUILDER_HH
