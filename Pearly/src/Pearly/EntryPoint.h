#pragma once
#include "Application.h"
#include "Log.h"


#ifdef PR_PLATFORM_WINDOWS

extern Pearly::Application* Pearly::CreateApplication();

int main(int argc, char** argv)
{
	Pearly::Log::Init();
	PR_CORE_WARN("Initialized log!");
	PR_INFO("Hello 5+5={0}", 10);

	Pearly::Application* application = Pearly::CreateApplication();
	application->Run();
	delete application;
}
#endif