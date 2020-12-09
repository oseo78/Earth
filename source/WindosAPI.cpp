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
		wc.lpfnWndProc = WndProc; // ** ���� ����� �ݹ� �Լ��� ** //
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(0);
		wc.hIcon = LoadIcon(NULL, IDC_ICON);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		// A1 ��ſ� A2, A3 �� �ٲٰ� ��� ���ϴ��� Ȯ�ιٶ�
		wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));// A1
		//wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)); // A2
		//wc.hbrBackground = CreateSolidBrush(RGB(255, 0, 0)); // A3
		wc.lpszMenuName = NULL;
		wc.lpszClassName = TEXT("MyApp"); // **  �Ʒ� CreateWindow ù��°������ MyApp �� ���ƾ� �� ** //

		// wc �� ���ǵ� ������ �����
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

	// �����Լ��� ��ü
	LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
	{
		//int mousePosX, mousePosY;
		//switch (iMsg)
		//{
		//case WM_LBUTTONDOWN:
		//	MessageBox(hWnd, TEXT("���콺 ���� ��ư�� �������ϴ�."), TEXT("WM_LBUTTONDOWN"), MB_OK);
		//	break;
		//case WM_RBUTTONDOWN:
		//	mousePosX = LOWORD(lParam); // x��ǥ�� ��´�.
		//	mousePosY = HIWORD(lParam); // y��ǥ�� ��´�.
		//	break;
		//case WM_RBUTTONUP:
		//	mousePosX = LOWORD(lParam); // x��ǥ�� ��´�.
		//	mousePosY = HIWORD(lParam); // y��ǥ�� ��´�.
		//	break;
		//case WM_MOUSEMOVE:
		//	mousePosX = LOWORD(lParam); // x��ǥ�� ��´�.
		//	mousePosY = HIWORD(lParam); // y��ǥ�� ��´�.
		//	break;
		//case WM_MOUSEWHEEL:
		//	if ((Earth::s16)HIWORD(wParam) > 0)
		//	{// ���콺 ���� �ø� ���
		//
		//	}
		//	else
		//	{// ���콺 ���� ���� ���
		//
		//	}
		//	break;
		//case WM_DESTROY:
		//	// �����϶�� �޽����� ������, PostQuitMessage �� ȣ�������μ� ���α׷��� ���� ��Ŵ
		//	//MessageBox(hWnd, TEXT("x ��ư�� �������ϴ�."), TEXT("WM_DESTROY"), MB_OK);
		//	PostQuitMessage(0);
		//	break;
		//}

		g_pwindow->WindowProc(hWnd, iMsg, wParam, lParam);

		// �׿� ���� ���� ���� �޽����� �⺻ ���� ó����
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}

	void CWindowAPI::MyCreateWindow()
	{
		m_hWnd = CreateWindow(TEXT("MyApp"), TEXT("Sample Application"),
			WS_OVERLAPPEDWINDOW, 0, 0,
			1024, 768, NULL, NULL, NULL, NULL);

		// �����츦 ����
		// ����, ������ġ, ũ�� ���� ������
		// B1 ��ſ� B2, B3 �� �����غ��� ��� �ٲ�� �� Ȯ�� �ٶ�
		// B1
		//m_hWnd = CreateWindow(TEXT("MyApp"), TEXT("Sample Application"),
		//	WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		//	CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);

		// B2
		//HWND hWnd = CreateWindow(TEXT("MyApp"), TEXT("â �̸�"),
		//	WS_OVERLAPPEDWINDOW, 100, 100,
		//	300, 300, NULL, NULL, NULL, NULL);

		// B3
		//HWND hWnd = CreateWindow(TEXT("MyApp"), TEXT("â �̸�"),
		//	WS_POPUP, 100, 100,
		//	300, 300, NULL, NULL, NULL, NULL);

		// ��μ� �غ�� �����찡 ������
		ShowWindow(m_hWnd, SW_SHOW);
		UpdateWindow(m_hWnd);
	}

	// GetMessage �� �޽����� ������ ��ٸ���, �޽����� ������ ó���Ѵ�.
	// PeekMessage �� �޽����� ������ ó���ϰ�, �޽����� ������ �����Ѵ�.
	// ���� �������� �޽����� ���, ������ ó���ؾ� �ϱ� ������
	// GetMessage �� ����� �� ����. �޽����� ������, ���� �����
	// ������ ó�� �� �� �ֱ� ������ PeekMessage �� ����ؾ� �Ѵ�.
	// ex) �ڵ� ��� ����
	// WinMain(...)
	// {
	// ...
	//     while(true)
	//     {
	//         if(ProcessMessage()==true)
	//             break;
	//
	//         GameFrameWork(); // ���� ����
	//     }
	//  ...(����)
	// }
	// [����]
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