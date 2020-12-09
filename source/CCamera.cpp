#include "CCamera.h"
#include "../include/EarthTypes.h"
#include "../include/matrix4.h"
#include "../include/quaternion.h"

namespace Earth
{
	CCamera::CCamera()
	{
		//m_vEyePt.x = 3.0f;
		//m_vEyePt.y = 3.0f;
		//m_vEyePt.z = 3.0f;

		m_vEyePt.x = 12.0f;
		m_vEyePt.y = 12.0f;
		m_vEyePt.z = 12.0f;

		//m_vLookAtPt.x = 0.0f;
		//m_vLookAtPt.y = 0.0f;
		//m_vLookAtPt.z = 0.0f;

		m_vLookAtPt.x = 4.0f;
		m_vLookAtPt.y = 0.0f;
		m_vLookAtPt.z = 4.0f;

		m_vUpVec.x = 0.0f;
		m_vUpVec.y = 1.0f;
		m_vUpVec.z = 0.0f;

		core::vector3df eye(m_vEyePt.x, m_vEyePt.y, m_vEyePt.z);
		core::vector3df look(m_vLookAtPt.x, m_vLookAtPt.y, m_vLookAtPt.z);
		f32 eye_look_distance = eye.getDistanceFrom(look);

		U.X = 1.f;
		U.Y = 0.f;
		U.Z = 0.f;

		V.X = 0.f;
		V.Y = 1.f;
		V.Z = 0.f;

		N.X = 0.f;
		N.Y = 0.f;
		N.Z = -1.f;

		P.X = 0.f;
		P.Y = 0.f;
		P.Z = 1.f * (-eye_look_distance);

		UpdateViewMatrix();
	}

	CCamera::~CCamera()
	{

	}

	//void CCamera::SetMouseRDown(int x, int y)
	//{
	//	isRPress = true;
	//	prePos.set((f32)x, (f32)y, 0.0f);
	//}

	//void CCamera::SetMouseRUp(int x, int y)
	//{
	//	isRPress = false;
	//}

	//void CCamera::SetMouseMove(int x, int y)
	//{
	//	nowPos.set((f32)x, (f32)y, 0.0f);
	//	if (isRPress) {
	//		f32 dx = 0.0f;// dx*0.005, dy*0.005f
	//	}
	//}

	void CCamera::Process(HWND hWnd)
	{
		oldPos.x = nowPos.x;
		oldPos.y = nowPos.y;
		::GetCursorPos(&nowPos);
		::ScreenToClient(hWnd, &nowPos);
	}

	void CCamera::RotateY(f32 radian)
	{
		core::quaternion qut;
		//qut.fromAngleAxis(radian, V);
		qut.fromAngleAxis(radian, core::vector3df(0.0f, 1.0f, 0.0f));

		V = qut * V;
		U = qut * U;
		N = qut * N;
		P = qut * P;

		core::vector3df look(m_vLookAtPt.x, m_vLookAtPt.y, m_vLookAtPt.z);
		core::vector3df eye = P + look;
		core::vector3df up = V;

		m_vEyePt.x = eye.X;
		m_vEyePt.y = eye.Y;
		m_vEyePt.z = eye.Z;

		m_vLookAtPt.x = look.X;
		m_vLookAtPt.y = look.Y;
		m_vLookAtPt.z = look.Z;

		m_vUpVec.x = up.X;
		m_vUpVec.y = up.Y;
		m_vUpVec.z = up.Z;

		//m_vUpVec.x = 0.0f;
		//m_vUpVec.y = 1.0f;
		//m_vUpVec.z = 0.0f;

		UpdateViewMatrix();
	}

	void CCamera::RotateXZ(f32 radian)
	{
		core::quaternion qut;
		qut.fromAngleAxis(radian, U);

		V = qut * V;
		N = qut * N;
		P = qut * P;

		core::vector3df look(m_vLookAtPt.x, m_vLookAtPt.y, m_vLookAtPt.z);
		core::vector3df eye = P + look;
		core::vector3df up = V;

		m_vEyePt.x = eye.X;
		m_vEyePt.y = eye.Y;
		m_vEyePt.z = eye.Z;

		m_vLookAtPt.x = look.X;
		m_vLookAtPt.y = look.Y;
		m_vLookAtPt.z = look.Z;

		m_vUpVec.x = up.X;
		m_vUpVec.y = up.Y;
		m_vUpVec.z = up.Z;

		//m_vUpVec.x = 0.0f;
		//m_vUpVec.y = 1.0f;
		//m_vUpVec.z = 0.0f;

		UpdateViewMatrix();
	}

	void CCamera::UpdateViewMatrix()
	{
		D3DXMatrixLookAtLH(&matView, &m_vEyePt, &m_vLookAtPt, &m_vUpVec);
	}

	void CCamera::SetViewMatrix(LPDIRECT3DDEVICE9 pd3dDevice)
	{
		pd3dDevice->SetTransform(D3DTS_VIEW, &matView);
	}

	void CCamera::WindowProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
	{
		//int mousePosX, mousePosY;
		switch (iMsg)
		{
		case WM_LBUTTONDOWN:
			LButtonDown = true;
			break;
		case WM_LBUTTONUP:
			LButtonDown = false;
			break;
		case WM_RBUTTONDOWN:
			RButtonDown = true;
			//mousePosX = LOWORD(lParam); // x좌표를 얻는다.
			//mousePosY = HIWORD(lParam); // y좌표를 얻는다.
			//SetMouseRDown(mousePosX, mousePosY);
			break;
		case WM_RBUTTONUP:
			RButtonDown = false;
			//mousePosX = LOWORD(lParam); // x좌표를 얻는다.
			//mousePosY = HIWORD(lParam); // y좌표를 얻는다.
			//SetMouseRUp(mousePosX, mousePosY);
			break;
		case WM_MBUTTONDOWN:
			MButtonDown = true;
			break;
		case WM_MBUTTONUP:
			MButtonDown = false;
			break;
		case WM_MOUSEMOVE:
			//mousePosX = LOWORD(lParam); // x좌표를 얻는다.
			//mousePosY = HIWORD(lParam); // y좌표를 얻는다.
			//SetMouseMove(mousePosX, mousePosY);
			if (wParam && MK_RBUTTON)
			{
				f32 dx = nowPos.x - oldPos.x;
				f32 dy = nowPos.y - oldPos.y;				
				this->RotateY(dx * 0.005f);
				this->RotateXZ(dy * 0.005f);				
			}
			break;
		case WM_MOUSEWHEEL:
			if ((Earth::s16)HIWORD(wParam) > 0)
			{// 마우스 휠을 올릴 경우

			}
			else
			{// 마우스 휠을 내릴 경우

			}
			break;
		}
	}
}