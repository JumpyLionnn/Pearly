#include <Pearly.h>
#include <Pearly/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Pearly {

	class PearlyEditor : public Application
	{
	public:
		PearlyEditor()
			: Application("Pearly")
		{
			PushLayer(new EditorLayer());
		}
		~PearlyEditor()
		{

		}

	private:

	};

	Application* CreateApplication()
	{
		return new PearlyEditor();
	}
	
}


