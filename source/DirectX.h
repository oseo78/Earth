#pragma once

#include <d3d9.h>
#include <d3dx9.h>

namespace Earth
{
	class CDirectX
	{
	public:
		CDirectX();
		~CDirectX();
		HRESULT InitD3D(HWND hWnd);
		VOID Cleanup();
		VOID Render(); // 사용하지 않음
		bool BeginScene();
		void EndScene();
		LPDIRECT3DDEVICE9 GetDevice() { return m_pd3dDevice; }

	private:
		LPDIRECT3D9 m_pD3D;
		LPDIRECT3DDEVICE9 m_pd3dDevice;
	};
}
