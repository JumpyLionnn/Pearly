#include "prpch.h"
#include "ImGuiLayer.h"

#include "Pearly/core.h"
#include "Pearly/Application.h"
#include "Pearly/KeyCodes.h"

#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "imgui.h"

// temporary
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Pearly {

    static ImGuiKey KeyToImGuiKey(int key)
    {
        switch (key)
        {
        case PR_KEY_TAB: return ImGuiKey_Tab;
        case PR_KEY_LEFT: return ImGuiKey_LeftArrow;
        case PR_KEY_RIGHT: return ImGuiKey_RightArrow;
        case PR_KEY_UP: return ImGuiKey_UpArrow;
        case PR_KEY_DOWN: return ImGuiKey_DownArrow;
        case PR_KEY_PAGE_UP: return ImGuiKey_PageUp;
        case PR_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
        case PR_KEY_HOME: return ImGuiKey_Home;
        case PR_KEY_END: return ImGuiKey_End;
        case PR_KEY_INSERT: return ImGuiKey_Insert;
        case PR_KEY_DELETE: return ImGuiKey_Delete;
        case PR_KEY_BACKSPACE: return ImGuiKey_Backspace;
        case PR_KEY_SPACE: return ImGuiKey_Space;
        case PR_KEY_ENTER: return ImGuiKey_Enter;
        case PR_KEY_ESCAPE: return ImGuiKey_Escape;
        case PR_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
        case PR_KEY_COMMA: return ImGuiKey_Comma;
        case PR_KEY_MINUS: return ImGuiKey_Minus;
        case PR_KEY_PERIOD: return ImGuiKey_Period;
        case PR_KEY_SLASH: return ImGuiKey_Slash;
        case PR_KEY_SEMICOLON: return ImGuiKey_Semicolon;
        case PR_KEY_EQUAL: return ImGuiKey_Equal;
        case PR_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
        case PR_KEY_BACKSLASH: return ImGuiKey_Backslash;
        case PR_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
        case PR_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
        case PR_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
        case PR_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
        case PR_KEY_NUM_LOCK: return ImGuiKey_NumLock;
        case PR_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
        case PR_KEY_PAUSE: return ImGuiKey_Pause;
        case PR_KEY_KP_0: return ImGuiKey_Keypad0;
        case PR_KEY_KP_1: return ImGuiKey_Keypad1;
        case PR_KEY_KP_2: return ImGuiKey_Keypad2;
        case PR_KEY_KP_3: return ImGuiKey_Keypad3;
        case PR_KEY_KP_4: return ImGuiKey_Keypad4;
        case PR_KEY_KP_5: return ImGuiKey_Keypad5;
        case PR_KEY_KP_6: return ImGuiKey_Keypad6;
        case PR_KEY_KP_7: return ImGuiKey_Keypad7;
        case PR_KEY_KP_8: return ImGuiKey_Keypad8;
        case PR_KEY_KP_9: return ImGuiKey_Keypad9;
        case PR_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
        case PR_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
        case PR_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
        case PR_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
        case PR_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
        case PR_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
        case PR_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
        case PR_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
        case PR_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
        case PR_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
        case PR_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
        case PR_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
        case PR_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
        case PR_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
        case PR_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
        case PR_KEY_MENU: return ImGuiKey_Menu;
        case PR_KEY_0: return ImGuiKey_0;
        case PR_KEY_1: return ImGuiKey_1;
        case PR_KEY_2: return ImGuiKey_2;
        case PR_KEY_3: return ImGuiKey_3;
        case PR_KEY_4: return ImGuiKey_4;
        case PR_KEY_5: return ImGuiKey_5;
        case PR_KEY_6: return ImGuiKey_6;
        case PR_KEY_7: return ImGuiKey_7;
        case PR_KEY_8: return ImGuiKey_8;
        case PR_KEY_9: return ImGuiKey_9;
        case PR_KEY_A: return ImGuiKey_A;
        case PR_KEY_B: return ImGuiKey_B;
        case PR_KEY_C: return ImGuiKey_C;
        case PR_KEY_D: return ImGuiKey_D;
        case PR_KEY_E: return ImGuiKey_E;
        case PR_KEY_F: return ImGuiKey_F;
        case PR_KEY_G: return ImGuiKey_G;
        case PR_KEY_H: return ImGuiKey_H;
        case PR_KEY_I: return ImGuiKey_I;
        case PR_KEY_J: return ImGuiKey_J;
        case PR_KEY_K: return ImGuiKey_K;
        case PR_KEY_L: return ImGuiKey_L;
        case PR_KEY_M: return ImGuiKey_M;
        case PR_KEY_N: return ImGuiKey_N;
        case PR_KEY_O: return ImGuiKey_O;
        case PR_KEY_P: return ImGuiKey_P;
        case PR_KEY_Q: return ImGuiKey_Q;
        case PR_KEY_R: return ImGuiKey_R;
        case PR_KEY_S: return ImGuiKey_S;
        case PR_KEY_T: return ImGuiKey_T;
        case PR_KEY_U: return ImGuiKey_U;
        case PR_KEY_V: return ImGuiKey_V;
        case PR_KEY_W: return ImGuiKey_W;
        case PR_KEY_X: return ImGuiKey_X;
        case PR_KEY_Y: return ImGuiKey_Y;
        case PR_KEY_Z: return ImGuiKey_Z;
        case PR_KEY_F1: return ImGuiKey_F1;
        case PR_KEY_F2: return ImGuiKey_F2;
        case PR_KEY_F3: return ImGuiKey_F3;
        case PR_KEY_F4: return ImGuiKey_F4;
        case PR_KEY_F5: return ImGuiKey_F5;
        case PR_KEY_F6: return ImGuiKey_F6;
        case PR_KEY_F7: return ImGuiKey_F7;
        case PR_KEY_F8: return ImGuiKey_F8;
        case PR_KEY_F9: return ImGuiKey_F9;
        case PR_KEY_F10: return ImGuiKey_F10;
        case PR_KEY_F11: return ImGuiKey_F11;
        case PR_KEY_F12: return ImGuiKey_F12;
        default: return ImGuiKey_None;
        }
    }

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{}

	ImGuiLayer::~ImGuiLayer()
	{}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;


        ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f - 60.0f);
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
        EventDispacher dispacher(event);
        dispacher.Dispatch<MouseButtonPressedEvent>(PR_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressed));
        dispacher.Dispatch<MouseButtonReleasedEvent>(PR_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonRleased));
        dispacher.Dispatch<MouseMovedEvent>(PR_BIND_EVENT_FN(ImGuiLayer::OnMouseMoved));
        dispacher.Dispatch<MouseScrolledEvent>(PR_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolled));
        dispacher.Dispatch<KeyPressedEvent>(PR_BIND_EVENT_FN(ImGuiLayer::OnKeyPressed));
        dispacher.Dispatch<KeyReleasedEvent>(PR_BIND_EVENT_FN(ImGuiLayer::OnKeyReleased));
        dispacher.Dispatch<KeyTypedEvent>(PR_BIND_EVENT_FN(ImGuiLayer::OnKeyTyped));
        dispacher.Dispatch<WindowResizeEvent>(PR_BIND_EVENT_FN(ImGuiLayer::OnWindowResize));
	}

    bool ImGuiLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = true;

        return false;
    }

    bool ImGuiLayer::OnMouseButtonRleased(MouseButtonReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::OnMouseMoved(MouseMovedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.GetX(), e.GetY());

        return false;
    }

    bool ImGuiLayer::OnMouseScrolled(MouseScrolledEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += e.GetXOffest();
        io.MouseWheel += e.GetYOffset();

        return false;
    }

    bool ImGuiLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[KeyToImGuiKey(e.GetKeyCode())] = true;

        io.KeyCtrl = io.KeysDown[KeyToImGuiKey(PR_KEY_LEFT_CONTROL)] || io.KeysDown[KeyToImGuiKey(PR_KEY_RIGHT_CONTROL)];
        io.KeyShift = io.KeysDown[KeyToImGuiKey(PR_KEY_LEFT_SHIFT)] || io.KeysDown[KeyToImGuiKey(PR_KEY_RIGHT_SHIFT)];
        io.KeyAlt = io.KeysDown[KeyToImGuiKey(PR_KEY_LEFT_ALT)] || io.KeysDown[KeyToImGuiKey(PR_KEY_RIGHT_ALT)];
        io.KeySuper = io.KeysDown[KeyToImGuiKey(PR_KEY_LEFT_SUPER)] || io.KeysDown[KeyToImGuiKey(PR_KEY_RIGHT_SUPER)];

        return false;
    }

    bool ImGuiLayer::OnKeyReleased(KeyReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[KeyToImGuiKey(e.GetKeyCode())] = false;

        return false;
    }

    bool ImGuiLayer::OnKeyTyped(KeyTypedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        int keycode = e.GetKeyCode();
        if (keycode > 0 && keycode < 0x10000)
            io.AddInputCharacter((unsigned short)keycode);

        return false;
    }

    bool ImGuiLayer::OnWindowResize(WindowResizeEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, e.GetWidth(), e.GetHeight());

        return false;
    }   
}