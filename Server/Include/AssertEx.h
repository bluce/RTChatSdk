#ifndef ST_ASSERTEX_H_
#define ST_ASSERTEX_H_

#include <assert.h>

namespace stExceptions
{
	extern void AssertOut( const char * szFile , unsigned int nLine , const char * szAssertion );
};

#ifdef _DEBUG

#define stAssert( assertion ) { if( !(assertion) ) { stExceptions::AssertOut( __FILE__ , __LINE__ , #assertion ); assert(0); } }
#define ASSERT stAssert
#define stASSERT stAssert

#else

#define stAssert( assertion ) ((void)0)
#define ASSERT stAssert
#define stASSERT stAssert
#endif

#endif
