#pragma once


#ifdef PR_PLATFORM_WINDOWS

#else
	#error Pearly only supports Windows!
#endif

#define BIT(x) (1 << x)