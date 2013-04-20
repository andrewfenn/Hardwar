#include <UnitTest++.h>
#include "Any.h"
#include "Tuple.h"
#include "Functor.h"

using namespace amop;

template<class T1, class T2>
struct Equal
{
	enum { Result = false };
};

template<class T>
struct Equal<T,T>
{
	enum { Result = true };
};

//------------------------------------------------------------------
TEST(TestTuple)
{
	typedef Detail::Tuple<float , int , double > TestTuple;
	typedef Detail::Tuple<> EmptyTuple;

	int len = TestTuple::Length;
	CHECK_EQUAL(len, 3);
	
	bool result = Equal< Detail::Get<TestTuple, 1>::Type, int >::Result;
	CHECK( result );
}

template <class T>
static void MakeFunctor(T f)
{
	Detail::Functor<T>();
}

struct TestFunctorClass
{
	void Foo1() {};
	int Foo2(float, int, double) { return 0; }
};

//------------------------------------------------------------------
TEST(TestFunctorParams)
{
	using namespace Detail;
	
	
	typedef Detail::Functor<void (TestFunctorClass::*)()> F1; //(&TestFunctorClass::Foo1); 
	typedef Detail::Functor<int (TestFunctorClass::*)(float, int, double) > F2; //(&TestFunctorClass::Foo2); 

	MakeFunctor(&TestFunctorClass::Foo1);
	MakeFunctor(&TestFunctorClass::Foo2);

	bool result = Equal< Get<F2::ParameterTypes, 1>::Type, int >::Result;
	CHECK(result);
}
