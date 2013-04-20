#include <UnitTest++.h>
#include "CheckOffsetFunc.h"

using namespace amop;

//------------------------------------------------------------------
class IInterface
{
public:
	~IInterface(){};

	virtual int foo1() = 0;
	virtual void foo2() = 0;
	virtual void foo3() = 0;
	virtual void foo4() = 0;
	virtual void foo5() = 0;
	virtual int foo6() = 0;
	virtual void foo7() = 0;
};

//------------------------------------------------------------------
TEST(TestGetOffset)
{
    CHECK_EQUAL(1, Detail::Inner::TCheckOffset::GetOffset(&IInterface::foo2));
	CHECK_EQUAL(0, Detail::Inner::TCheckOffset::GetOffset(&IInterface::foo1));

	CHECK_EQUAL(4, Detail::Inner::TCheckOffset::GetOffset(&IInterface::foo5));
	CHECK_EQUAL(5, Detail::Inner::TCheckOffset::GetOffset(&IInterface::foo6));
	CHECK_EQUAL(6, Detail::Inner::TCheckOffset::GetOffset(&IInterface::foo7));	
}

//------------------------------------------------------------------
class A {
 public:
       virtual float Afunc() { return 2.0f; };
};

//------------------------------------------------------------------
class B {
 public: 
      int Bfunc() { return 3; };
};

//------------------------------------------------------------------
// C is a single inheritance class, derives only from A
class C: public A {
 public: 
     int Cfunc() { return 40; };
};

//------------------------------------------------------------------
// D uses multiple inheritance
class D: public A, public B {
 public: 
    virtual int Dfunc() { return 5; };
};

