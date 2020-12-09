#pragma once

#include <Windows.h>
#include "IEventReceiver.h"

namespace Earth {

	LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	class CWindowAPI {

	public:
		CWindowAPI(IEventReceiver* receiver);
		~CWindowAPI();

		void WindowProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

		bool Initialize();
		bool ProcessMessage();
		HWND GetHwnd() { return m_hWnd; }

	private:		
		void HideConsolWindow();
		void MyRegisterClass();
		void MyCreateWindow();
		HWND m_hWnd = NULL;
		IEventReceiver* m_pUserReceiver;
		IEventReceiver* m_pCameraReceiver;
	};
}