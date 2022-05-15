#include "prpch.h"
#include "Pearly/Core/Platform.h"

#include <GLFW/glfw3.h>

namespace Pearly {

	float Platform::GetTime()
	{
		return (float)glfwGetTime();
	}

}