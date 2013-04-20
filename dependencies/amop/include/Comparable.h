#ifndef __AMOP_COMPARABLE_HH
#define __AMOP_COMPARABLE_HH

#include "Any.h"

namespace amop
{
namespace Detail
{

template <class From, class To>
struct IsConvertible
{
	struct Yes { char c[10]; };;
	typedef char No;

	static From MakeFrom();
	static No Convertor(...);
	static Yes Convertor(To);

	enum { Result = (sizeof (Convertor(MakeFrom())) == sizeof(Yes)) };
};
	
class TComparable;

//------------------------------------------------------------------
class TComparableBase
{
public:
	virtual ~TComparableBase(){}

	virtual bool Compare(const any& other) const = 0;	
	virtual TComparableBase* Clone() = 0;
};

//------------------------------------------------------------------
template<class From, class To>
class TComparableImp : public TComparableBase
{
public:
	TComparableImp(const any& data) : mData(data) {}

	virtual bool Compare(const any& other) const
	{
		return any_cast<To>(other) == (To)any_cast<From>(mData);
	}

	virtual TComparableBase* Clone() { return new TComparableImp<From, To>(mData); }

protected:
	any mData;
};


//------------------------------------------------------------------
class TComparable
{
public:
	TComparable() : mHolder(NULL) {}

	template<class To, class From>
	static TComparable MakeCompare(From value)
	{
		TComparable compare;
		compare.mHolder = new TComparableImp<From, To>(value);
		return compare;
	}

	TComparable(const TComparable& other)
	{
		mHolder = other.mHolder ? other.mHolder->Clone() : NULL;
	}

	~TComparable()
	{
		delete mHolder;
	}

	TComparable& operator= (const TComparable& other)
	{
		delete mHolder;
		mHolder = other.mHolder ? other.mHolder->Clone() : NULL;

		return *this;
	}	

	bool operator == (const any& other) const
	{
		return mHolder->Compare(other);
	}	

	bool operator != (const any& other) const
	{
		return !(*this == other);
	}

	TComparableBase* mHolder;
};

}
}

#endif //__AMOP_COMPARABLE_HH

