#pragma once
#include "Pearly/Core/Layer.h"

namespace Pearly {
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnEvent(Event& event) override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();

		void SetBlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
	};
}


