#pragma once
#include "prpch.h"

#include "Pearly/Events/Event.h"

namespace Pearly {

	struct WindowProperties
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProperties(const std::string& title = "Pearly Engine", unsigned int width = 1280, unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};

		virtual void OnUpdate() const = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProperties& properties = WindowProperties());
	};
}