#include "Widgets.h"
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <cctype>
#include <string>

namespace Pearly {

	static float s_ItemRounding = 8.0f;
	static ImGuiTreeNodeFlags s_DefaultTreeNodeFlags = ImGuiTreeNodeFlags_SpanAvailWidth;

	bool Widgets::Button(const std::string& label, const glm::vec2& size)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, s_ItemRounding);
		bool clicked = ImGui::Button(label.c_str(), { size.x, size.y });
		ImGui::PopStyleVar();
		return clicked;
	}

	bool Widgets::Button(const Ref<Image>& image, const glm::vec2& size, bool round, bool disabled)
	{
		if (disabled)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, s_ItemRounding * round);
		bool clicked = ImGui::ImageButton(image->GetTextureID(), { size.x, size.y }, { 0.0f, 1.0f }, { 1.0f, 0.0f }, 0);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		
		if (disabled)
		{
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}
		
		return clicked;
	}

	bool Widgets::TreeNode(const void* ptr_id, ImGuiTreeNodeFlags flags, const std::string& label)
	{
		return ImGui::TreeNodeEx(ptr_id, flags | s_DefaultTreeNodeFlags, label.c_str());
	}

	bool Widgets::ColorEdit4(const std::string& label, glm::vec4& color, ImGuiColorEditFlags flags, float labelMinWidth)
	{
		ImGui::PushID(label.c_str());
		float labelWidth = std::max(labelMinWidth, ImGui::GetContentRegionAvail().x / 4.0f);
		ImGui::Columns(2, "", false);

		ImGui::SetColumnWidth(0, labelWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, s_ItemRounding);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
		bool changed = ImGui::ColorEdit4("##tColorEdit4", glm::value_ptr(color), flags);
		ImGui::PopStyleVar(2);

		ImGui::Columns(1);
		ImGui::PopID();
		return changed;
	}

	void Widgets::Vec2Controls(const std::string& label, glm::vec2& values, float speed, float labelMinWidth)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImFont* boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());
		float labelWidth = std::max(labelMinWidth, ImGui::GetContentRegionAvail().x / 4.0f);
		ImGui::Columns(2, "", false);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, s_ItemRounding);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

		ImGui::SetColumnWidth(0, labelWidth);
		ImGui::Text(label.c_str());

		ImGui::NextColumn();

		const float xSpace = 5.0f;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(xSpace, 3.0f));
		ImGuiStyle& style = ImGui::GetStyle();
		float inputsWidth = ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("XY").x - style.WindowPadding.x - xSpace;
		ImGui::PushMultiItemsWidths(2, inputsWidth);
		
		ImGui::AlignTextToFramePadding();
		ImGui::PushFont(boldFont);
		ImGui::Text("X");
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, speed);
		ImGui::PopItemWidth();

		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::Text("Y");
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, speed);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar(3);
		ImGui::Columns(1);
		ImGui::PopID();
	}
	bool Widgets::FloatControls(const std::string& label, float& value, float speed, float componentWidthAddition, float labelMinWidth)
	{
		ImGui::PushID(label.c_str());
		float labelWidth = std::max(labelMinWidth, ImGui::GetContentRegionAvail().x / 4.0f);
		ImGui::Columns(2, "", false);

		ImGui::SetColumnWidth(0, labelWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, s_ItemRounding);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool changed = ImGui::DragFloat("##Float", &value, speed);
		ImGui::PopStyleVar(2);

		ImGui::Columns(1);
		ImGui::PopID();
		return changed;
	}
	void Widgets::Checkbox(const std::string& label, bool& value, float labelMinWidth)
	{
		ImGui::PushID(label.c_str());

		float labelWidth = std::max(labelMinWidth, ImGui::GetContentRegionAvail().x / 4.0f);
		ImGui::Columns(2, "", false);
		ImGui::SetColumnWidth(0, labelWidth);

		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, s_ItemRounding);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
		ImGui::Checkbox("##Checkbox", &value);
		ImGui::PopStyleVar(2);

		ImGui::Columns(1);
		ImGui::PopID();
	}

	

	void Widgets::Separator()
	{
		ImGui::Separator();
	}

	bool Widgets::BeginModal(const std::string& name, bool showCloseButton, ImGuiWindowFlags flags)
	{
		return BeginModal(name, showCloseButton ? &showCloseButton : nullptr, flags);
	}

	bool Widgets::BeginModal(const std::string& name, bool* closeButton, ImGuiWindowFlags flags)
	{
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
		bool modalOpened = ImGui::BeginPopupModal(name.c_str(), closeButton, flags | ImGuiWindowFlags_AlwaysAutoResize  | ImGuiWindowFlags_NoSavedSettings);
		ImGui::PopStyleVar();
		return modalOpened;
	}

	char ToLowerAscii(char in) {
		if (in <= 'Z' && in >= 'A')
			return in - ('Z' - 'z');
		return in;
	}

	static std::string ToLower(const std::string& str)
	{
		std::string result;
		result.resize(str.size());
		std::transform(str.begin(), str.end(), result.begin(), ToLowerAscii);
		return result;
	}

	Widgets::TextFilter::TextFilter(const std::string& label, const std::string& hint)
		: m_Label(label), m_Hint(hint)
	{}

	bool Widgets::TextFilter::Draw()
	{
		if (!m_Label.starts_with("##"))
		{
			ImGui::Text(m_Label.c_str());
			ImGui::SameLine();
		}
		bool changed = ImGui::InputTextWithHint("##Input", m_Hint.c_str(), m_Buffer, sizeof(m_Buffer));
		return changed;
	}
	bool Widgets::TextFilter::PassFilter(const std::string& str)
	{
		return ToLower(str).find(ToLower(m_Buffer)) != std::string::npos;
	}
}