//Ryan Cochlin - 7/13/2019
//Assertions Main
//TODO document

#if ENABLE_ASSERT

#ifdef __cplusplus
	#if __cplusplus >= 201103L
		#define STATIC_ASSERT(expr) \
			static_assert(expr, _s_assert_message(expr))
	#else
		#define STATIC_ASSERT(expr) \
			enum \
			{ \
				ASSERT_GLUE(_s_assert_fail, __LINE__) \
				= _s_assert(expr) \
			}
	#endif
#endif

#else

#define STATIC_ASSERT(expr) 0

#endif