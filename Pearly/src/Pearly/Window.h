#pragma once
#include "prpch.h"

#include "Pearly/Events/Event.h"

namespace Pearly {

	struct WindowProperties
	{
		std::string Title;
		uint32 Width;
		uint32 Height;

		WindowProperties(const std::string& title = "Pearly Engine", uint32 width = 1280, uint32 height = 720)
			: Title(title), Width(width), Height(height)
		{}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};

		virtual void OnUpdate() const = 0;

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProperties& properties = WindowProperties());
	};
}