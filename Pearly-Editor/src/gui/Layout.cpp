#include "Layout.h"
#include <imgui/imgui.h>

namespace Pearly {

	void Layout::Space(float size)
	{
		ImGui::Dummy(ImVec2(0.0f, size));
	}

	void Layout::Indent(float size)
	{
		ImGui::Indent(size);
	}

	void Layout::EndIndent()
	{
		ImGui::Unindent();
	}
}