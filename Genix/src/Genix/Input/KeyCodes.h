﻿#pragma once

#include "Genix/Common/PrimitiveTypes.h"

typedef enum class KeyCode : uint16
{
	// From glfw3.h
	Space               = 32,
	Apostrophe          = 39, /* ' */
	Comma               = 44, /* , */
	Minus               = 45, /* - */
	Period              = 46, /* . */
	Slash               = 47, /* / */

	D0                  = 48, /* 0 */
	D1                  = 49, /* 1 */
	D2                  = 50, /* 2 */
	D3                  = 51, /* 3 */
	D4                  = 52, /* 4 */
	D5                  = 53, /* 5 */
	D6                  = 54, /* 6 */
	D7                  = 55, /* 7 */
	D8                  = 56, /* 8 */
	D9                  = 57, /* 9 */

	Semicolon           = 59, /* ; */
	Equal               = 61, /* = */

	A                   = 65,
	B                   = 66,
	C                   = 67,
	D                   = 68,
	E                   = 69,
	F                   = 70,
	G                   = 71,
	H                   = 72,
	I                   = 73,
	J                   = 74,
	K                   = 75,
	L                   = 76,
	M                   = 77,
	N                   = 78,
	O                   = 79,
	P                   = 80,
	Q                   = 81,
	R                   = 82,
	S                   = 83,
	T                   = 84,
	U                   = 85,
	V                   = 86,
	W                   = 87,
	X                   = 88,
	Y                   = 89,
	Z                   = 90,

	LeftBracket         = 91,  /* [ */
	Backslash           = 92,  /* \ */
	RightBracket        = 93,  /* ] */
	GraveAccent         = 96,  /* ` */

	World1              = 161, /* non-US #1 */
	World2              = 162, /* non-US #2 */

	/* Function keys */
	Escape              = 256,
	Enter               = 257,
	Tab                 = 258,
	Backspace           = 259,
	Insert              = 260,
	Delete              = 261,
	Right               = 262,
	Left                = 263,
	Down                = 264,
	Up                  = 265,
	PageUp              = 266,
	PageDown            = 267,
	Home                = 268,
	End                 = 269,
	CapsLock            = 280,
	ScrollLock          = 281,
	NumLock             = 282,
	PrintScreen         = 283,
	Pause               = 284,
	F1                  = 290,
	F2                  = 291,
	F3                  = 292,
	F4                  = 293,
	F5                  = 294,
	F6                  = 295,
	F7                  = 296,
	F8                  = 297,
	F9                  = 298,
	F10                 = 299,
	F11                 = 300,
	F12                 = 301,
	F13                 = 302,
	F14                 = 303,
	F15                 = 304,
	F16                 = 305,
	F17                 = 306,
	F18                 = 307,
	F19                 = 308,
	F20                 = 309,
	F21                 = 310,
	F22                 = 311,
	F23                 = 312,
	F24                 = 313,
	F25                 = 314,

	/* Keypad */
	KP0                 = 320,
	KP1                 = 321,
	KP2                 = 322,
	KP3                 = 323,
	KP4                 = 324,
	KP5                 = 325,
	KP6                 = 326,
	KP7                 = 327,
	KP8                 = 328,
	KP9                 = 329,
	KPDecimal           = 330,
	KPDivide            = 331,
	KPMultiply          = 332,
	KPSubtract          = 333,
	KPAdd               = 334,
	KPEnter             = 335,
	KPEqual             = 336,

	LeftShift           = 340,
	LeftControl         = 341,
	LeftAlt             = 342,
	LeftSuper           = 343,
	RightShift          = 344,
	RightControl        = 345,
	RightAlt            = 346,
	RightSuper          = 347,
	Menu                = 348
} Key;

inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
{
	os << static_cast<int32>(keyCode);
	return os;
}

// From glfw3.h
#define GX_KEY_SPACE           Key::Space
#define GX_KEY_APOSTROPHE      Key::Apostrophe    /* ' */
#define GX_KEY_COMMA           Key::Comma         /* , */
#define GX_KEY_MINUS           Key::Minus         /* - */
#define GX_KEY_PERIOD          Key::Period        /* . */
#define GX_KEY_SLASH           Key::Slash         /* / */
#define GX_KEY_0               Key::D0
#define GX_KEY_1               Key::D1
#define GX_KEY_2               Key::D2
#define GX_KEY_3               Key::D3
#define GX_KEY_4               Key::D4
#define GX_KEY_5               Key::D5
#define GX_KEY_6               Key::D6
#define GX_KEY_7               Key::D7
#define GX_KEY_8               Key::D8
#define GX_KEY_9               Key::D9
#define GX_KEY_SEMICOLON       Key::Semicolon     /* ; */
#define GX_KEY_EQUAL           Key::Equal         /* = */
#define GX_KEY_A               Key::A
#define GX_KEY_B               Key::B
#define GX_KEY_C               Key::C
#define GX_KEY_D               Key::D
#define GX_KEY_E               Key::E
#define GX_KEY_F               Key::F
#define GX_KEY_G               Key::G
#define GX_KEY_H               Key::H
#define GX_KEY_I               Key::I
#define GX_KEY_J               Key::J
#define GX_KEY_K               Key::K
#define GX_KEY_L               Key::L
#define GX_KEY_M               Key::M
#define GX_KEY_N               Key::N
#define GX_KEY_O               Key::O
#define GX_KEY_P               Key::P
#define GX_KEY_Q               Key::Q
#define GX_KEY_R               Key::R
#define GX_KEY_S               Key::S
#define GX_KEY_T               Key::T
#define GX_KEY_U               Key::U
#define GX_KEY_V               Key::V
#define GX_KEY_W               Key::W
#define GX_KEY_X               Key::X
#define GX_KEY_Y               Key::Y
#define GX_KEY_Z               Key::Z
#define GX_KEY_LEFT_BRACKET    Key::LeftBracket   /* [ */
#define GX_KEY_BACKSLASH       Key::Backslash     /* \ */
#define GX_KEY_RIGHT_BRACKET   Key::RightBracket  /* ] */
#define GX_KEY_GRAVE_ACCENT    Key::GraveAccent   /* ` */
#define GX_KEY_WORLD_1         Key::World1        /* non-US #1 */
#define GX_KEY_WORLD_2         Key::World2        /* non-US #2 */

