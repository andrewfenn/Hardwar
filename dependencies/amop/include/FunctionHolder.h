#ifndef __AMOP_FUNCTIONHOLDER_HH
#define __AMOP_FUNCTIONHOLDER_HH

#include "Config.h"
#include "Functor.h"

namespace amop
{
namespace Detail
{
	//------------------------------------------------------------------
	template <int I, class T, bool>
	struct FunctionHolderBase;
	
	//------------------------------------------------------------------
	template <int I, class T>
	struct FunctionHolder : public FunctionHolderBase<I, T, 
		!IsEqual< typename Functor<T>::ReturnType, void >::Result  >
	{
	};
	
#ifndef VT_THIS 
#define VT_THIS TVirtualTable::GetThis(this)
#endif


#ifndef HANDLE_REDIRECT_R
#define HANDLE_REDIRECT_R			\
	do { \
	if(VT_THIS->template Redirector<FunctorType>(I)){ \
		return VT_THIS->template Redirector<FunctorType>(I)(DETAIL_FUNC_PARAMS(n, t));	\
		}	\
	}while(false)
#endif

#ifndef HANDLE_REDIRECT
#define HANDLE_REDIRECT			\
	do { \
	if(VT_THIS->template Redirector(I)){ \
		VT_THIS->template Redirector<FunctorType>(I)(DETAIL_FUNC_PARAMS(n, t));	\
		}	\
	}while(false)
#endif
	
	template <int I, class C>
	struct FunctionHolderBase<I, void (C::*)(), false>
	{			
		void Func() { 
			typedef void (C::*FunctorType)();
			VT_THIS->AddCallCounter(I); 
			if(VT_THIS->template Redirector<FunctorType>(I))
				VT_THIS->template Redirector<FunctorType>(I)();
		}
	};

	template <int I, class C>
	struct FunctionHolderBase<I, void (C::*)() const, false>
	{			
		typedef void (C::*FunctorType)();
		void Func() 
		{ 
			VT_THIS->AddCallCounter(I); 
			
			if(VT_THIS->template Redirector<FunctorType>(I))
			{
				VT_THIS->template Redirector<FunctorType>(I)();
			}
		}
	};

	//------------------------------------------------------------------
	template <int I, class R, class C>
	struct FunctionHolderBase<I, R (C::*)(), true>
	{			
		typedef R (C::*FunctorType)();
		R Func() 
		{ 
			VT_THIS->AddCallCounter(I);

			if(VT_THIS->template Redirector<FunctorType>(I))
			{
				return VT_THIS->template Redirector<FunctorType>(I)();
			}

			return VT_THIS->template Return<R>(I);
		}
	};

	//------------------------------------------------------------------
	template <int I, class R, class C>
	struct FunctionHolderBase<I, R (C::*)() const, true>
	{			
		typedef R (C::*FunctorType)();
		R Func() 
		{ 
			VT_THIS->AddCallCounter(I);

			if(VT_THIS->template Redirector<FunctorType>(I))
			{
				return VT_THIS->template Redirector<FunctorType>(I)();
			}

			return VT_THIS->template Return<R>(I);
		}
	};

#define DETAIL_ACTUALCALL_ITEM(n, t)		VT_THIS->template Actual<T##n>(I,DETAIL_DEC(n),t##n);
#define DETAIL_ACTUALCALL_ITEM_END(n, t)	VT_THIS->template Actual<T##n>(I,DETAIL_DEC(n),t##n);


