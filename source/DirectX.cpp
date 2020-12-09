#include "DirectX.h"

namespace Earth
{
	CDirectX::CDirectX()
	{
        m_pD3D = NULL;
        m_pd3dDevice = NULL;
	}

	CDirectX::~CDirectX()
	{

	}

	HRESULT CDirectX::InitD3D(HWND hWnd)
	{
        // Create the D3D object, which is needed to create the D3DDevice.
        if (NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
            return E_FAIL;

        // Set up the structure used to create the D3DDevice. Most parameters are
        // zeroed out. We set Windowed to TRUE, since we want to do D3D in a
        // window, and then set the SwapEffect to "discard", which is the most
        // efficient method of presenting the back buffer to the display.  And 
        // we request a back buffer format that matches the current desktop display 
        // format.
        D3DPRESENT_PARAMETERS d3dpp;
        ZeroMemory(&d3dpp, sizeof(d3dpp));
        d3dpp.Windowed = TRUE;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
        // new code (14)
        d3dpp.EnableAutoDepthStencil = TRUE;
        d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

        // Create the Direct3D device. Here we are using the default adapter (most
        // systems only have one, unless they have multiple graphics hardware cards
        // installed) and requesting the HAL (which is saying we want the hardware
        // device rather than a software one). Software vertex processing is 
        // specified since we know it will work on all cards. On cards that support 
        // hardware vertex processing, though, we would see a big performance gain 
        // by specifying hardware vertex processing.
        if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &d3dpp, &m_pd3dDevice)))
        {
            return E_FAIL;
        }

        // Device state would normally be set here

        return S_OK;
	}
    
    VOID CDirectX::Cleanup()
    {
        if (m_pd3dDevice != NULL)
            m_pd3dDevice->Release();

        if (m_pD3D != NULL)
            m_pD3D->Release();
    }

    VOID CDirectX::Render()
    {
        if (NULL == m_pd3dDevice)
            return;

        // Clear the backbuffer to a blue color
        m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

        // Begin the scene
        if (SUCCEEDED(m_pd3dDevice->BeginScene()))
        {
            // Rendering of scene objects can happen here

            // End the scene
            m_pd3dDevice->EndScene();
        }

        // Present the backbuffer contents to the display
        m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
    }

    bool CDirectX::BeginScene()
    {
        if (NULL == m_pd3dDevice)
            return false;

        // Clear the backbuffer to a blue color
        m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
        // new code (14)
        //m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

        // Begin the scene
        if(SUCCEEDED(m_pd3dDevice->BeginScene()))
            return true;

        return false;
    }

    void CDirectX::EndScene()
    {
        m_pd3dDevice->EndScene();
        m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
    }
}