#ifndef COMMON_H_
#define COMMON_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>

#undef ASSERT
#undef VERIFY

#include "ZTrace.h"

#ifdef NDEBUG

#define ASSERT(X)
#define VERIFY(X)	((void)(X))
#define TRACE_FN(FN)
#define TRACE(X)

#else

#define ASSERT(X)	assert(X)
#define VERIFY(X)	assert(X)
#define TRACE_FN(FN) ZTrace _fn_(#FN)
#define TRACE(X)    ZTrace::out(X)

#endif

#endif
