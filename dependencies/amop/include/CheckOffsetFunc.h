#ifndef __AMOP_CHECKOFFSETFUNC_HH
#define __AMOP_CHECKOFFSETFUNC_HH

#include "MockObjectException.h"
#include "Config.h"

#ifdef _MSC_VER
#pragma warning(disable:4505)
#endif

namespace amop
{

namespace Detail
{

//------------------------------------------------------------------
namespace Inner
{
    class TCheck 
    {
    public:
        static bool mIsCheckCall;
        static size_t mDestructorOffset;

        typedef bool* TCheckType;        
    };

    namespace TCheckOffset
    {
        //------------------------------------------------------------------
        typedef int (TCheck::*TCheckFuncPtr) (TCheck::TCheckType&);

        //------------------------------------------------------------------
        template<int I>
        class TCheckIdx
        {
        public:
            int Func( TCheck::TCheckType& checkCode) 
            { 
                checkCode = &TCheck::mIsCheckCall; 
                return I; 
            }

            void Destructor( void* )
            {
                TCheck::mDestructorOffset = I;
            }
        };

        //------------------------------------------------------------------
        template<int I>
        static TFunctionAddress GetIndexFuncRecur(size_t i)
        {
            typedef int (TCheckIdx<I>::*TCheckFuncPtr)(TCheck::TCheckType&);

            TCheckFuncPtr _ptr = &TCheckIdx<I>::Func;
            TFunctionAddress p = HorribleCast<void*>(_ptr);

            if(i == I)
                return p;
            else
                return GetIndexFuncRecur<I+1>(i);
        }

        //------------------------------------------------------------------
        template<int I>
        static TFunctionAddress GetIndexDestructorRecur(size_t i)
        {
            typedef void (TCheckIdx<I>::*TCheckFuncPtr)(void*);

            TCheckFuncPtr _ptr = &TCheckIdx<I>::Destructor;
            TFunctionAddress p = HorribleCast<void*>(_ptr);

            if(i == I)
                return p;
            else
                return GetIndexDestructorRecur<I+1>(i);
        }

	//------------------------------------------------------------------
        static TFunctionAddress GetIndexFunc(size_t i)
        {
            return GetIndexFuncRecur<0>(i);
        }

        //------------------------------------------------------------------
        static TFunctionAddress GetIndexDestructor(size_t i)
        {
            return GetIndexDestructorRecur<0>(i);
        }

        //------------------------------------------------------------------
        static TCheck* CreateCheckObject()
        {
            static TFunctionAddress vtable[MAX_NUM_VIRTUAL_FUNCTIONS];
            static TFunctionAddress vtbl = &vtable[0];

            for(size_t i = 0 ; i < MAX_NUM_VIRTUAL_FUNCTIONS; ++i)
                vtable[i] = Inner::TCheckOffset::GetIndexFunc(i);

            return (TCheck*)&vtbl;
        }

        //------------------------------------------------------------------
        static TCheck* CreateCheckDestroyObject()
        {
            static TFunctionAddress vtable[MAX_NUM_VIRTUAL_FUNCTIONS];
            static TFunctionAddress vtbl = &vtable[0];

            for(size_t i = 0 ; i < MAX_NUM_VIRTUAL_FUNCTIONS; ++i)
                vtable[i] = Inner::TCheckOffset::GetIndexDestructor(i);

            return (TCheck*)&vtbl;
        }

        //------------------------------------------------------------------
        template <class T>
        static size_t GetOffset(T memFuncPtr)
        {
            Inner::TCheckOffset::TCheckFuncPtr check = 
                reinterpret_cast<Inner::TCheckOffset::TCheckFuncPtr>(memFuncPtr);

            TCheck::mIsCheckCall = false; 
            TCheck* checkObject = CreateCheckObject();

            TCheck::TCheckType checkCode = 0;
            size_t offset = (checkObject->*check)(checkCode);	

            if(&TCheck::mIsCheckCall != checkCode)
            {
                throw TNotPureVirtualException();
            }

            return offset;
        }

        //------------------------------------------------------------------
        template <class T>
        static size_t GetOffsetDestructor()
        {
            TCheck::mDestructorOffset = 0xFFFFFFFF;
            
            TCheck* checkObject = CreateCheckDestroyObject();

            // Force delete it..
            delete (T*)checkObject;

            if(0xFFFFFFFF == TCheck::mDestructorOffset)
            {
                throw TNotPureVirtualException();
            }

            return TCheck::mDestructorOffset;
        }
            

    	//------------------------------------------------------------------
    	template<>
    	inline TFunctionAddress GetIndexFuncRecur<MAX_NUM_VIRTUAL_FUNCTIONS>(size_t)
    	{
        	return 0;
    	}
	
    	//------------------------------------------------------------------
    	template<>
    	inline TFunctionAddress GetIndexDestructorRecur<MAX_NUM_VIRTUAL_FUNCTIONS>(size_t)
    	{
        	return 0;
    	}        
    };

}

}

}

#endif //__AMOP_CHECKOFFSETFUNC_HH

