#pragma once

#include <Windows.h>

namespace Earth
{
	class IEventReceiver
	{
	public:
		virtual ~IEventReceiver() {}
		virtual void WindowProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) = 0;
	};
}