#ifndef __AMOP_CONFIG__HH
#define __AMOP_CONFIG__HH

namespace amop
{

#define MAX_NUM_VIRTUAL_FUNCTIONS 25

#define DETAIL_TYPELIST_WITHOUT_HEAD typename T2,typename T3, typename T4, \
  typename T5, typename T6, typename T7, typename T8, typename T9, \
  typename T10, typename T11, typename T12, typename T13, typename T14, \
  typename T15

#define DETAIL_TYPELIST typename T1, DETAIL_TYPELIST_WITHOUT_HEAD

#define DETAIL_TYPELIST_DEFAULT typename T1=Empty, typename T2=Empty,typename T3=Empty, typename T4=Empty, \
  typename T5=Empty, typename T6=Empty, typename T7=Empty, typename T8=Empty, typename T9=Empty, \
  typename T10=Empty, typename T11=Empty, typename T12=Empty, typename T13=Empty, typename T14=Empty, \
  typename T15=Empty

#define DETAIL_TYPELIST_EMPTY Empty, Empty, Empty, Empty, \
  Empty, Empty, Empty, Empty, Empty, \
  Empty, Empty, Empty, Empty, Empty, \
  Empty


namespace Detail
{

struct Empty {};

template <class T1, class T2>
struct IsEqual { enum { Result = false }; };

template <class T>
struct IsEqual<T,T> { enum { Result = true }; };

template <class T>
struct IsPointerToMethod
{
	enum {Result = 1 };
};

template <class T>
struct RemoveReference
{
	typedef T Type;
};

template <class T>
struct RemoveReference<T&>
{
	typedef T Type;
};

template <class T>
struct RemoveReference<const T&>
{
	typedef T Type;
};

// This union is declared outside the horrible_cast because BCC 5.5.1
// can't inline a function with a nested class, and gives a warning.
template <class OutputClass, class InputClass>
union THorribleUnion{
	OutputClass out;
	InputClass in;
};

#ifdef WIN32

template <class OutputClass, class InputClass>
inline OutputClass HorribleCast(const InputClass input){
	THorribleUnion<OutputClass, InputClass> u;
	// Cause a compile-time error if in, out and u are not the same size.
	// If the compile fails here, it means the compiler has peculiar
	// unions which would prevent the cast from working.
	typedef int ERROR_CantUseHorrible_cast[sizeof(InputClass)==sizeof(u) 
		&& sizeof(InputClass)==sizeof(OutputClass) ? 1 : -1];
	u.in = input;
	return u.out;
}

#endif

#ifdef __GNUC__

struct GnuMFP
{
	union
	{
		void* funcadr;
		int vtable_index_2; // = vindex*2+1, always odd
	};
	int delta;
};

template <class OutputClass, class InputClass>
inline OutputClass HorribleCast(const InputClass input){
	THorribleUnion<GnuMFP, InputClass> u;
	// Cause a compile-time error if in, out and u are not the same size.
	// If the compile fails here, it means the compiler has peculiar
	// unions which would prevent the cast from working.
	typedef int ERROR_CantUseHorrible_cast[sizeof(InputClass)==sizeof(u) 
		&& sizeof(InputClass)==sizeof(GnuMFP) ? 1 : -1];
	u.in = input;
	return u.out.funcadr;
}



#endif

typedef void* TFunctionAddress;

}

}

#endif //__AMOP_CONFIG__HH

