#pragma once

#include "Pearly/Core.h"

namespace Pearly {


	enum class EventType 
	{
		None,

		WindowClose,
		WindowResize,
		WindowFocus,
		WindowBlur, // lost focus

		KeyPressed,
		KeyReleased,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,
		MouseScrolled
	};

	enum class EventCategory
	{
		None,
		Window		= BIT(0),
		Input		= BIT(1),
		Keyboard	= BIT(2),
		Mouse		= BIT(3),
		MouseButton	= BIT(4),
	};

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
									virtual EventType GetEventType() const override { return GetStaticType(); }\
									virtual const char* GetName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
		friend class EventDispacher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & (int)category;
		}

	public:
		bool Handled = false;
	};

	class EventDispacher 
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispacher(Event& event)
			: m_Event(event)
		{}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}



	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}