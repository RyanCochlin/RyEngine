#pragma once

namespace RE
{
// Keycodes stolen from windows. Currently only building windows so they should be the same for easy mapping

#define RE_SHIFT		0x10
#define RE_CONTROL		0x11
#define RE_MENU			0x12
#define RE_PAUSE		0x13
#define RE_CAPITAL		0x14

#define RE_KANA			0x15
#define RE_HANGEUL		0x15
#define RE_HANGUL		0x15
#define RE_IME_ON		0x16
#define RE_JUNJA		0x17
#define RE_FINAL		0x18
#define RE_HANJA		0x19
#define RE_KANJI		0x19
#define RE_IME_OFF		0x1A

#define RE_ESCAPE		0x1B

#define RE_CONVERT		0x1C
#define RE_NONCONVERT	0x1D
#define RE_ACCEPT		0x1E
#define RE_MODECHANGE	0x1F

#define RE_SPACE		0x20
#define RE_PRIOR		0x21
#define RE_NEXT			0x22
#define RE_END			0x23
#define RE_HOME			0x24
#define RE_LEFT			0x25
#define RE_UP			0x26
#define RE_RIGHT		0x27
#define RE_DOWN			0x28
#define RE_SELECT		0x29
#define RE_PRINT		0x2A
#define RE_EXECUTE		0x2B
#define RE_SNAPSHOT		0x2C
#define RE_INSERT		0x2D
#define RE_DELETE		0x2E
#define RE_HELP			0x2F

#define RE_0			0x30
#define RE_1			0x31
#define RE_2			0x32
#define RE_3			0x33
#define RE_4			0x34
#define RE_5			0x35
#define RE_6			0x36
#define RE_7			0x37
#define RE_8			0x38
#define RE_9			0x39

#define RE_A			0x41
#define RE_B			0x42
#define RE_C			0x43
#define RE_D			0x44
#define RE_E			0x45
#define RE_F			0x46
#define RE_G			0x47
#define RE_H			0x48
#define RE_I			0x49
#define RE_J			0x4A
#define RE_K			0x4B
#define RE_L			0x4C
#define RE_M			0x4D
#define RE_N			0x4E
#define RE_O			0x4F
#define RE_P			0x50
#define RE_Q			0x51
#define RE_R			0x52
#define RE_S			0x53
#define RE_T			0x54
#define RE_U			0x55
#define RE_V			0x56
#define RE_W			0x57
#define RE_X			0x58
#define RE_Y			0x59
#define RE_Z			0x5A

#define RE_LWIN			0x5B
#define RE_RWIN			0x5C //TODO should probably change the name of these keys if this is going to be multi-platform some day
#define RE_APPS			0x5D

#define RE_NUMPAD0		0x60
#define RE_NUMPAD1		0x61
#define RE_NUMPAD2		0x62
#define RE_NUMPAD3		0x63
#define RE_NUMPAD4		0x64
#define RE_NUMPAD5		0x65
#define RE_NUMPAD6		0x66
#define RE_NUMPAD7		0x67
#define RE_NUMPAD8		0x68
#define RE_NUMPAD9		0x69
#define RE_MULTIPLY		0x6A
#define RE_ADD			0x6B
#define RE_SEPARATOR	0x6C
#define RE_SUBTRACT		0x6D
#define RE_DECIMAL		0x6E
#define RE_DIVIDE		0x6F
#define RE_F1			0x70
#define RE_F2			0x71
#define RE_F3			0x72
#define RE_F4			0x73
#define RE_F5			0x74
#define RE_F6			0x75
#define RE_F7			0x76
#define RE_F8			0x77
#define RE_F9			0x78
#define RE_F10			0x79
#define RE_F11			0x7A
#define RE_F12			0x7B
#define RE_F13			0x7C
#define RE_F14			0x7D
#define RE_F15			0x7E
#define RE_F16			0x7F
#define RE_F17			0x80
#define RE_F18			0x81
#define RE_F19			0x82
#define RE_F20			0x83
#define RE_F21			0x84
#define RE_F22			0x85
#define RE_F23			0x86
#define RE_F24			0x87


typedef unsigned char RE_KEYCODE;
}