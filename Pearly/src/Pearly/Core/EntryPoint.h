#pragma once
#include "Application.h"
#include "Log.h"


#ifdef PR_PLATFORM_WINDOWS

extern Pearly::Application* Pearly::CreateApplication();

int main(int argc, char** argv)
{
	Pearly::Log::Init();

	PR_PROFILE_BEGIN_SESSION("Startup", "PearlyProfileStartup.json");
	Pearly::Application* application = Pearly::CreateApplication();
	PR_PROFILE_END_SESSION();

	PR_PROFILE_BEGIN_SESSION("Runtime", "PearlyProfileRuntime.json");
	application->Run();
	PR_PROFILE_END_SESSION();

	PR_PROFILE_BEGIN_SESSION("Shutdown", "PearlyProfileShutdown.json");
	delete application;
	PR_PROFILE_END_SESSION();
}
#endif