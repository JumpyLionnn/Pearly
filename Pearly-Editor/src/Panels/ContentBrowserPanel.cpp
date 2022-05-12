#include "ContentBrowserPanel.h"
#include "Gui/Widgets.h"

#include <imgui/imgui.h>

namespace Pearly {

	static const std::filesystem::path s_AssetsDirectoyPath = "assets";

	// for settings in the future
	float g_ThumbnailSize = 96.0f;
	float g_Gap = g_ThumbnailSize / 16.0f;
	float g_CellSize = g_ThumbnailSize + g_Gap;

	static const int s_MinColumnCount = 2;
	

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetsDirectoyPath)
	{
		m_DirectoryIcon = Image::Load("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Image::Load("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnRender()
	{
		ImGui::Begin("Content Browser");
		if (!m_DirectoriesHistory.empty())
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_DirectoriesHistory.top();
				m_DirectoriesHistory.pop();
			}
		}

		float panelWidth = ImGui::GetContentRegionAvail().x;

		int columnCount = std::max((int)(panelWidth / g_CellSize), s_MinColumnCount);

		ImGui::Columns(columnCount, 0, false);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{	
			EntryButton(entry);

			ImGui::NextColumn();
		}

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::End();
	}

	void ContentBrowserPanel::EntryButton(const std::filesystem::directory_entry& entry)
	{
		std::filesystem::path path = entry.path();

		std::filesystem::path filename = path.filename().string();
		std::string filenameStr = filename.string();
		float paddingBottom = ImGui::GetFontSize() * 0.5f;

		float textHeight = ImGui::CalcTextSize(filenameStr.c_str(), 0, false, g_ThumbnailSize).y;
		glm::vec2 itemSize = { g_ThumbnailSize, g_ThumbnailSize + textHeight + paddingBottom };


		ImGui::PushID(filenameStr.c_str());
		std::string buttonLabel = "##" + filenameStr;
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::Button(buttonLabel.c_str(), ImVec2(itemSize.x, itemSize.y));
		ImGui::PopStyleColor();
		ImGui::PopID();

		if (ImGui::BeginDragDropSource())
		{
			const wchar_t* itemPath = path.c_str();
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);


			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (entry.is_directory())
			{
				m_CurrentDirectory /= filename;
				m_DirectoriesHistory.push(path.parent_path());
			}
			else
			{

			}
		}


		ImVec2 pos = ImGui::GetCursorPos();
		ImGui::SetCursorPos({ pos.x, pos.y - itemSize.y });
		ImGui::Image(GetIcon(entry)->GetTextureID(), { itemSize.x, itemSize.x }, { 0, 1 }, { 1, 0 });
		ImGui::Indent(g_Gap);
		ImGui::TextWrapped(filenameStr.c_str());
		ImGui::Unindent();
	}

	Ref<Image> ContentBrowserPanel::GetIcon(std::filesystem::directory_entry entry)
	{
		return entry.is_directory() ? m_DirectoryIcon : m_FileIcon;
	}
}