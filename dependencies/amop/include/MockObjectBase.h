#ifndef __AMOP_MOCKOBJECTBASE_HH
#define __AMOP_MOCKOBJECTBASE_HH

#include "Any.h"
#include "ObjectHolder.h"
#include "ReturnMatchBuilder.h"
#include "CheckOffsetFunc.h"
#include "CallHandler.h"
#include "Destructor.h"
#include <map>
#include <vector>

namespace amop
{

namespace Detail
{
	class TVirtualTable;


	//------------------------------------------------------------------
	class TMockObjectBase : public Detail::TObjectHolder
	{
	public:
		TMockObjectBase();
		~TMockObjectBase();

		void Clear();
        void Verify();

	protected:
		typedef std::vector<TComparable> TComparableList;
		typedef std::map<size_t, TComparableList> TParamMap;
		typedef std::map<size_t, TComparable> TParamDefaultMap;
		
		std::map<size_t, any> mReturnDefaults;
		std::map<size_t, std::vector<any> > mReturns;
		std::map<size_t, size_t> mCallCounter;
        std::map<size_t, size_t> mExpectCallCounter;
		
		std::map<size_t, any> mRedirects;
		std::map<size_t,  TParamMap> mExpects;
		std::map<size_t,  TParamDefaultMap> mExpectDefaults;

		Detail::TVirtualTable* mVirtualTable;	

		void AddCallCounter(size_t idx);
		size_t GetCallCounter(size_t idx);
        void SetExpectCallCounter(size_t idx, size_t c);

		any& GetRedirect(size_t idx);
		void SetRedirect(size_t idx, const any& redirect);

		void SetReturnDefault(size_t idx, const any& result);
		void SetReturn(size_t idx, const any& result);
		any& GetReturn(size_t idx);

		void SetActual(size_t idx, size_t paramId, const any& param);
		void SetExpectDefault(size_t idx, size_t paramId, const TComparable& param);
		void SetExpect(size_t idx, size_t paramId, const TComparable& param);

        bool HaveExpectDefault(size_t idx, size_t paramId);
        bool HaveExpect(size_t idx, size_t paramId);

		void* GetVptr();

		template <class F>
		TReturnMatchBuilder<F> CreateMatchBuilder(size_t offset, TFunctionAddress data)
		{
			assert(offset < MAX_NUM_VIRTUAL_FUNCTIONS);
			mVirtualTable->mVtable[offset] = data;

			return TReturnMatchBuilder<F>(this, offset);
		}

		template <class F, class Type>
		TReturnMatchBuilder<F> CreateMatchBuilder(F method)
		{
            size_t offset = Detail::Inner::TCheckOffset::GetOffset(method);

			return CreateMatchBuilder<F>(offset, 
				Detail::CallHandler::Create<F>(offset) );
		}

        template <class F, class Type>
		TReturnMatchBuilder<F> CreateMatchBuilder(const Destructor& d)
		{
            size_t offset = Detail::Inner::TCheckOffset::GetOffsetDestructor<Type>();

			return CreateMatchBuilder<F>(offset, 
				Detail::CallHandler::Create<F>(offset) );
		}

	};
}

}

#endif //__AMOP_MOCKOBJECTBASE_HH
