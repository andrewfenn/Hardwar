#ifndef __AMOP_TUPLE__HH
#define __AMOP_TUPLE__HH

#include "Config.h"
#include "MacroParams.h"

namespace amop
{

namespace Detail
{
	template <DETAIL_TPARAMS_DEF(15, Empty)> 
	struct Tuple;

	template <DETAIL_TPARAMS(15)> 
	struct Tuple
	{
	  typedef T1 head;
	  typedef Tuple< T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15 > tail;
	  enum
	  {
		 Length = tail::Length+1
	  };
	};

	template<>
	struct Tuple<DETAIL_LIST_ITEMS(15, Empty)> 
	{
	  enum
	  {
		 Length = 0
	  };
	};

	template< typename Typelist >
	struct Length
	{
		enum { Value = Typelist::Length };
	};

	template<
		typename Typelist, 
		int Index, //requested element index
		int Step = 0, //current recusion step
		bool Stop=(Index==Step), //stop recusion flag
		bool OutOfRange = Length<Typelist>::Value==0 //out of range flag
	>
	struct Get
	{
		typedef typename Get<typename Typelist::tail, Index, Step+1>::Type Type;
	};

	//"out of range" specialization
	template<
		typename Typelist, 
		int Index,
		int Step,
		bool Stop
	>
	struct Get< Typelist, Index, Step, Stop, true >
	{
		//if OutOfRange is 'true' the 'type' is undefined
		//so we'll get a compile-time error
	};

	//"element found" specialization
	template<
		typename Typelist, 
		int Index,
		int Step,
		bool OutOfRange
	>
	struct Get<Typelist, Index, Step, true, OutOfRange >
	{
		//the index is equal to the recursion step
		//so the result type is the head of the Typlist and stop!
		typedef typename Typelist::head Type;
	};

}

}

#endif //__AMOP_TUPLE__HH
