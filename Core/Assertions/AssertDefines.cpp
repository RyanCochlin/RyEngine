#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "GlobalDefines.h"

//Assert failure print messages
#define _assert_fail "_assert_fail"
#define _s_assert_fail _s_assert_fail
#define _s_assert_message(expr) "Static Assert Failed: #expr"

#define _ASSERT_GLUE(a, b) a ## b
#define ASSERT_GLUE(a, b) _ASSERT_GLUE(a, b)

//Static Assert type defines
#ifdef S_ASSERT_TYPE_DIV

#define _s_assert(expr) 1 / (int)(!!(expr))

#elif S_ASSERT_TYPE_TEMPLATE

template<bool> class TStaticAssert;
template<> class TStaticAssert<true> {};

#define _s_assert(expr) sizeof(TStaticAssert<!!(expr)>)

#else

#define _s_assert(expr) 0

#endif