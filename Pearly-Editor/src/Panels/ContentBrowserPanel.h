#pragma once

#include <filesystem>
#include <stack>

#include "Gui/Image.h"

namespace Pearly {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnRender();
	private:
		void EntryButton(const std::filesystem::directory_entry& entry);

		Ref<Image> GetIcon(std::filesystem::directory_entry entry);
	private:
		std::filesystem::path m_CurrentDirectory;
		std::stack<std::filesystem::path> m_DirectoriesHistory;
		std::stack<std::filesystem::path> m_DirectoriesUndoHistory;

		Ref<Image> m_UndoFileNavgationIcon;
		Ref<Image> m_RedoFileNavgationIcon;

		Ref<Image> m_DirectoryIcon;
		Ref<Image> m_FileIcon;
	};

}