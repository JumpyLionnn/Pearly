#pragma once
#include <string>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include "Image.h"

namespace Pearly {

	class Widgets
	{
	public:
		static bool Button(const std::string& label, const glm::vec2& size);
		static bool Button(const Ref<Image>& image, const glm::vec2& size, bool round = true, bool disabled = false);

		static bool TreeNode(const void* ptr_id, ImGuiTreeNodeFlags flags, const std::string& label);

		static bool ColorEdit4(const std::string& label, glm::vec4& color, ImGuiColorEditFlags flags = 0, float labelMinWidth = 100.0f);

		template<int maxCharacters = 256>
		static bool TextInput(const std::string& label, std::string& value, ImGuiInputTextFlags flags = 0)
		{
			char buffer[maxCharacters];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), value.c_str());
			if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer), flags))
			{
				value = std::string(buffer);
				return true;
			}
			return false;
		}

		static void Vec2Controls(const std::string& label, glm::vec2& values, float speed = 0.01f, float labelMinWidth = 100.0f);
		static bool FloatControls(const std::string& label, float& value, float speed = 0.1f, float componentWidthAddition = 0.0f, float labelMinWidth = 100.0f);
		static void Checkbox(const std::string& label, bool& value, float labelMinWidth = 100.0f);

		static void Separator();
		static void BeginModal();
		static bool BeginModal(const std::string& name, bool showCloseButton = false, ImGuiWindowFlags flags = 0);
		static bool BeginModal(const std::string& name, bool* closeButton = nullptr, ImGuiWindowFlags flags = 0);

		class TextFilter
		{
		public:
			TextFilter(const std::string& label, const std::string& hint);

			bool Update(float width = 0);
			bool PassFilter(const std::string& str);
			inline bool Empty() const { return m_Buffer[0] == 0; }
		private:
			std::string m_Label;
			std::string m_Hint;
			char m_Buffer[256];
		};
	};
}