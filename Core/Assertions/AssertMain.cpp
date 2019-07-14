//Ryan Cochlin - 7/13/2019
//Assertions Main
//TODO document

#if ENABLE_ASSERT

#define DebugBreak() __debugbreak();

#define ASSERT(expr) \
	if(expr){} \
	else \
	{ \
		ReportAssertionFail(_assert_fail, \
			#expr, \
			__FILE__, \
			__LINE__); \
		DebugBreak() \
	}

#else

#define ASSERT(expr)

#endif