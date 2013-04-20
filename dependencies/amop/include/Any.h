#ifndef _AMOP_ANY__HH
#define _AMOP_ANY__HH

#include <algorithm>
#include <typeinfo>

namespace amop
{

class any
{
public: // structors
	any() : content(0) {}

	template<typename ValueType>
		any(const ValueType & value)
		: content(new holder<ValueType>(value))
	{
	}

	any(const any & other)
		: content(other.content ? other.content->clone() : 0)
	{
	}

	~any()
	{
		delete content;
	}

public: // modifiers

	any & swap(any & rhs)
	{
		std::swap(content, rhs.content);
		return *this;
	}

	template<typename ValueType>
		any & operator=(const ValueType & rhs)
	{
		any(rhs).swap(*this);
		return *this;
	}

	any & operator=(const any & rhs)
	{
		any(rhs).swap(*this);
		return *this;
	}

public: // queries

	bool empty() const
	{
		return !content;
	}

	const std::type_info & type() const
	{
		return content ? content->type() : typeid(void);
	}

private: // types

	class placeholder
	{
	public: // structors

		virtual ~placeholder()
		{
		}

	public: // queries

		virtual const std::type_info & type() const = 0;

		virtual placeholder * clone() const = 0;

	};

	template<typename ValueType>
	class holder : public placeholder
	{
	public: // structors

		holder(const ValueType & value)
			: held(value)
		{
		}

	public: // queries

		virtual const std::type_info & type() const
		{
			return typeid(ValueType);
		}

		virtual placeholder * clone() const
		{
			return new holder(held);
		}

	public: // representation

		ValueType held;

	};

private: // representation

	template<typename ValueType>
		friend ValueType * any_cast(any *);

	template<typename ValueType>
		friend ValueType * unsafe_any_cast(any *);

	placeholder * content;

};

class bad_any_cast : public std::bad_cast
{
public:
	virtual const char * what() const throw()
	{
		return "boost::bad_any_cast: "
			"failed conversion using boost::any_cast";
	}
};

template<typename ValueType>
ValueType * any_cast(any * operand)
{
	return operand && operand->type() == typeid(ValueType)
		? &static_cast<any::holder<ValueType> *>(operand->content)->held
		: 0;
}

template<typename ValueType>
const ValueType * any_cast(const any * operand)
{
	return any_cast<ValueType>(const_cast<any *>(operand));
}

template <class T>
struct remove_reference
{
	typedef T type;
};

template <class T>
struct remove_reference<T&>
{
	typedef T type;
};

template <class T>
struct remove_reference<const T&>
{
	typedef T type;
};

template<typename ValueType>
ValueType any_cast(const any & operand)
{
	typedef typename remove_reference<ValueType>::type nonref;

	const nonref * result = any_cast<nonref>(&operand);
	if(!result)
		throw bad_any_cast();
	return *result;
}

template<typename ValueType>
ValueType any_cast(any & operand)
{
	typedef typename remove_reference<ValueType>::type nonref;

	nonref * result = any_cast<nonref>(&operand);
	if(!result)
		throw bad_any_cast();
	return *result;
}

// Note: The "unsafe" versions of any_cast are not part of the
// public interface and may be removed at any time. They are
// required where we know what type is stored in the any and can't
// use typeid() comparison, e.g., when our types may travel across
// different shared libraries.
template<typename ValueType>
ValueType * unsafe_any_cast(any * operand)
{
	return &static_cast<any::holder<ValueType> *>(operand->content)->held;
}

template<typename ValueType>
const ValueType * unsafe_any_cast(const any * operand)
{
	return any_cast<ValueType>(const_cast<any *>(operand));
}

}

#endif //_AMOP_ANY__HH

