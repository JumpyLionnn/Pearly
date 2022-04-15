#pragma once


namespace Pearly {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};
	
	// to be defined in a client
	Application* CreateApplication();
}

