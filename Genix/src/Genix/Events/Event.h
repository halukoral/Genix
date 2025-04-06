#pragma once
#include "gxpch.h"
#include "Genix/Core/Core.h"
#include "Genix/Common/PrimitiveTypes.h"

template <typename E>
struct FEnableBitmaskOperators
{
	static constexpr bool enable = false;
};

template <typename E>
std::enable_if_t<FEnableBitmaskOperators<E>::enable, E> operator|(E Lhs, E Rhs)
{
	return static_cast<E>(static_cast<std::underlying_type_t<E>>(Lhs) |
						  static_cast<std::underlying_type_t<E>>(Rhs));
}

template <typename E>
std::enable_if_t<FEnableBitmaskOperators<E>::enable, E> operator&(E Lhs, E Rhs)
{
	return static_cast<E>(static_cast<std::underlying_type_t<E>>(Lhs) &
						  static_cast<std::underlying_type_t<E>>(Rhs));
}

enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	AppTick, AppUpdate, AppRender,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum class EventCategory : uint8
{
	None = 0,
	Application    = BIT(0),
	Input          = BIT(1),
	Keyboard       = BIT(2),
	Mouse          = BIT(3),
	MouseButton    = BIT(4)
};

template <>
struct FEnableBitmaskOperators<EventCategory>
{
	static constexpr bool enable = true;
};

#define EVENT_CLASS_TYPE(type)  static  EventType   GetStaticType() { return EventType::type; }\
								virtual EventType   GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual EventCategory GetCategory() const override { return category; }

class Event
{
public:
	friend class EventDispatcher;
	virtual ~Event() = default;

	virtual EventType		GetEventType() const = 0;
	virtual EventCategory	GetCategory() const = 0;
	virtual const char*		GetName() const = 0;
	virtual std::string		ToString() const { return GetName(); }

	void SetHandled(const bool handled) { m_Handled = handled; }
	bool GetHandled() const { return m_Handled; }
	
	bool IsInCategory(EventCategory category) const
	{
		return (bool)(GetCategory() & category);
	}
		
protected:
	bool m_Handled = false;
};

class EventDispatcher
{
public:
	EventDispatcher(Event& event) : m_Event(event) { }

	// F will be deduced by the compiler
	template<typename T, typename F>
	bool Dispatch(const F& func)
	{
		if (m_Event.GetEventType() == T::GetStaticType())
		{
			m_Event.m_Handled |= func(static_cast<T&>(m_Event));
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
