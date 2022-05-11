#include "prpch.h"
#include "Pearly/Utilities/PlatformUtilities.h"

#include "Pearly/Core/Application.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Pearly {
	std::string FileDialogs::OpenFile(const char* filter)
	{
		OPENFILENAMEA openFileName;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&openFileName, sizeof(OPENFILENAMEA));
		openFileName.lStructSize = sizeof(OPENFILENAMEA);
		openFileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		openFileName.lpstrFile = szFile;
		openFileName.nMaxFile = sizeof(szFile);
		
		openFileName.lpstrFilter = filter;
		openFileName.nFilterIndex = 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		CHAR currentDir[256] = { 0 };
		if (GetCurrentDirectoryA(256, currentDir))
			openFileName.lpstrInitialDir = currentDir;

		if (GetOpenFileNameA(&openFileName) == TRUE)
		{
			return openFileName.lpstrFile;
		}
		return std::string();
	}
	std::string FileDialogs::SaveFile(const char* filter)
	{
		OPENFILENAMEA openFileName;
		CHAR szFile[260] = { 0 };
		ZeroMemory(&openFileName, sizeof(OPENFILENAMEA));
		openFileName.lStructSize = sizeof(OPENFILENAMEA);
		openFileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
		openFileName.lpstrFile = szFile;
		openFileName.nMaxFile = sizeof(szFile);
		openFileName.lpstrFilter = filter;
		openFileName.nFilterIndex = 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		CHAR currentDir[256] = { 0 };
		if (GetCurrentDirectoryA(256, currentDir))
			openFileName.lpstrInitialDir = currentDir;

		if (GetSaveFileNameA(&openFileName) == TRUE)
		{	
			return openFileName.lpstrFile;
		}
		return std::string();
	}
}