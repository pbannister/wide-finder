#include "common.h"
#include "ZString.h"

#ifndef ASSERT
#include <assert.h>
#define ASSERT(X) assert(X)
#endif

//
//  Out-of-line string methods and free list maintenance.
//

enum {
    STRING_BUFFER_SIZE  = 254,
    BOB_SENTINEL        = 0x66,
    EOB_SENTINEL        = 0x99
};

static void* g_pFreeList = 0;

#ifdef NDEBUG
#define CheckBuffer(p,n)
#else
inline static void CheckBuffer(const char* pBuffer,int cbBuffer = STRING_BUFFER_SIZE) 
{
	ASSERT(pBuffer);
    ASSERT(BOB_SENTINEL == (255&*(pBuffer-1)));
    ASSERT(EOB_SENTINEL == (255&*(pBuffer+cbBuffer)));
}
#endif

void ZString::recycle()
{
	//fprintf(stderr,"recycle 0x%08lx size %d\n",(long)sBuffer,cbBufferMax);
	CheckBuffer(sBuffer,cbBufferMax);
	char* p = sBuffer - 1;
    sBuffer = 0;
    cbBufferMax = 0;
    if (STRING_BUFFER_SIZE == cbBufferMax) {
    	fprintf(stderr,"recycle 0x%08lx\n",(long)p);
        *((void**)p) = g_pFreeList;
        g_pFreeList = p;
    } else {
        delete p;
    }
}

void ZString::upsizeTo(int n)
{
	if (n < STRING_BUFFER_SIZE) {
	    cbBufferMax = STRING_BUFFER_SIZE;
	    // Grab a buffer from the free list if present (the usual case).
	    if (g_pFreeList) {
	        sBuffer = (char*) g_pFreeList;
	        g_pFreeList = *((void**)g_pFreeList);
	    } else {
	        // Allocate a stock-sized buffer.
	        sBuffer = new char[2+STRING_BUFFER_SIZE];
	        *sBuffer++                      = (char) BOB_SENTINEL;
	        *(sBuffer+STRING_BUFFER_SIZE)   = (char) EOB_SENTINEL;
	    }
	    *sBuffer = 0;
	} else {
        // round up to a quanta allowing for NUL
		++n;
        n = ((n + STRING_BUFFER_SIZE) / STRING_BUFFER_SIZE) * STRING_BUFFER_SIZE;
        char* p = new char[n+2];
        p[0]    = (char) BOB_SENTINEL;
        p[1+n]  = (char) EOB_SENTINEL;
        ++p;
        ::strcpy(p,sBuffer);
        recycle();
        sBuffer = p;
        cbBufferMax = n;
	}
	//fprintf(stderr,"upsizeTo returns 0x%08lx size %d\n",(long)sBuffer,cbBufferMax);
}
