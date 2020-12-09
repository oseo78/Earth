#include "WindowAPI.h"
#include "../include/EarthTypes.h"
#include "CCamera.h"

namespace Earth
{
	CWindowAPI* g_pwindow = NULL;
	extern CCamera* g_pCamera;

	CWindowAPI::CWindowAPI(IEventReceiver* receiver)
	{
		g_pwindow = this;
		m_pUserReceiver = receiver;
		m_pCameraReceiver = g_pCamera;
	}

	CWindowAPI::~CWindowAPI()
	{

	}

	void CWindowAPI::HideConsolWindow()
	{
		HWND hConsole = GetConsoleWindow();
		ShowWindow(hConsole, SW_HIDE);
	}

	void CWindowAPI::MyRegisterClass()
	{
		WNDCLASS wc;		

		wc.style = CS_OWNDC;
		wc.lpfnWndProc = WndProc; // ** 위에 선언된 콜백 함수명 ** //
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(0);
		wc.hIcon = LoadIcon(NULL, IDC_ICON);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		// A1 대신에 A2, A3 로 바꾸고 어떻게 변하는지 확인바람
		wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));// A1
		//wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)); // A2
		//wc.hbrBackground = CreateSolidBrush(RGB(255, 0, 0)); // A3
		wc.lpszMenuName = NULL;
		wc.lpszClassName = TEXT("MyApp"); // **  아래 CreateWindow 첫번째인자인 MyApp 과 같아야 함 ** //

		// wc 에 정의된 성격을 등록함
		RegisterClass(&wc);
	}

	void CWindowAPI::WindowProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (iMsg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}

		if(m_pUserReceiver)
			m_pUserReceiver->WindowProc(hWnd, iMsg, wParam, lParam);

		if(m_pCameraReceiver)
			m_pCameraReceiver->WindowProc(hWnd, iMsg, wParam, lParam);
	}

	// 전역함수로 교체
	LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
	{
		//int mousePosX, mousePosY;
		//switch (iMsg)
		//{
		//case WM_LBUTTONDOWN:
		//	MessageBox(hWnd, TEXT("마우스 왼쪽 버튼을 눌렀습니다."), TEXT("WM_LBUTTONDOWN"), MB_OK);
		//	break;
		//case WM_RBUTTONDOWN:
		//	mousePosX = LOWORD(lParam); // x좌표를 얻는다.
		//	mousePosY = HIWORD(lParam); // y좌표를 얻는다.
		//	break;
		//case WM_RBUTTONUP:
		//	mousePosX = LOWORD(lParam); // x좌표를 얻는다.
		//	mousePosY = HIWORD(lParam); // y좌표를 얻는다.
		//	break;
		//case WM_MOUSEMOVE:
		//	mousePosX = LOWORD(lParam); // x좌표를 얻는다.
		//	mousePosY = HIWORD(lParam); // y좌표를 얻는다.
		//	break;
		//case WM_MOUSEWHEEL:
		//	if ((Earth::s16)HIWORD(wParam) > 0)
		//	{// 마우스 휠을 올릴 경우
		//
		//	}
		//	else
		//	{// 마우스 휠을 내릴 경우
		//
		//	}
		//	break;
		//case WM_DESTROY:
		//	// 종료하라는 메시지를 받으면, PostQuitMessage 를 호출함으로서 프로그램을 종료 시킴
		//	//MessageBox(hWnd, TEXT("x 버튼을 눌렀습니다."), TEXT("WM_DESTROY"), MB_OK);
		//	PostQuitMessage(0);
		//	break;
		//}

		g_pwindow->WindowProc(hWnd, iMsg, wParam, lParam);

		// 그외 정의 되지 않은 메시지는 기본 으로 처리함
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}

	void CWindowAPI::MyCreateWindow()
	{
		m_hWnd = CreateWindow(TEXT("MyApp"), TEXT("Sample Application"),
			WS_OVERLAPPEDWINDOW, 0, 0,
			1024, 768, NULL, NULL, NULL, NULL);

		// 윈도우를 만듬
		// 형태, 시작위치, 크기 등을 지정함
		// B1 대신에 B2, B3 로 변경해보고 어떻게 바뀌는 지 확인 바람
		// B1
		//m_hWnd = CreateWindow(TEXT("MyApp"), TEXT("Sample Application"),
		//	WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		//	CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);

		// B2
		//HWND hWnd = CreateWindow(TEXT("MyApp"), TEXT("창 이름"),
		//	WS_OVERLAPPEDWINDOW, 100, 100,
		//	300, 300, NULL, NULL, NULL, NULL);

		// B3
		//HWND hWnd = CreateWindow(TEXT("MyApp"), TEXT("창 이름"),
		//	WS_POPUP, 100, 100,
		//	300, 300, NULL, NULL, NULL, NULL);

		// 비로소 준비된 윈도우가 보여짐
		ShowWindow(m_hWnd, SW_SHOW);
		UpdateWindow(m_hWnd);
	}

	// GetMessage 는 메시지가 없으면 기다리고, 메시지가 있으면 처리한다.
	// PeekMessage 는 메시지가 있으면 처리하고, 메시지가 없으면 리턴한다.
	// 게임 구현에서 메시지가 없어도, 게임을 처리해야 하기 때문에
	// GetMessage 는 사용할 수 없다. 메시지가 없을때, 리턴 해줘야
	// 게임을 처리 할 수 있기 때문에 PeekMessage 를 사용해야 한다.
	// ex) 코드 사용 예시
	// WinMain(...)
	// {
	// ...
	//     while(true)
	//     {
	//         if(ProcessMessage()==true)
	//             break;
	//
	//         GameFrameWork(); // 게임 구현
	//     }
	//  ...(종료)
	// }
	// [참고]
	// https://adolys.tistory.com/entry/PeekMessage-GetMessage-TranslateMessage-DispatchMessage
	bool CWindowAPI::ProcessMessage()
	{
		MSG msg;
		bool bGotMsg;
		while (true)
		{
			bGotMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);
			if (bGotMsg==false)
			{
				return false;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (WM_QUIT == msg.message)
			{
				return true;
			}
		}

		return false;
	}

	bool CWindowAPI::Initialize() {
		HideConsolWindow();
		MyRegisterClass();
		MyCreateWindow();
		return true;
	}
}