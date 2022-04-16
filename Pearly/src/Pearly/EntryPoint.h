#pragma once
#include "Application.h"
#include "Log.h"


#ifdef PR_PLATFORM_WINDOWS

extern Pearly::Application* Pearly::CreateApplication();

int main(int argc, char** argv)
{
	Pearly::Log::Init();
	PR_CORE_WARN("Initialized log!");

	Pearly::Application* application = Pearly::CreateApplication();
	application->Run();
	delete application;
}
#endif