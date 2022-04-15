#include "prpch.h"
#include "Application.h"

#include "Pearly/Events/WindowEvents.h"
#include "Pearly/Log.h"

namespace Pearly {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		PR_TRACE(e);
		while (true)
		{
			
		}
	}
}