/* Function keys */
#define GX_KEY_ESCAPE          Key::Escape
#define GX_KEY_ENTER           Key::Enter
#define GX_KEY_TAB             Key::Tab
#define GX_KEY_BACKSPACE       Key::Backspace
#define GX_KEY_INSERT          Key::Insert
#define GX_KEY_DELETE          Key::Delete
#define GX_KEY_RIGHT           Key::Right
#define GX_KEY_LEFT            Key::Left
#define GX_KEY_DOWN            Key::Down
#define GX_KEY_UP              Key::Up
#define GX_KEY_PAGE_UP         Key::PageUp
#define GX_KEY_PAGE_DOWN       Key::PageDown
#define GX_KEY_HOME            Key::Home
#define GX_KEY_END             Key::End
#define GX_KEY_CAPS_LOCK       Key::CapsLock
#define GX_KEY_SCROLL_LOCK     Key::ScrollLock
#define GX_KEY_NUM_LOCK        Key::NumLock
#define GX_KEY_PRINT_SCREEN    Key::PrintScreen
#define GX_KEY_PAUSE           Key::Pause
#define GX_KEY_F1              Key::F1
#define GX_KEY_F2              Key::F2
#define GX_KEY_F3              Key::F3
#define GX_KEY_F4              Key::F4
#define GX_KEY_F5              Key::F5
#define GX_KEY_F6              Key::F6
#define GX_KEY_F7              Key::F7
#define GX_KEY_F8              Key::F8
#define GX_KEY_F9              Key::F9
#define GX_KEY_F10             Key::F10
#define GX_KEY_F11             Key::F11
#define GX_KEY_F12             Key::F12
#define GX_KEY_F13             Key::F13
#define GX_KEY_F14             Key::F14
#define GX_KEY_F15             Key::F15
#define GX_KEY_F16             Key::F16
#define GX_KEY_F17             Key::F17
#define GX_KEY_F18             Key::F18
#define GX_KEY_F19             Key::F19
#define GX_KEY_F20             Key::F20
#define GX_KEY_F21             Key::F21
#define GX_KEY_F22             Key::F22
#define GX_KEY_F23             Key::F23
#define GX_KEY_F24             Key::F24
#define GX_KEY_F25             Key::F25

/* Keypad */
#define GX_KEY_KP_0            Key::KP0
#define GX_KEY_KP_1            Key::KP1
#define GX_KEY_KP_2            Key::KP2
#define GX_KEY_KP_3            Key::KP3
#define GX_KEY_KP_4            Key::KP4
#define GX_KEY_KP_5            Key::KP5
#define GX_KEY_KP_6            Key::KP6
#define GX_KEY_KP_7            Key::KP7
#define GX_KEY_KP_8            Key::KP8
#define GX_KEY_KP_9            Key::KP9
#define GX_KEY_KP_DECIMAL      Key::KPDecimal
#define GX_KEY_KP_DIVIDE       Key::KPDivide
#define GX_KEY_KP_MULTIPLY     Key::KPMultiply
#define GX_KEY_KP_SUBTRACT     Key::KPSubtract
#define GX_KEY_KP_ADD          Key::KPAdd
#define GX_KEY_KP_ENTER        Key::KPEnter
#define GX_KEY_KP_EQUAL        Key::KPEqual

#define GX_KEY_LEFT_SHIFT      Key::LeftShift
#define GX_KEY_LEFT_CONTROL    Key::LeftControl
#define GX_KEY_LEFT_ALT        Key::LeftAlt
#define GX_KEY_LEFT_SUPER      Key::LeftSuper
#define GX_KEY_RIGHT_SHIFT     Key::RightShift
#define GX_KEY_RIGHT_CONTROL   Key::RightControl
#define GX_KEY_RIGHT_ALT       Key::RightAlt
#define GX_KEY_RIGHT_SUPER     Key::RightSuper
#define GX_KEY_MENU            Key::Menu