	//------------------------------------------------------------------
#define	DETAIL_FUNCTIONHOLDER_BUILD_O(n)				\
	template <int I, class C, DETAIL_TPARAMS(n)>	\
	struct FunctionHolderBase<I, void (C::*)(DETAIL_ARGS(n)), false>   \
	{																  \
		typedef void (C::*FunctorType)(DETAIL_FUNC_PARAMS(n, t));		\
		void Func(DETAIL_FUNC_PARAMS(n, t))								  \
		{															  \
			DETAIL_REPEAT(n,DETAIL_ACTUALCALL_ITEM,DETAIL_ACTUALCALL_ITEM_END,t);  \
			VT_THIS->AddCallCounter(I);								  \
			if(VT_THIS->template Redirector<FunctorType>(I))					\
				VT_THIS->template Redirector<FunctorType>(I)(DETAIL_ARGS_P(n));	\
		}															  \
	};																\
	template <int I, class C, DETAIL_TPARAMS(n)>						\
	struct FunctionHolderBase<I, void (C::*)(DETAIL_ARGS(n)) const, false>   \
	{																  \
		typedef void (C::*FunctorType)(DETAIL_FUNC_PARAMS(n, t));		\
		void Func(DETAIL_FUNC_PARAMS(n, t))								  \
		{															  \
			DETAIL_REPEAT(n,DETAIL_ACTUALCALL_ITEM,DETAIL_ACTUALCALL_ITEM_END,t);  \
			VT_THIS->AddCallCounter(I);								  \
			if(VT_THIS->template Redirector<FunctorType>(I))					\
				VT_THIS->template Redirector<FunctorType>(I)(DETAIL_ARGS_P(n));	\
		}															  \
	};																	

#define	DETAIL_FUNCTIONHOLDER_BUILD_R(n)				\
	template <int I, class R, class C, DETAIL_TPARAMS(n)>				\
	struct FunctionHolderBase<I, R (C::*)(DETAIL_ARGS(n)), true>		\
	{																	\
		typedef R (C::*FunctorType)(DETAIL_FUNC_PARAMS(n, t));		\
		R Func(DETAIL_FUNC_PARAMS(n,t))									\
		{																\
			DETAIL_REPEAT(n,DETAIL_ACTUALCALL_ITEM,DETAIL_ACTUALCALL_ITEM_END,t);  \
			VT_THIS->AddCallCounter(I);									\
			if(VT_THIS->template Redirector<FunctorType>(I))					\
				return VT_THIS->template Redirector<FunctorType>(I)(DETAIL_ARGS_P(n));	\
			return VT_THIS->template Return<R>(I);								\
		}																\
	};			\
	template <int I, class R, class C, DETAIL_TPARAMS(n)>				\
	struct FunctionHolderBase<I, R (C::*)(DETAIL_ARGS(n)) const, true>		\
	{																	\
		typedef R (C::*FunctorType)(DETAIL_FUNC_PARAMS(n, t));		\
		R Func(DETAIL_FUNC_PARAMS(n,t))									\
		{																\
			DETAIL_REPEAT(n,DETAIL_ACTUALCALL_ITEM,DETAIL_ACTUALCALL_ITEM_END,t);  \
			VT_THIS->AddCallCounter(I);									\
			if(VT_THIS->template Redirector<FunctorType>(I))					\
				return VT_THIS->template Redirector<FunctorType>(I)(DETAIL_ARGS_P(n));	\
			return VT_THIS->template Return<R>(I);								\
		}																\
	};

#define DETAIL_FUNCTIONHOLDER_BUILD(n) DETAIL_FUNCTIONHOLDER_BUILD_O(n) \
	DETAIL_FUNCTIONHOLDER_BUILD_R(n)

DETAIL_FUNCTIONHOLDER_BUILD(1);
DETAIL_FUNCTIONHOLDER_BUILD(2);
DETAIL_FUNCTIONHOLDER_BUILD(3);
DETAIL_FUNCTIONHOLDER_BUILD(4);
DETAIL_FUNCTIONHOLDER_BUILD(5);
DETAIL_FUNCTIONHOLDER_BUILD(6);
DETAIL_FUNCTIONHOLDER_BUILD(7);
DETAIL_FUNCTIONHOLDER_BUILD(8);
/*
DETAIL_FUNCTIONHOLDER_BUILD(9);
DETAIL_FUNCTIONHOLDER_BUILD(10);
DETAIL_FUNCTIONHOLDER_BUILD(11);
DETAIL_FUNCTIONHOLDER_BUILD(12);
DETAIL_FUNCTIONHOLDER_BUILD(13);
DETAIL_FUNCTIONHOLDER_BUILD(14);
DETAIL_FUNCTIONHOLDER_BUILD(15);
*/

#undef VT_THIS
#undef HANDLE_REDIRECT
#undef HANDLE_REDIRECT_R
}

}

#endif //__FUNCTIONHOLDER_HH
