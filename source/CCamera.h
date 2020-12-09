#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "IEventReceiver.h"
#include "../include/vector3d.h"

namespace Earth
{
	class CCamera : public IEventReceiver
	{
	public:
		CCamera();
		~CCamera();		

		void WindowProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) override;

		void SetViewMatrix(LPDIRECT3DDEVICE9 pd3dDevice);
		void Process(HWND hWnd);

	private:		

		//void SetMouseRDown(int x, int y);
		//void SetMouseRUp(int x, int y);
		//void SetMouseMove(int x, int y);
		void UpdateViewMatrix();

		void RotateY(f32 radian);
		void RotateXZ(f32 radian);

		bool LButtonDown;
		bool RButtonDown;
		bool MButtonDown;
		POINT oldPos;
		POINT nowPos;

		D3DXVECTOR3 m_vEyePt;
		D3DXVECTOR3 m_vLookAtPt;
		D3DXVECTOR3 m_vUpVec;

		D3DXMATRIXA16 matView;

		core::vector3df U; // 오른쪽 방향
		core::vector3df V; // 위쪽 방향
		core::vector3df N; // 보는 방향
		core::vector3df P; // 카메라 위치(from lookPt)
	};
}