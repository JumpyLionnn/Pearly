#pragma once
#include <Pearly.h>
#include "EditorCamera.h"

namespace Pearly {

	class EditorScene : public Scene
	{
	public:
		EditorScene();
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& event) override;
		void OnViewportResize(uint32 width, uint32 height) override;

		const EditorCamera& GetCamera() const { return m_Camera; }
	private:
		EditorCamera m_Camera;
	};

}