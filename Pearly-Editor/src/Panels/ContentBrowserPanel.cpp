#include "ContentBrowserPanel.h"
#include "Gui/Widgets.h"

#include <Pearly.h>

#include <imgui/imgui.h>

namespace Pearly {

	static const std::filesystem::path s_AssetsDirectoyPath = "assets";

	// for settings in the future
	float g_ThumbnailSize = 96.0f;
	float g_Gap = g_ThumbnailSize / 16.0f;
	float g_CellSize = g_ThumbnailSize + g_Gap;


	static const float s_HeaderHeight = 30.0f;
	static const int s_MinColumnCount = 2;
	

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetsDirectoyPath)
	{
		m_UndoFileNavgationIcon = Image::Load("Resources/Icons/LeftIcon.png");
		m_RedoFileNavgationIcon = Image::Load("Resources/Icons/RightIcon.png");

		m_DirectoryIcon = Image::Load("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Image::Load("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnRender()
	{
		ImGui::ShowDemoWindow();
		ImGui::Begin("Content Browser");

		ImVec2 startCursor = ImGui::GetCursorPos();

		if (Widgets::Button(m_UndoFileNavgationIcon, { s_HeaderHeight, s_HeaderHeight }, true, m_DirectoriesHistory.empty()))
		{
			m_DirectoriesUndoHistory.push(m_CurrentDirectory);
			m_CurrentDirectory = m_DirectoriesHistory.top();
			m_DirectoriesHistory.pop();
		}
		ImGui::SameLine();
		if (Widgets::Button(m_RedoFileNavgationIcon, { s_HeaderHeight, s_HeaderHeight }, true, m_DirectoriesUndoHistory.empty()))
		{
			m_DirectoriesHistory.push(m_CurrentDirectory);
			m_CurrentDirectory = m_DirectoriesUndoHistory.top();
			m_DirectoriesUndoHistory.pop();
		}

		ImGui::SameLine();
		
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 spacing = style.ItemSpacing;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, spacing.y));
		// centering the text verticly
		float paddingY = (s_HeaderHeight - ImGui::GetFontSize());
		ImGui::SetCursorPosY(startCursor.y + paddingY * 0.35f);

		std::filesystem::path base;
		std::filesystem::path::iterator last = --m_CurrentDirectory.end();
		for (std::filesystem::path::iterator it = m_CurrentDirectory.begin(); it != m_CurrentDirectory.end(); it++)
		{
			if (!it->empty() && it != last)
			{
				base /= *it;
				PathDirectory(base);
			}
		}
		
		std::string currentDirectoryName = m_CurrentDirectory.filename().string();
		ImGui::Text(currentDirectoryName.c_str());
		ImGui::PopStyleVar();


		static Widgets::TextFilter filter("##search", "Search...");
		static constexpr float maxFilterWidth = 200.0f;
		ImGui::SameLine();
		float avilWidth = ImGui::GetContentRegionAvail().x;
		float filterWidth = std::min(maxFilterWidth, avilWidth);
		ImGui::Dummy(ImVec2(avilWidth - filterWidth, 0));
		ImGui::SameLine();
		ImGui::SetCursorPosY(startCursor.y + paddingY * 0.2f);
		filter.Update(filterWidth);

		ImGui::SetCursorPosY(startCursor.y + s_HeaderHeight + spacing.y);

		ImGui::Separator();

		ImGui::BeginChild("##filelist");

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = std::max((int)(panelWidth / g_CellSize), s_MinColumnCount);
		ImGui::Columns(columnCount, 0, false);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
		if (filter.Empty())
		{
			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_CurrentDirectory))
			{
				EntryButton(entry);
				ImGui::NextColumn();
			}
		}
		else
		{
			for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(m_CurrentDirectory))
			{
				std::string name = entry.path().filename().string();
				if (filter.PassFilter(name))
				{
					EntryButton(entry);
					ImGui::NextColumn();
				}
				
			}
		}

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::EndChild();

		ImGui::End();
	}

	void ContentBrowserPanel::PathDirectory(const std::filesystem::path& path)
	{
		
		std::string filename = path.filename().string();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
		if (ImGui::SmallButton(filename.c_str()))
			ChangePath(path);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::SameLine();

		ImGui::Text("\\");
		ImGui::SameLine();
	}


	void ContentBrowserPanel::EntryButton(const std::filesystem::directory_entry& entry)
	{
		std::filesystem::path path = entry.path();

		std::filesystem::path filename = path.filename().string();
		std::string filenameStr = filename.string();
		float paddingBottom = ImGui::GetFontSize() * 0.5f;

		ImVec2 textSize = ImGui::CalcTextSize(filenameStr.c_str(), 0, false, g_ThumbnailSize);
		glm::vec2 itemSize = { g_ThumbnailSize, g_ThumbnailSize + textSize.y + paddingBottom };

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
				ChangePath(path);
			}
			else
			{

			}
		}


		ImVec2 cursor = ImGui::GetCursorPos();
		ImGui::SetCursorPos({ cursor.x, cursor.y - itemSize.y });
		ImGui::Image(GetIcon(entry)->GetTextureID(), { itemSize.x, itemSize.x }, { 0, 1 }, { 1, 0 });
		ImGui::TextWrapped(filenameStr.c_str());
	}

	void ContentBrowserPanel::ChangePath(const std::filesystem::path& path)
	{
		m_DirectoriesHistory.push(m_CurrentDirectory);
		m_CurrentDirectory = path;
		m_DirectoriesUndoHistory = {};
	}

	Ref<Image> ContentBrowserPanel::GetIcon(std::filesystem::directory_entry entry)
	{
		return entry.is_directory() ? m_DirectoryIcon : m_FileIcon;
	}
}