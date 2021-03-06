#include "prpch.h"
#include "ImGuiLayer.h"

#include "Pearly/Core/core.h"
#include "Pearly/Core/Application.h"

#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>


namespace Pearly {

    
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{}

	void ImGuiLayer::OnAttach()
	{
		PR_PROFILE_FUNCTION();
		// Setup dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // enable keyboard controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;// eanble gamepad controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;// enable docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;// enable multi-viewport / platform windows

		// setup dear imgui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// when viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = (GLFWwindow*)app.GetWindow().GetNativeWindow();
		
		//setup platform/renderer binding
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		if (m_BlockEvents)
		{
			e.Handled |= e.IsInCategory(EventCategory::Mouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategory::Keyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::OnDetach()
	{
		PR_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{
		PR_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		PR_PROFILE_FUNCTION();
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupCurrentContext);
		}
	}
}