#pragma once

#if defined(WINDOWS)
	#define SLASH "\\"
	#include<windows.h>
	#define getcwd(x,y) GetCurrentDirectory(y,x)
#else
	#define SLASH "/"
	#include<unistd.h> // For getcwd
#endif
