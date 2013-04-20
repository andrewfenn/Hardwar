#include "MockObjectBase.h"

#include "VirtualTable.h"
#include "CheckOffsetFunc.h"
#include "CallHandler.h"
#include "Any.h"
#include "ObjectHolder.h"
#include "ReturnMatchBuilder.h"
#include "MockObjectException.h"
#include "Comparable.h"

#include <map>

namespace amop
{

namespace Detail
{

//------------------------------------------------------------------
TMockObjectBase::TMockObjectBase()
{
    mVirtualTable = Detail::TVirtualTable::CreateVirtualTable(
		this);
}

//------------------------------------------------------------------
TMockObjectBase::~TMockObjectBase()
{
	delete mVirtualTable;
}		

//------------------------------------------------------------------
void TMockObjectBase::Clear()
{
	delete mVirtualTable;
    mVirtualTable = Detail::TVirtualTable::CreateVirtualTable(
		this);
    
    mReturnDefaults.clear();
	mReturns.clear();
	mCallCounter.clear();
	mRedirects.clear();
	mExpects.clear();
	mExpectDefaults.clear();
    mExpectCallCounter.clear();
}

//------------------------------------------------------------------
void TMockObjectBase::Verify()
{
    // Check the expect call counter first.
    for( std::map<size_t, size_t>::iterator iter = mExpectCallCounter.begin()
        ; iter != mExpectCallCounter.end()
        ; ++iter )
    {
        size_t actualCount = GetCallCounter(iter->first);
        
        if( iter->second != actualCount )
        {
            throw TCallCountException(iter->second, actualCount);
        }
    }    

    // Check wills
    for( std::map<size_t, std::vector<any> >::iterator iter = mReturns.begin()
        ; iter != mReturns.end()
        ; ++iter )
    {
        size_t actualCount = GetCallCounter(iter->first);
        
        if( mReturnDefaults.count(iter->first) )
        {
            continue;
        }

        if( iter->second.size() != actualCount )
        {
            throw TCallCountException(iter->second.size() , actualCount);
        }
    }

    // Check expects
    for( std::map<size_t,  TParamMap>::iterator iter = mExpects.begin()
        ; iter != mExpects.end()
        ; ++iter )
    {
        size_t maxCount = 0;
        
        TParamMap& paramMap = iter->second;

        for( TParamMap::iterator mapIter = paramMap.begin()
            ; mapIter != paramMap.end()
            ; ++mapIter )
        {
            if(HaveExpectDefault(iter->first, mapIter->first))
            {
                continue;
            }

            maxCount = std::max(maxCount, mapIter->second.size());
        }

        size_t actualCount = GetCallCounter(iter->first);

        if(maxCount != 0 && maxCount != actualCount)
        {
            throw TCallCountException(maxCount, actualCount);
        }
    }    
}

//------------------------------------------------------------------
void TMockObjectBase::AddCallCounter(size_t idx)
{
	mCallCounter[idx]++;
}

//------------------------------------------------------------------
size_t TMockObjectBase::GetCallCounter(size_t idx)
{
	return mCallCounter[idx];
}

//------------------------------------------------------------------
void TMockObjectBase::SetExpectCallCounter(size_t idx, size_t c)
{
    mExpectCallCounter[idx] = c;
}

//------------------------------------------------------------------
void TMockObjectBase::SetReturnDefault(size_t idx, const any& result)
{
	mReturnDefaults[idx] = result;
}

//------------------------------------------------------------------
void TMockObjectBase::SetReturn(size_t idx, const any& result)
{
	mReturns[idx].push_back(result);
}

//------------------------------------------------------------------
void TMockObjectBase::SetRedirect(size_t idx, const any& result)
{
	mRedirects[idx] = result;
}

//------------------------------------------------------------------
any& TMockObjectBase::GetReturn(size_t idx)
{
	size_t callCounter = mCallCounter[idx] - 1;
    
	if( mReturns.count(idx) && 
		callCounter < mReturns[idx].size())
	{
		return (mReturns[idx])[callCounter] ;
	}	
	
    if(!mReturnDefaults.count(idx))
    {
        size_t expect = mReturns.count(idx) ? mReturns[idx].size() : 0;
        
        throw TCallCountException(expect, callCounter);
    }

    return mReturnDefaults[idx];
}

//------------------------------------------------------------------
any& TMockObjectBase::GetRedirect(size_t idx)
{
	return mRedirects[idx];
}


//------------------------------------------------------------------
bool TMockObjectBase::HaveExpectDefault(size_t idx, size_t paramId)
{
    if(!mExpectDefaults.count(idx))
    {
        return false;
    }

    if(!mExpectDefaults[idx].count(paramId))
    {
        return false;
    }

    return true;    
}

//------------------------------------------------------------------
bool TMockObjectBase::HaveExpect(size_t idx, size_t paramId)
{
    if(!mExpects.count(idx))
    {
        return false;
    }

    if(!mExpects[idx].count(paramId))
    {
        return false;
    }

    return true;    
}

//------------------------------------------------------------------
void TMockObjectBase::SetActual(size_t idx, size_t paramId, const any& param)
{
	if(!HaveExpect(idx, paramId))
    {
        if(!HaveExpectDefault(idx, paramId))
        {
            return ;
        }
        else
        {
            if( (mExpectDefaults[idx])[paramId] == param )
            {
                return ;
            }
         
            throw TNotEqualException(paramId, (mExpectDefaults[idx])[paramId], param);
        }
    }

    // Must have expect
	TComparableList& clist = (mExpects[idx])[paramId] ;
	size_t callCounter = mCallCounter[idx];

	if( clist.size() <= callCounter )
	{
		if(!mExpectDefaults.count(idx)) return ;
		TParamDefaultMap& paramDefaultMap = mExpectDefaults[idx];
		
		if( !paramDefaultMap.count(paramId) ) return ;

		if( paramDefaultMap[paramId] != param )
			throw TNotEqualException(paramId, paramDefaultMap[paramId], param);
	}

	if( clist[callCounter] != param )
		throw TNotEqualException(paramId, clist[callCounter], param);
}

//------------------------------------------------------------------
void TMockObjectBase::SetExpectDefault(size_t idx, size_t paramId, const TComparable& param)
{
	(mExpectDefaults[idx])[paramId] = param;
}

//------------------------------------------------------------------
void TMockObjectBase::SetExpect(size_t idx, size_t paramId, const TComparable& param)
{
	(mExpects[idx])[paramId].push_back(param);
}

//------------------------------------------------------------------
void* TMockObjectBase::GetVptr()
{
	return &mVirtualTable->mVtpr;
}

}
}

