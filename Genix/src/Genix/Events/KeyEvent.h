#pragma once
#include "Event.h"
#include "Genix/Input/Input.h"

class KeyEvent : public Event
{
public:
	KeyCode  GetKeyCode() const { return m_KeyCode; }

	EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input)
	
protected:
	KeyEvent(KeyCode  keycode) : m_KeyCode(keycode) {}

	KeyCode  m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(KeyCode  keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

	int GetRepeatCount() const { return m_RepeatCount; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)
	
private:
	int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(KeyCode  keycode) : KeyEvent(keycode) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(KeyCode  keycode) : KeyEvent(keycode) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyTyped)
};
