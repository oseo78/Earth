#include "TestVertex1.h"

// for timeGetTime ///////////////
#include <Windows.h>
#pragma comment(lib, "winmm.lib")
/// //////////////////////////////


namespace Earth
{
	/// <summary>
	/// 2D 삼각형
	/// </summary>
	CTriangle2D::CTriangle2D()
	{
		m_pVB = NULL;
	}

	CTriangle2D::~CTriangle2D()
	{
		if (m_pVB)
			m_pVB->Release();
	}

	bool CTriangle2D::InitVertexBuffer(LPDIRECT3DDEVICE9 pd3dDevice)
	{
		m_pd3dDevice = pd3dDevice;

		vertices[0].x = 150.0f;
		vertices[0].y = 50.0f;
		vertices[0].z = 0.5f;
		vertices[0].rhw = 1.0f;
		vertices[0].color = 0xffff0000;

		vertices[1].x = 250.0f;
		vertices[1].y = 250.0f;
		vertices[1].z = 0.5f;
		vertices[1].rhw = 1.0f;
		vertices[1].color = 0xff00ff00;

		vertices[2].x = 50.0f;
		vertices[2].y = 250.0f;
		vertices[2].z = 0.5f;
		vertices[2].rhw = 1.0f;
		vertices[2].color = 0xff00ffff;

		if (FAILED(m_pd3dDevice->CreateVertexBuffer(3 * sizeof(SVertex2D), 0,
			SVertex2D::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL))) {
			return false;
		}

		VOID* pVertices;
		if (FAILED(m_pVB->Lock(0, 3 * sizeof(SVertex2D), (void**)&pVertices, 0)))
			return false;

		memcpy(pVertices, vertices, 3 * sizeof(SVertex2D));

		m_pVB->Unlock();

		return true;
	}

	void CTriangle2D::Render()
	{
		m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SVertex2D));
		m_pd3dDevice->SetFVF(SVertex2D::FVF);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
	}

	/// <summary>
	/// 3D 삼각형
	/// </summary>
	CTriangle3D::CTriangle3D()
	{
		m_pVB = NULL;
		m_pd3dDevice = NULL;		
	}

	CTriangle3D::~CTriangle3D()
	{
//		if (m_pVB != NULL)
//			m_pVB->Release();

//		if (m_pd3dDevice != NULL)
//			m_pd3dDevice->Release();
		SAFE_RELEASE(m_pVB);
		//SAFE_RELEASE(m_pd3dDevice);
	}

	bool CTriangle3D::InitGeometry(LPDIRECT3DDEVICE9 pd3dDevice)
	{
		m_pd3dDevice = pd3dDevice;

		SVertex3D g_Vertices[] =
		{
			{ -1.0f,-1.0f, 0.0f, 0xffff0000, },
			{  1.0f,-1.0f, 0.0f, 0xff0000ff, },
			{  0.0f, 1.0f, 0.0f, 0xffffffff, },
		};

		// 버택스 버퍼를 만듭니다.
		if (FAILED(m_pd3dDevice->CreateVertexBuffer(3 * sizeof(SVertex3D),
			0, SVertex3D::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		{
			return false;
		}

		// 버택스 버퍼에 값을 채웁니다.
		VOID* pVertices;
		if (FAILED(m_pVB->Lock(0, sizeof(g_Vertices), (void**)&pVertices, 0)))
			return false;
		memcpy(pVertices, g_Vertices, sizeof(g_Vertices));
		m_pVB->Unlock();

		return true;
	}
	 
	//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
	VOID CTriangle3D::SetupMatrices()
	{
		// 1. 월드 매트릭스 설정
		// 월드 매트릭스를 간단히 표현하면, 
		// 월드상 어디에 위치할 것인가? (positon)
		//        어떤 각도로 위치할 것인가? (rotation)
		//        어떤 크기로 위치할 것인가? (scale)
		// 여기에서는 Y 축을 기준으로 시간에 따라서 회전할 수 있도록 구성함.
		// For our world matrix, we will just rotate the object about the y-axis.
		D3DXMATRIXA16 matWorld;

		// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
		// every 1000 ms. To avoid the loss of precision inherent in very high 
		// floating point numbers, the system time is modulated by the rotation 
		// period before conversion to a radian angle.
		UINT iTime = timeGetTime() % 1000;
		FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
		D3DXMatrixRotationY(&matWorld, fAngle);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		// 2. 뷰 매트릭스 설정
		// 뷰 매트릭스를 간단히 표현하면,
		// 카메라를 어떻게 설정할 것인가?
		// 카메라는 vEyePt 에 있고, vLookatPt 를 바라보고 있으며,
		// 카메라의 위쪽은 vUpVec 이다.
		// Set up our view matrix. A view matrix can be defined given an eye point,
		// a point to lookat, and a direction for which way is up. Here, we set the
		// eye five units back along the z-axis and up three units, look at the
		// origin, and define "up" to be in the y-direction.
		D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMATRIXA16 matView;
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

		// 3. 프로젝션 매트릭스 설정
		// 프로젝션 매트릭스란?
		// 카메라의 시야각은 어떻게 되는가? (FOV) - 여기서는 180/4 즉, 45도
		// 어떤 비율로 화면에 표시 할 것인가? (Aspect)	
		// 근접(가장가까이)과 원경(가장먼곳) 은 어디인가? (zn, zf)
		// 그접미만과, 원경초과는 랜더링하지 않는다.
		// D3DXMatrixPerspectiveFovLH (pOut, fov, Aspect, zn, zf);
		// For the projection matrix, we set up a perspective transform (which
		// transforms geometry from 3D view space to 2D viewport space, with
		// a perspective divide making objects smaller in the distance). To build
		// a perpsective transform, we need the field of view (1/4 pi is common),
		// the aspect ratio, and the near and far clipping planes (which define at
		// what distances geometry should be no longer be rendered).
		D3DXMATRIXA16 matProj;
		D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	}


	void CTriangle3D::SetRenderState()
	{
		if (m_pd3dDevice == NULL)
		{
			__debugbreak();// m_pd3dDevice 가 널이면 디버그모드에서 여기가 브레이크포인트
			return;
		}

		// Turn off culling, so we see the front and back of the triangle
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// Turn off D3D lighting, since we are providing our own vertex colors
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}

	VOID CTriangle3D::Render()
	{
		m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SVertex3D));
		m_pd3dDevice->SetFVF(SVertex3D::FVF);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);
	}

	/// <summary>
	/// 원통과 빛
	/// </summary>
	CCylinder3N::CCylinder3N()
	{
		m_pd3dDevice = NULL;
		m_pVB = NULL;
	}
	
	CCylinder3N::~CCylinder3N()
	{
		SAFE_RELEASE(m_pVB);
		//SAFE_RELEASE(m_pd3dDevice);
	}
	
	bool CCylinder3N::InitGeometry(LPDIRECT3DDEVICE9 pd3dDevice)
	{
		m_pd3dDevice = pd3dDevice;

		// Create the vertex buffer.
		if (FAILED(m_pd3dDevice->CreateVertexBuffer(50 * 2 * sizeof(SVertex3N),
			0, SVertex3N::FVF, D3DPOOL_DEFAULT, &m_pVB, NULL)))
		{
			return false;
		}

		// Fill the vertex buffer. We are algorithmically generating a cylinder
		// here, including the normals, which are used for lighting.
		SVertex3N* pVertices;
		if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, 0)))
			return false;

		for (DWORD i = 0; i < 50; i++)
		{
			FLOAT theta = (2 * D3DX_PI * i) / (50 - 1);
			pVertices[2 * i + 0].position = D3DXVECTOR3(sinf(theta), -1.0f, cosf(theta));
			pVertices[2 * i + 0].normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));
			pVertices[2 * i + 1].position = D3DXVECTOR3(sinf(theta), 1.0f, cosf(theta));
			pVertices[2 * i + 1].normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));
		}
		m_pVB->Unlock();

		return true;
	}
	
	VOID CCylinder3N::SetupMatrices()
	{
		// Set up world matrix
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixRotationX(&matWorld, timeGetTime() / 500.0f);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		// Set up our view matrix. A view matrix can be defined given an eye point,
		// a point to lookat, and a direction for which way is up. Here, we set the
		// eye five units back along the z-axis and up three units, look at the
		// origin, and define "up" to be in the y-direction.
		D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMATRIXA16 matView;
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

		// For the projection matrix, we set up a perspective transform (which
		// transforms geometry from 3D view space to 2D viewport space, with
		// a perspective divide making objects smaller in the distance). To build
		// a perpsective transform, we need the field of view (1/4 pi is common),
		// the aspect ratio, and the near and far clipping planes (which define at
		// what distances geometry should be no longer be rendered).
		D3DXMATRIXA16 matProj;
		D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	}

	VOID CCylinder3N::SetupLights()
	{
		// Set up a material. The material here just has the diffuse and ambient
	// colors set to yellow. Note that only one material can be used at a time.
		D3DMATERIAL9 mtrl;
		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
		mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
		mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
		mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
		m_pd3dDevice->SetMaterial(&mtrl);

		// Set up a white, directional light, with an oscillating direction.
		// Note that many lights may be active at a time (but each one slows down
		// the rendering of our scene). However, here we are just using one. Also,
		// we need to set the D3DRS_LIGHTING renderstate to enable lighting
		D3DXVECTOR3 vecDir;
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));
		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Diffuse.r = 1.0f;
		light.Diffuse.g = 1.0f;
		light.Diffuse.b = 1.0f;
		vecDir = D3DXVECTOR3(cosf(timeGetTime() / 350.0f),
			1.0f,
			sinf(timeGetTime() / 350.0f));
		D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
		light.Range = 1000.0f;
		m_pd3dDevice->SetLight(0, &light);
		m_pd3dDevice->LightEnable(0, TRUE);
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// Finally, turn on some ambient light.
		m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
	}

	void CCylinder3N::SetRenderState()
	{
		// Turn off culling
		// 컬링모드는 앞뒷면 모두 보이게 한다.
		// D3DCULL_CW 로 바꾸어서 확인해 볼 것!(시계방향 컬링)
		// D3DCULL_CCW 로 바꾸어서 확인해 볼 것!(반시계방향 컬링)
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// Turn on the zbuffer
		// 제트버퍼를 사용한다.
		// 제트버퍼를 사용하기 때문에 Clear 할 때 제트버퍼도 지워줘야 함.
		// m_d3dDevice->Clear(0,NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0); 참고
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	}
	VOID CCylinder3N::Render()
	{
		m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SVertex3N));
		m_pd3dDevice->SetFVF(SVertex3N::FVF);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 * 50 - 2);
	}


	// 텍스쳐를 입히다
	CCylinder3DT::CCylinder3DT()
	{
		m_pd3dDevice = NULL; // Our rendering device
		m_pVB = NULL; // Buffer to hold vertices
		m_pTexture = NULL; // Our texture
	}

	CCylinder3DT::~CCylinder3DT()
	{		
		SAFE_RELEASE(m_pVB);
		SAFE_RELEASE(m_pTexture);
		//SAFE_RELEASE(m_pd3dDevice);
	}

	bool CCylinder3DT::InitGeometry(LPDIRECT3DDEVICE9 pd3dDevice)
	{
		m_pd3dDevice = pd3dDevice;
		
		// Create the vertex buffer.
		if (FAILED(m_pd3dDevice->CreateVertexBuffer(50 * 2 * sizeof(SVertex3DT),
			0, SVertex3DT::FVF,
			D3DPOOL_DEFAULT, &m_pVB, NULL)))
		{
			return false;
		}

		// Fill the vertex buffer. We are setting the tu and tv texture
		// coordinates, which range from 0.0 to 1.0
		SVertex3DT* pVertices;
		if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, 0)))
			return false;

		for (DWORD i = 0; i < 50; i++)
		{
			FLOAT theta = (2 * D3DX_PI * i) / (50 - 1);

			pVertices[2 * i + 0].position = D3DXVECTOR3(sinf(theta), -1.0f, cosf(theta));
			pVertices[2 * i + 0].color = 0xffffffff;
			pVertices[2 * i + 0].tu = ((FLOAT)i) / (50 - 1);
			pVertices[2 * i + 0].tv = 1.0f;

			pVertices[2 * i + 1].position = D3DXVECTOR3(sinf(theta), 1.0f, cosf(theta));
			pVertices[2 * i + 1].color = 0xff808080;
			pVertices[2 * i + 1].tu = ((FLOAT)i) / (50 - 1);
			pVertices[2 * i + 1].tv = 0.0f;
		}
		
		m_pVB->Unlock();

		return true;
	}

	bool CCylinder3DT::LoadTexture()
	{
		// Use D3DX to create a texture from a file based image
		if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, L"banana.bmp", &m_pTexture)))
		{
			// If texture is not in current folder, try parent folder
			if (FAILED(D3DXCreateTextureFromFile(m_pd3dDevice, L"..\\banana.bmp", &m_pTexture)))
			{
				MessageBox(NULL, L"Could not find banana.bmp", L"Textures.exe", MB_OK);
				return false;
			}
		}
		return true;
	}

	VOID CCylinder3DT::SetupMatrices()
	{
		// Set up world matrix
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixRotationX(&matWorld, timeGetTime() / 1000.0f);
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		// Set up our view matrix. A view matrix can be defined given an eye point,
		// a point to lookat, and a direction for which way is up. Here, we set the
		// eye five units back along the z-axis and up three units, look at the
		// origin, and define "up" to be in the y-direction.
		D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMATRIXA16 matView;
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

		// For the projection matrix, we set up a perspective transform (which
		// transforms geometry from 3D view space to 2D viewport space, with
		// a perspective divide making objects smaller in the distance). To build
		// a perpsective transform, we need the field of view (1/4 pi is common),
		// the aspect ratio, and the near and far clipping planes (which define at
		// what distances geometry should be no longer be rendered).
		D3DXMATRIXA16 matProj;
		D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	}

	void CCylinder3DT::SetRenderState()
	{
		// Turn off culling
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		// Turn off D3D lighting
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// Turn on the zbuffer
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	}

	void CCylinder3DT::SetTexture()
	{
		m_pd3dDevice->SetTexture(0, m_pTexture);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	}

	VOID CCylinder3DT::Render()
	{
		// Render the vertex buffer contents
		m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SVertex3DT));
		m_pd3dDevice->SetFVF(SVertex3DT::FVF);
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 * 50 - 2);
	}

	// CAxis
	CAxis::CAxis()
	{
		m_pd3dDevice = NULL; // Our rendering device
		m_pVB = NULL; // Buffer to hold vertices
	}

	CAxis::~CAxis()
	{
		SAFE_RELEASE(m_pVB);
	}

	bool CAxis::Init(LPDIRECT3DDEVICE9 pd3dDevice)
	{
		m_pd3dDevice = pd3dDevice;

		// Create the vertex buffer.
		if (FAILED(m_pd3dDevice->CreateVertexBuffer(6 * sizeof(SVertex3Db),
			0, SVertex3Db::FVF,
			D3DPOOL_DEFAULT, &m_pVB, NULL)))
		{
			return false;
		}

		// Fill the vertex buffer. We are setting the tu and tv texture
		// coordinates, which range from 0.0 to 1.0
		SVertex3Db* pVertices;
		if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, 0)))
			return false;

		pVertices[0].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVertices[0].color = 0xffff0000; 
		
		pVertices[1].position = D3DXVECTOR3(1000.0f, 0.0f, 0.0f);		
		pVertices[1].color = 0xffff0000;

		pVertices[2].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVertices[2].color = 0xff00ff00; 
		
		pVertices[3].position = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);		
		pVertices[3].color = 0xff00ff00;
		
		pVertices[4].position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVertices[4].color = 0xff0000ff; 
		
		pVertices[5].position = D3DXVECTOR3(0.0f, 0.0f, 1000.0f);		
		pVertices[5].color = 0xff0000ff;		

		m_pVB->Unlock();

		// Set up world matrix
		D3DXMATRIXA16 matWorld;
		D3DXMatrixIdentity(&matWorld);		
		m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

		// Set up our view matrix. A view matrix can be defined given an eye point,
		// a point to lookat, and a direction for which way is up. Here, we set the
		// eye five units back along the z-axis and up three units, look at the
		// origin, and define "up" to be in the y-direction.
		D3DXVECTOR3 vEyePt(3.0f, 3.0f, 3.0f);
		D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
		D3DXMATRIXA16 matView;
		D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

		// For the projection matrix, we set up a perspective transform (which
		// transforms geometry from 3D view space to 2D viewport space, with
		// a perspective divide making objects smaller in the distance). To build
		// a perpsective transform, we need the field of view (1/4 pi is common),
		// the aspect ratio, and the near and far clipping planes (which define at
		// what distances geometry should be no longer be rendered).
		D3DXMATRIXA16 matProj;
		D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f);
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

		return true;
	}

	void CAxis::Render()
	{
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

		m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SVertex3Db));
		m_pd3dDevice->SetFVF(SVertex3Db::FVF);
		m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, 0, 3);
	}

	// index buffer tutorial(cube)
	CCube::CCube()
	{
		m_pVB = NULL;
		m_pIB = NULL;
	}

	CCube::~CCube()
	{
		SAFE_RELEASE( m_pVB );
		SAFE_RELEASE( m_pIB );
	}

	bool CCube::Init(LPDIRECT3DDEVICE9 pd3dDevice)
	{
		m_pd3dDevice = pd3dDevice;
		
		// Create Vertex Buffer

		SVertex3D vertices[] =
		{
			{-1,  1,  1, 0xffff0000}, // 0
			{ 1,  1,  1, 0xffff0000}, // 1
			{ 1,  1, -1, 0xffff0000}, // 2
			{-1,  1, -1, 0xffff0000}, // 3

			{-1, -1,  1, 0xff00ff00}, // 4
			{ 1, -1,  1, 0xff00ff00}, // 5
			{ 1, -1, -1, 0xff00ff00}, // 6
			{-1, -1, -1, 0xff00ff00}, // 7
		};

		if (FAILED(m_pd3dDevice->CreateVertexBuffer(8 * sizeof(SVertex3D), 0, SVertex3D::FVF,
			D3DPOOL_DEFAULT, &m_pVB, NULL)))
		{
			return false;
		}

		VOID* pVertices;
		if (FAILED(m_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		{
			return false;
		}

		memcpy(pVertices, vertices, sizeof(vertices));
		m_pVB->Unlock();		

		// Create Index Buffer

		MyIndex indices[] =
		{
			{0,1,2}, {0,2,3}, // 윗면
			{4,6,5}, {4,7,6}, // 아랫면
			{0,3,7}, {0,7,4}, // 왼면
			{1,5,6}, {1,6,2}, // 오른면
			{3,2,6}, {3,6,7}, // 앞면
			{0,4,5}, {0,5,1}  // 뒷면
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(12 * sizeof(MyIndex), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB->Unlock();

		return true;
	}

	void CCube::Render()
	{
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); // 와이어 프레임으로 보기

		m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SVertex3D));
		m_pd3dDevice->SetFVF(SVertex3D::FVF);
		m_pd3dDevice->SetIndices(m_pIB);
		m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	}


	// CCell
	CCell::CCell()
	{
		m_pVB = NULL;

		m_pIB_lod_0 = NULL;

		m_pIB_lod_1 = NULL;
		m_pIB_lod_1_t = NULL;
		m_pIB_lod_1_b = NULL;
		m_pIB_lod_1_l = NULL;
		m_pIB_lod_1_r = NULL;
		m_pIB_lod_1_tb = NULL;
		m_pIB_lod_1_lr = NULL;
		m_pIB_lod_1_tl = NULL;
		m_pIB_lod_1_tr = NULL;
		m_pIB_lod_1_bl = NULL;
		m_pIB_lod_1_br = NULL;
		m_pIB_lod_1_tbl = NULL;
		m_pIB_lod_1_tbr = NULL;
		m_pIB_lod_1_tlr = NULL;
		m_pIB_lod_1_blr = NULL;
		m_pIB_lod_1_tblr = NULL;

		m_pIB_lod_2 = NULL;
		m_pIB_lod_2_t = NULL;
		m_pIB_lod_2_b = NULL;
		m_pIB_lod_2_l = NULL;
		m_pIB_lod_2_r = NULL;
		m_pIB_lod_2_tb = NULL;
		m_pIB_lod_2_lr = NULL;
		m_pIB_lod_2_tl = NULL;
		m_pIB_lod_2_tr = NULL;
		m_pIB_lod_2_bl = NULL;
		m_pIB_lod_2_br = NULL;
		m_pIB_lod_2_tbl = NULL;
		m_pIB_lod_2_tbr = NULL;
		m_pIB_lod_2_tlr = NULL;
		m_pIB_lod_2_blr = NULL;
		m_pIB_lod_2_tblr = NULL;

		m_pIB_lod_3 = NULL;
		m_pIB_lod_3_t = NULL;
		m_pIB_lod_3_b = NULL;
		m_pIB_lod_3_l = NULL;
		m_pIB_lod_3_r = NULL;
		m_pIB_lod_3_tb = NULL;
		m_pIB_lod_3_lr = NULL;
		m_pIB_lod_3_tl = NULL;
		m_pIB_lod_3_tr = NULL;
		m_pIB_lod_3_bl = NULL;
		m_pIB_lod_3_br = NULL;
		m_pIB_lod_3_tbl = NULL;
		m_pIB_lod_3_tbr = NULL;
		m_pIB_lod_3_tlr = NULL;
		m_pIB_lod_3_blr = NULL;
		m_pIB_lod_3_tblr = NULL;
	}
	CCell::~CCell()
	{
		SAFE_RELEASE(m_pVB);

		SAFE_RELEASE(m_pIB_lod_0);

		SAFE_RELEASE(m_pIB_lod_1);
		SAFE_RELEASE(m_pIB_lod_1_t);
		SAFE_RELEASE(m_pIB_lod_1_b);
		SAFE_RELEASE(m_pIB_lod_1_l);
		SAFE_RELEASE(m_pIB_lod_1_r);
		SAFE_RELEASE(m_pIB_lod_1_tb);
		SAFE_RELEASE(m_pIB_lod_1_lr);
		SAFE_RELEASE(m_pIB_lod_1_tl);
		SAFE_RELEASE(m_pIB_lod_1_tr);
		SAFE_RELEASE(m_pIB_lod_1_bl);
		SAFE_RELEASE(m_pIB_lod_1_br);
		SAFE_RELEASE(m_pIB_lod_1_tbl);
		SAFE_RELEASE(m_pIB_lod_1_tbr);
		SAFE_RELEASE(m_pIB_lod_1_tlr);
		SAFE_RELEASE(m_pIB_lod_1_blr);
		SAFE_RELEASE(m_pIB_lod_1_tblr);

		SAFE_RELEASE(m_pIB_lod_2);
		SAFE_RELEASE(m_pIB_lod_2_t);
		SAFE_RELEASE(m_pIB_lod_2_b);
		SAFE_RELEASE(m_pIB_lod_2_l);
		SAFE_RELEASE(m_pIB_lod_2_r);
		SAFE_RELEASE(m_pIB_lod_2_tb);
		SAFE_RELEASE(m_pIB_lod_2_lr);
		SAFE_RELEASE(m_pIB_lod_2_tl);
		SAFE_RELEASE(m_pIB_lod_2_tr);
		SAFE_RELEASE(m_pIB_lod_2_bl);
		SAFE_RELEASE(m_pIB_lod_2_br);
		SAFE_RELEASE(m_pIB_lod_2_tbl);
		SAFE_RELEASE(m_pIB_lod_2_tbr);
		SAFE_RELEASE(m_pIB_lod_2_tlr);
		SAFE_RELEASE(m_pIB_lod_2_blr);
		SAFE_RELEASE(m_pIB_lod_2_tblr);

		SAFE_RELEASE(m_pIB_lod_3);
		SAFE_RELEASE(m_pIB_lod_3_t);
		SAFE_RELEASE(m_pIB_lod_3_b);
		SAFE_RELEASE(m_pIB_lod_3_l);
		SAFE_RELEASE(m_pIB_lod_3_r);
		SAFE_RELEASE(m_pIB_lod_3_tb);
		SAFE_RELEASE(m_pIB_lod_3_lr);
		SAFE_RELEASE(m_pIB_lod_3_tl);
		SAFE_RELEASE(m_pIB_lod_3_tr);
		SAFE_RELEASE(m_pIB_lod_3_bl);
		SAFE_RELEASE(m_pIB_lod_3_br);
		SAFE_RELEASE(m_pIB_lod_3_tbl);
		SAFE_RELEASE(m_pIB_lod_3_tbr);
		SAFE_RELEASE(m_pIB_lod_3_tlr);
		SAFE_RELEASE(m_pIB_lod_3_blr);
		SAFE_RELEASE(m_pIB_lod_3_tblr);
	}
	bool CCell::Init(LPDIRECT3DDEVICE9 pd3dDevice)
	{
		m_pd3dDevice = pd3dDevice;

		// Create Vertex Buffer		

		if (FAILED(m_pd3dDevice->CreateVertexBuffer(81 * sizeof(SVertex3D), 0, SVertex3D::FVF,
			D3DPOOL_DEFAULT, &m_pVB, NULL)))
		{
			return false;
		}

		SVertex3D* pVertices;
		if (FAILED(m_pVB->Lock(0, 81 * sizeof(SVertex3D), (void**)&pVertices, 0)))
		{
			return false;
		}

		for (int z = 0; z < 9; ++z) {
			for (int x = 0; x < 9; ++x) {
				pVertices[z * 9 + x].x = x;
				pVertices[z * 9 + x].y = 0.0f;
				pVertices[z * 9 + x].z = z;
				pVertices[z * 9 + x].color = 0xffffffff;
			}
		}
		m_pVB->Unlock();

		// Create Index Buffer
		// Lod level 0

		count_lod_0 = 128;
		// 삼각형 128 개
		MyIndex indices0[] =
		{
			{	0	,	10	,	1	},
			{	0	,	9	,	10	},
			{	1	,	10	,	2	},
			{	2	,	10	,	11	},
			{	2	,	11	,	12	},
			{	2	,	12	,	3	},
			{	3	,	12	,	4	},
			{	4	,	12	,	13	},
			{	4	,	13	,	14	},
			{	5	,	4	,	14	},
			{	5	,	14	,	6	},
			{	6	,	14	,	15	},
			{	6	,	15	,	16	},
			{	6	,	16	,	7	},
			{	7	,	16	,	8	},
			{	8	,	16	,	17	},
			{	9	,	18	,	10	},
			{	10	,	18	,	19	},
			{	10	,	19	,	20	},
			{	10	,	20	,	11	},
			{	11	,	20	,	12	},
			{	12	,	20	,	21	},
			{	12	,	21	,	22	},
			{	12	,	22	,	13	},
			{	13	,	22	,	14	},
			{	14	,	22	,	23	},
			{	14	,	23	,	24	},
			{	14	,	24	,	15	},
			{	15	,	24	,	16	},
			{	16	,	24	,	25	},
			{	16	,	25	,	26	},
			{	16	,	26	,	17	},
			{	18	,	27	,	28	},
			{	18	,	28	,	19	},
			{	19	,	28	,	20	},
			{	20	,	28	,	29	},
			{	20	,	29	,	30	},
			{	20	,	30	,	21	},
			{	21	,	30	,	22	},
			{	22	,	30	,	31	},
			{	22	,	31	,	32	},
			{	22	,	32	,	23	},
			{	23	,	32	,	24	},
			{	24	,	32	,	33	},
			{	24	,	33	,	34	},
			{	24	,	34	,	25	},
			{	25	,	34	,	26	},
			{	26	,	34	,	35	},
			{	27	,	36	,	28	},
			{	28	,	36	,	37	},
			{	28	,	37	,	38	},
			{	28	,	38	,	29	},
			{	29	,	38	,	30	},
			{	30	,	38	,	39	},
			{	30	,	39	,	40	},
			{	30	,	40	,	31	},
			{	31	,	40	,	32	},
			{	32	,	40	,	41	},
			{	32	,	41	,	42	},
			{	32	,	42	,	33	},
			{	33	,	42	,	34	},
			{	34	,	42	,	43	},
			{	34	,	43	,	44	},
			{	34	,	44	,	35	},
			{	36	,	45	,	46	},
			{	36	,	46	,	37	},
			{	37	,	46	,	38	},
			{	38	,	46	,	47	},
			{	38	,	47	,	48	},
			{	38	,	48	,	39	},
			{	39	,	48	,	40	},
			{	40	,	48	,	49	},
			{	40	,	49	,	50	},
			{	40	,	50	,	41	},
			{	41	,	50	,	42	},
			{	42	,	50	,	51	},
			{	42	,	51	,	52	},
			{	42	,	52	,	43	},
			{	43	,	52	,	44	},
			{	44	,	52	,	53	},
			{	45	,	54	,	46	},
			{	46	,	54	,	55	},
			{	46	,	55	,	56	},
			{	46	,	56	,	47	},
			{	47	,	56	,	48	},
			{	48	,	56	,	57	},
			{	48	,	57	,	58	},
			{	48	,	58	,	49	},
			{	49	,	58	,	50	},
			{	50	,	58	,	59	},
			{	50	,	59	,	60	},
			{	50	,	60	,	51	},
			{	51	,	60	,	52	},
			{	52	,	60	,	61	},
			{	52	,	61	,	62	},
			{	52	,	62	,	53	},
			{	54	,	63	,	64	},
			{	54	,	64	,	55	},
			{	55	,	64	,	56	},
			{	56	,	64	,	65	},
			{	56	,	65	,	66	},
			{	56	,	66	,	57	},
			{	57	,	66	,	58	},
			{	58	,	66	,	67	},
			{	58	,	67	,	68	},
			{	58	,	68	,	59	},
			{	59	,	68	,	60	},
			{	60	,	68	,	69	},
			{	60	,	69	,	70	},
			{	60	,	70	,	61	},
			{	61	,	70	,	62	},
			{	62	,	70	,	71	},
			{	63	,	72	,	64	},
			{	64	,	72	,	73	},
			{	64	,	73	,	74	},
			{	64	,	74	,	65	},
			{	65	,	74	,	66	},
			{	66	,	74	,	75	},
			{	66	,	75	,	76	},
			{	66	,	76	,	67	},
			{	67	,	76	,	68	},
			{	68	,	76	,	77	},
			{	68	,	77	,	78	},
			{	68	,	78	,	69	},
			{	69	,	78	,	70	},
			{	70	,	78	,	79	},
			{	70	,	79	,	80	},
			{	70	,	80	,	71	}
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices0), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_0, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_0->Lock(0, sizeof(indices0), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices0, sizeof(indices0));
		m_pIB_lod_0->Unlock();

		CreateIndexBufferLodLevel1none();
		CreateIndexBufferLodLevel1t();
		CreateIndexBufferLodLevel1b();
		CreateIndexBufferLodLevel1l();
		CreateIndexBufferLodLevel1r();
		CreateIndexBufferLodLevel1tb();
		CreateIndexBufferLodLevel1lr();
		CreateIndexBufferLodLevel1tl();
		CreateIndexBufferLodLevel1tr();
		CreateIndexBufferLodLevel1bl();
		CreateIndexBufferLodLevel1br();
		CreateIndexBufferLodLevel1tbl();
		CreateIndexBufferLodLevel1tbr();
		CreateIndexBufferLodLevel1tlr();
		CreateIndexBufferLodLevel1blr();
		CreateIndexBufferLodLevel1tblr();

		CreateIndexBufferLodLevel2none();
		CreateIndexBufferLodLevel2t();
		CreateIndexBufferLodLevel2b();
		CreateIndexBufferLodLevel2l();
		CreateIndexBufferLodLevel2r();
		CreateIndexBufferLodLevel2tb();
		CreateIndexBufferLodLevel2lr();
		CreateIndexBufferLodLevel2tl();
		CreateIndexBufferLodLevel2tr();
		CreateIndexBufferLodLevel2bl();
		CreateIndexBufferLodLevel2br();
		CreateIndexBufferLodLevel2tbl();
		CreateIndexBufferLodLevel2tbr();
		CreateIndexBufferLodLevel2tlr();
		CreateIndexBufferLodLevel2blr();
		CreateIndexBufferLodLevel2tblr();

		CreateIndexBufferLodLevel3none();
		CreateIndexBufferLodLevel3t();
		CreateIndexBufferLodLevel3b();
		CreateIndexBufferLodLevel3l();
		CreateIndexBufferLodLevel3r();
		CreateIndexBufferLodLevel3tb();
		CreateIndexBufferLodLevel3lr();
		CreateIndexBufferLodLevel3tl();
		CreateIndexBufferLodLevel3tr();
		CreateIndexBufferLodLevel3bl();
		CreateIndexBufferLodLevel3br();
		CreateIndexBufferLodLevel3tbl();
		CreateIndexBufferLodLevel3tbr();
		CreateIndexBufferLodLevel3tlr();
		CreateIndexBufferLodLevel3blr();
		CreateIndexBufferLodLevel3tblr();

		return true;
	}

	bool CCell::CreateIndexBufferLodLevel1none()
	{
		// 삼각형 64개
		count_lod_1 = 64;
		MyIndex indices[] = {
		{	10, 2, 0	},
		{ 10	,	0	,	18 },
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },
		{ 12	,	4	,	2 },
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		{ 14	,	6	,	4 },
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		{ 16	,	8	,	6 },
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },
		{ 16	,	26	,	8 },
		{ 28	,	20	,	18 },
		{ 28	,	18	,	36 },
		{ 28	,	36	,	38 },
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },
		{ 34	,	44	,	26 },
		{ 46	,	38	,	36 },
		{ 46	,	36	,	54 },
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },
		{ 52	,	62	,	44 },
		{ 64	,	56	,	54 },
		{ 64	,	54	,	72 },
		{ 64	,	72	,	74 },
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		{ 66	,	74	,	76 },
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		{ 68	,	76	,	78 },
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		{ 70	,	78	,	80 },
		{ 70	,	80	,	62 }
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1t()
	{
		count_lod_1_t = 68;
		// 삼각형 64개 + 4
		MyIndex indices[] = {
		{	10, 2, 0	},
		{ 10	,	0	,	18 },
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },
		{ 12	,	4	,	2 },
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		{ 14	,	6	,	4 },
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		{ 16	,	8	,	6 },
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },
		{ 16	,	26	,	8 },
		{ 28	,	20	,	18 },
		{ 28	,	18	,	36 },
		{ 28	,	36	,	38 },///
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },
		{ 34	,	44	,	26 },
		{ 46	,	38	,	36 },
		{ 46	,	36	,	54 },
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },
		{ 52	,	62	,	44 },
		{ 64	,	56	,	54 },
		{ 64	,	54	,	72 },
		//{ 64	,	72	,	74 },//top
			{64,72,73},
			{64,73,74},
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		//{ 66	,	74	,	76 },//top
			{66,74,75},
			{66,75,76},
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		//{ 68	,	76	,	78 },//top
			{68,76,77},
			{68,77,78},
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		//{ 70	,	78	,	80 },//top
			{70,78,79},
			{70,79,80},
		{ 70	,	80	,	62 }
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_t, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_t->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_t->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1b()
	{
		count_lod_1_b = 68;
		// 삼각형 64개 + 4
		MyIndex indices[] = {
		//{	10, 2, 0	},//bot
			{10,1,0},
			{10,2,1},
		{ 10	,	0	,	18 },
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },
		//{ 12	,	4	,	2 },//bot
			{12,3,2},
			{12,4,3},
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		//{ 14	,	6	,	4 },//bot
			{14,5,4},
			{14,6,5},
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		//{ 16	,	8	,	6 },//bot
			{16,7,6},
			{16,8,7},
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },
		{ 16	,	26	,	8 },
		{ 28	,	20	,	18 },
		{ 28	,	18	,	36 },
		{ 28	,	36	,	38 },
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },
		{ 34	,	44	,	26 },
		{ 46	,	38	,	36 },
		{ 46	,	36	,	54 },
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },
		{ 52	,	62	,	44 },
		{ 64	,	56	,	54 },
		{ 64	,	54	,	72 },
		{ 64	,	72	,	74 },
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		{ 66	,	74	,	76 },
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		{ 68	,	76	,	78 },
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		{ 70	,	78	,	80 },
		{ 70	,	80	,	62 }
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_b, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_b->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_b->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1l()
	{
		count_lod_1_l = 68;
		// 삼각형 64개 + 4
		MyIndex indices[] = {
		{	10, 2, 0	},
		//{ 10	,	0	,	18 },//left
			{10,0,9},
			{10,9,18},
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },
		{ 12	,	4	,	2 },
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		{ 14	,	6	,	4 },
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		{ 16	,	8	,	6 },
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },
		{ 16	,	26	,	8 },
		{ 28	,	20	,	18 },
		//{ 28	,	18	,	36 },//left
			{28,18,27},
			{28,27,36},
		{ 28	,	36	,	38 },
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },
		{ 34	,	44	,	26 },
		{ 46	,	38	,	36 },
		//{ 46	,	36	,	54 },//left
			{46,36,45},
			{46,45,54},
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },
		{ 52	,	62	,	44 },
		{ 64	,	56	,	54 },
		//{ 64	,	54	,	72 },//left
			{64,54,63},
			{64,63,72},
		{ 64	,	72	,	74 },
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		{ 66	,	74	,	76 },
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		{ 68	,	76	,	78 },
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		{ 70	,	78	,	80 },
		{ 70	,	80	,	62 }
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_l, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_l->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_l->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1r()
	{
		count_lod_1_r = 68;
		// 삼각형 64개 + 4
		MyIndex indices[] = {
		{	10, 2, 0	},
		{ 10	,	0	,	18 },
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },
		{ 12	,	4	,	2 },
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		{ 14	,	6	,	4 },
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		{ 16	,	8	,	6 },
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },
		//{ 16	,	26	,	8 },//right
			{16,17,8},
			{16,26,17},
		{ 28	,	20	,	18 },
		{ 28	,	18	,	36 },
		{ 28	,	36	,	38 },
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },
		//{ 34	,	44	,	26 },//right
			{34,35,26},
			{34,44,35},
		{ 46	,	38	,	36 },
		{ 46	,	36	,	54 },
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },
		//{ 52	,	62	,	44 },//right
			{52,53,44},
			{52,62,53},
		{ 64	,	56	,	54 },
		{ 64	,	54	,	72 },
		{ 64	,	72	,	74 },
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		{ 66	,	74	,	76 },
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		{ 68	,	76	,	78 },
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		{ 70	,	78	,	80 },
		//{ 70	,	80	,	62 }//right
			{70,71,62},
			{70,80,71}
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_r, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_r->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_r->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1tb()
	{
		count_lod_1_tb = 72;
		// 삼각형 64개 + 4 + 4
		MyIndex indices[] = {
			//{	10, 2, 0	},//bot
			{10,1,0},
			{10,2,1},
		{ 10	,	0	,	18 },
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },
		//{ 12	,	4	,	2 },//bot
			{12,3,2},
			{12,4,3},
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		//{ 14	,	6	,	4 },//bot
			{14,5,4},
			{14,6,5},
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		//{ 16	,	8	,	6 },//bot
			{16,7,6},
			{16,8,7},		
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },
		{ 16	,	26	,	8 },
		{ 28	,	20	,	18 },
		{ 28	,	18	,	36 },
		{ 28	,	36	,	38 },
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },
		{ 34	,	44	,	26 },
		{ 46	,	38	,	36 },
		{ 46	,	36	,	54 },
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },
		{ 52	,	62	,	44 },
		{ 64	,	56	,	54 },
		{ 64	,	54	,	72 },
		//{ 64	,	72	,	74 },//top
			{64,72,73},
			{64,73,74},
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		//{ 66	,	74	,	76 },//top
			{66,74,75},
			{66,75,76},
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		//{ 68	,	76	,	78 },//top
			{68,76,77},
			{68,77,78},
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		//{ 70	,	78	,	80 },//top
			{70,78,79},
			{70,79,80},
		{ 70	,	80	,	62 }
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_tb, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_tb->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_tb->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1lr()
	{
		count_lod_1_lr = 72;
		// 삼각형 64개 + 4 + 4
		MyIndex indices[] = {
		{	10, 2, 0	},
		//{ 10	,	0	,	18 },//left
			{10,0,9},
			{10,9,18},
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },
		{ 12	,	4	,	2 },
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		{ 14	,	6	,	4 },
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		{ 16	,	8	,	6 },
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },		
		//{ 16	,	26	,	8 },//right
			{16,17,8},
			{16,26,17},
		{ 28	,	20	,	18 },
		//{ 28	,	18	,	36 },//left
			{28,18,27},
			{28,27,36},
		{ 28	,	36	,	38 },
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },		
		//{ 34	,	44	,	26 },//right
			{34,35,26},
			{34,44,35},
		{ 46	,	38	,	36 },
		//{ 46	,	36	,	54 },//left
			{46,36,45},
			{46,45,54},
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },		
		//{ 52	,	62	,	44 },//right
			{52,53,44},
			{52,62,53},
		{ 64	,	56	,	54 },
		//{ 64	,	54	,	72 },//left
			{64,54,63},
			{64,63,72},
		{ 64	,	72	,	74 },
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		{ 66	,	74	,	76 },
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		{ 68	,	76	,	78 },
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		{ 70	,	78	,	80 },		
		//{ 70	,	80	,	62 }//right
			{70,71,62},
			{70,80,71}
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_lr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_lr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_lr->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1tl()
	{
		count_lod_1_tl = 72;
		// 삼각형 64개 + 4 + 4
		MyIndex indices[] = {
		{	10, 2, 0	},		
		//{ 10	,	0	,	18 },//left
			{10,0,9},
			{10,9,18},
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },
		{ 12	,	4	,	2 },
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		{ 14	,	6	,	4 },
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		{ 16	,	8	,	6 },
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },
		{ 16	,	26	,	8 },
		{ 28	,	20	,	18 },		
		//{ 28	,	18	,	36 },//left
			{28,18,27},
			{28,27,36},
		{ 28	,	36	,	38 },
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },
		{ 34	,	44	,	26 },
		{ 46	,	38	,	36 },		
		//{ 46	,	36	,	54 },//left
			{46,36,45},
			{46,45,54},
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },
		{ 52	,	62	,	44 },
		{ 64	,	56	,	54 },		
		//{ 64	,	54	,	72 },//left
			{64,54,63},
			{64,63,72},
		//{ 64	,	72	,	74 },//top
			{64,72,73},
			{64,73,74},
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		//{ 66	,	74	,	76 },//top
			{66,74,75},
			{66,75,76},
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		//{ 68	,	76	,	78 },//top
			{68,76,77},
			{68,77,78},
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		//{ 70	,	78	,	80 },//top
			{70,78,79},
			{70,79,80},
		{ 70	,	80	,	62 }
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_tl, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_tl->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_tl->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1tr()
	{
		count_lod_1_tr = 72;
		// 삼각형 64개 + 4 + 4
		MyIndex indices[] = {
		{	10, 2, 0	},
		{ 10	,	0	,	18 },
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },
		{ 12	,	4	,	2 },
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		{ 14	,	6	,	4 },
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		{ 16	,	8	,	6 },
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },		
		//{ 16	,	26	,	8 },//right
			{16,17,8},
			{16,26,17},
		{ 28	,	20	,	18 },
		{ 28	,	18	,	36 },
		{ 28	,	36	,	38 },
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },		
		//{ 34	,	44	,	26 },//right
			{34,35,26},
			{34,44,35},
		{ 46	,	38	,	36 },
		{ 46	,	36	,	54 },
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },		
		//{ 52	,	62	,	44 },//right
			{52,53,44},
			{52,62,53},
		{ 64	,	56	,	54 },
		{ 64	,	54	,	72 },
		//{ 64	,	72	,	74 },//top
			{64,72,73},
			{64,73,74},
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		//{ 66	,	74	,	76 },//top
			{66,74,75},
			{66,75,76},
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		//{ 68	,	76	,	78 },//top
			{68,76,77},
			{68,77,78},
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		//{ 70	,	78	,	80 },//top
			{70,78,79},
			{70,79,80},		
		//{ 70	,	80	,	62 }//right
			{70,71,62},
			{70,80,71}
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_tr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_tr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_tr->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1bl()
	{
		count_lod_1_bl = 72;
		// 삼각형 64개 + 4 + 4
		MyIndex indices[] = {
			//{	10, 2, 0	},//bot
				{10,1,0},
				{10,2,1},
			//{ 10	,	0	,	18 },//left
				{10,0,9},
				{10,9,18},
			{ 10	,	18	,	20 },
			{ 10	,	20	,	2 },
			//{ 12	,	4	,	2 },//bot
				{12,3,2},
				{12,4,3},
			{ 12	,	2	,	20 },
			{ 12	,	20	,	22 },
			{ 12	,	22	,	4 },
			//{ 14	,	6	,	4 },//bot
				{14,5,4},
				{14,6,5},
			{ 14	,	4	,	22 },
			{ 14	,	22	,	24 },
			{ 14	,	24	,	6 },
			//{ 16	,	8	,	6 },//bot
				{16,7,6},
				{16,8,7},
			{ 16	,	6	,	24 },
			{ 16	,	24	,	26 },
			{ 16	,	26	,	8 },//right
			//	{16,17,8},
			//	{16,26,17},
			{ 28	,	20	,	18 },
			//{ 28	,	18	,	36 },//left
				{28,18,27},
				{28,27,36},
			{ 28	,	36	,	38 },
			{ 28	,	38	,	20 },
			{ 30	,	22	,	20 },
			{ 30	,	20	,	38 },
			{ 30	,	38	,	40 },
			{ 30	,	40	,	22 },
			{ 32	,	24	,	22 },
			{ 32	,	22	,	40 },
			{ 32	,	40	,	42 },
			{ 32	,	42	,	24 },
			{ 34	,	26	,	24 },
			{ 34	,	24	,	42 },
			{ 34	,	42	,	44 },
			{ 34	,	44	,	26 },//right
			//	{34,35,26},
			//	{34,44,35},
			{ 46	,	38	,	36 },
			//{ 46	,	36	,	54 },//left
				{46,36,45},
				{46,45,54},
			{ 46	,	54	,	56 },
			{ 46	,	56	,	38 },
			{ 48	,	40	,	38 },
			{ 48	,	38	,	56 },
			{ 48	,	56	,	58 },
			{ 48	,	58	,	40 },
			{ 50	,	42	,	40 },
			{ 50	,	40	,	58 },
			{ 50	,	58	,	60 },
			{ 50	,	60	,	42 },
			{ 52	,	44	,	42 },
			{ 52	,	42	,	60 },
			{ 52	,	60	,	62 },
			{ 52	,	62	,	44 },//right
			//	{52,53,44},
			//	{52,62,53},
			{ 64	,	56	,	54 },
			//{ 64	,	54	,	72 },//left
				{64,54,63},
				{64,63,72},
			{ 64	,	72	,	74 },//top
			//	{64,72,73},
			//	{64,73,74},
			{ 64	,	74	,	56 },
			{ 66	,	58	,	56 },
			{ 66	,	56	,	74 },
			{ 66	,	74	,	76 },//top
			//	{66,74,75},
			//	{66,75,76},
			{ 66	,	76	,	58 },
			{ 68	,	60	,	58 },
			{ 68	,	58	,	76 },
			{ 68	,	76	,	78 },//top
			//	{68,76,77},
			//	{68,77,78},
			{ 68	,	78	,	60 },
			{ 70	,	62	,	60 },
			{ 70	,	60	,	78 },
			{ 70	,	78	,	80 },//top
			//	{70,78,79},
			//	{70,79,80},
			{ 70	,	80	,	62 }//right
			//	{70,71,62},
			//	{70,80,71}
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_bl, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_bl->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_bl->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1br()
	{
		count_lod_1_br = 72;
		// 삼각형 64개 + 4 + 4
		MyIndex indices[] = {
		//{	10, 2, 0	},//bot
			{10,1,0},
			{10,2,1},
		{ 10	,	0	,	18 },//left
		//	{10,0,9},
		//	{10,9,18},
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },
		//{ 12	,	4	,	2 },//bot
			{12,3,2},
			{12,4,3},
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		//{ 14	,	6	,	4 },//bot
			{14,5,4},
			{14,6,5},
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		//{ 16	,	8	,	6 },//bot
			{16,7,6},
			{16,8,7},
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },
		//{ 16	,	26	,	8 },//right
			{16,17,8},
			{16,26,17},
		{ 28	,	20	,	18 },
		{ 28	,	18	,	36 },//left
		//	{28,18,27},
		//	{28,27,36},
		{ 28	,	36	,	38 },
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },
		//{ 34	,	44	,	26 },//right
			{34,35,26},
			{34,44,35},
		{ 46	,	38	,	36 },
		{ 46	,	36	,	54 },//left
		//	{46,36,45},
		//	{46,45,54},
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },
		//{ 52	,	62	,	44 },//right
			{52,53,44},
			{52,62,53},
		{ 64	,	56	,	54 },
		{ 64	,	54	,	72 },//left
		//	{64,54,63},
		//	{64,63,72},
		{ 64	,	72	,	74 },//top
		//	{64,72,73},
		//	{64,73,74},
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		{ 66	,	74	,	76 },//top
		//	{66,74,75},
		//	{66,75,76},
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		{ 68	,	76	,	78 },//top
		//	{68,76,77},
		//	{68,77,78},
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		{ 70	,	78	,	80 },//top
		//	{70,78,79},
		//	{70,79,80},
		//{ 70	,	80	,	62 }//right
			{70,71,62},
			{70,80,71}
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_br, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_br->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_br->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1tbl()
	{
		count_lod_1_tbl = 76;
		// 삼각형 64개 + 4 + 4 + 4
		MyIndex indices[] = {
		//{	10, 2, 0	},//bot
			{10,1,0},
			{10,2,1},
		//{ 10	,	0	,	18 },//left
			{10,0,9},
			{10,9,18},
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },
		//{ 12	,	4	,	2 },//bot
			{12,3,2},
			{12,4,3},
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		//{ 14	,	6	,	4 },//bot
			{14,5,4},
			{14,6,5},
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		//{ 16	,	8	,	6 },//bot
			{16,7,6},
			{16,8,7},
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },
		{ 16	,	26	,	8 },//right
		//	{16,17,8},
		//	{16,26,17},
		{ 28	,	20	,	18 },
		//{ 28	,	18	,	36 },//left
			{28,18,27},
			{28,27,36},
		{ 28	,	36	,	38 },
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },
		{ 34	,	44	,	26 },//right
		//	{34,35,26},
		//	{34,44,35},
		{ 46	,	38	,	36 },
		//{ 46	,	36	,	54 },//left
			{46,36,45},
			{46,45,54},
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },
		{ 52	,	62	,	44 },//right
		//	{52,53,44},
		//	{52,62,53},
		{ 64	,	56	,	54 },
		//{ 64	,	54	,	72 },//left
			{64,54,63},
			{64,63,72},
		//{ 64	,	72	,	74 },//top
			{64,72,73},
			{64,73,74},
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		//{ 66	,	74	,	76 },//top
			{66,74,75},
			{66,75,76},
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		//{ 68	,	76	,	78 },//top
			{68,76,77},
			{68,77,78},
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		//{ 70	,	78	,	80 },//top
			{70,78,79},
			{70,79,80},
		{ 70	,	80	,	62 }//right
		//	{70,71,62},
		//	{70,80,71}
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_tbl, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_tbl->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_tbl->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1tbr()
	{
		count_lod_1_tbr = 76;
		// 삼각형 64개 + 4 + 4 + 4
		MyIndex indices[] = {
		//{	10, 2, 0	},//bot
			{10,1,0},
			{10,2,1},
		{ 10	,	0	,	18 },//left
		//	{10,0,9},
		//	{10,9,18},
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },
		//{ 12	,	4	,	2 },//bot
			{12,3,2},
			{12,4,3},
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		//{ 14	,	6	,	4 },//bot
			{14,5,4},
			{14,6,5},
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		//{ 16	,	8	,	6 },//bot
			{16,7,6},
			{16,8,7},
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },
		//{ 16	,	26	,	8 },//right
			{16,17,8},
			{16,26,17},
		{ 28	,	20	,	18 },
		{ 28	,	18	,	36 },//left
		//	{28,18,27},
		//	{28,27,36},
		{ 28	,	36	,	38 },
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },
		//{ 34	,	44	,	26 },//right
			{34,35,26},
			{34,44,35},
		{ 46	,	38	,	36 },
		{ 46	,	36	,	54 },//left
		//	{46,36,45},
		//	{46,45,54},
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },
		//{ 52	,	62	,	44 },//right
			{52,53,44},
			{52,62,53},
		{ 64	,	56	,	54 },
		{ 64	,	54	,	72 },//left
		//	{64,54,63},
		//	{64,63,72},
		//{ 64	,	72	,	74 },//top
			{64,72,73},
			{64,73,74},
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		//{ 66	,	74	,	76 },//top
			{66,74,75},
			{66,75,76},
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		//{ 68	,	76	,	78 },//top
			{68,76,77},
			{68,77,78},
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		//{ 70	,	78	,	80 },//top
			{70,78,79},
			{70,79,80},
		//{ 70	,	80	,	62 }//right
			{70,71,62},
			{70,80,71}
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_tbr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_tbr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_tbr->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1tlr()
	{
		count_lod_1_tlr = 76;
		// 삼각형 64개 + 4 + 4 + 4
		MyIndex indices[] = {
		{	10, 2, 0	},//bot
		//	{10,1,0},
		//	{10,2,1},
		//{ 10	,	0	,	18 },//left
			{10,0,9},
			{10,9,18},
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },
		{ 12	,	4	,	2 },//bot
		//	{12,3,2},
		//	{12,4,3},
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		{ 14	,	6	,	4 },//bot
		//	{14,5,4},
		//	{14,6,5},
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		{ 16	,	8	,	6 },//bot
		//	{16,7,6},
		//	{16,8,7},
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },
		//{ 16	,	26	,	8 },//right
			{16,17,8},
			{16,26,17},
		{ 28	,	20	,	18 },
		//{ 28	,	18	,	36 },//left
			{28,18,27},
			{28,27,36},
		{ 28	,	36	,	38 },
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },
		//{ 34	,	44	,	26 },//right
			{34,35,26},
			{34,44,35},
		{ 46	,	38	,	36 },
		//{ 46	,	36	,	54 },//left
			{46,36,45},
			{46,45,54},
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },
		//{ 52	,	62	,	44 },//right
			{52,53,44},
			{52,62,53},
		{ 64	,	56	,	54 },
		//{ 64	,	54	,	72 },//left
			{64,54,63},
			{64,63,72},
		//{ 64	,	72	,	74 },//top
			{64,72,73},
			{64,73,74},
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		//{ 66	,	74	,	76 },//top
			{66,74,75},
			{66,75,76},
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		//{ 68	,	76	,	78 },//top
			{68,76,77},
			{68,77,78},
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		//{ 70	,	78	,	80 },//top
			{70,78,79},
			{70,79,80},
		//{ 70	,	80	,	62 }//right
			{70,71,62},
			{70,80,71}
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_tlr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_tlr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_tlr->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1blr()
	{
		count_lod_1_blr = 76;
		// 삼각형 64개 + 4 + 4 + 4
		MyIndex indices[] = {
		//{	10, 2, 0	},//bot
			{10,1,0},
			{10,2,1},
		//{ 10	,	0	,	18 },//left
			{10,0,9},
			{10,9,18},
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },
		//{ 12	,	4	,	2 },//bot
			{12,3,2},
			{12,4,3},
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		//{ 14	,	6	,	4 },//bot
			{14,5,4},
			{14,6,5},
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		//{ 16	,	8	,	6 },//bot
			{16,7,6},
			{16,8,7},
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },
		//{ 16	,	26	,	8 },//right
			{16,17,8},
			{16,26,17},
		{ 28	,	20	,	18 },
		//{ 28	,	18	,	36 },//left
			{28,18,27},
			{28,27,36},
		{ 28	,	36	,	38 },
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },
		//{ 34	,	44	,	26 },//right
			{34,35,26},
			{34,44,35},
		{ 46	,	38	,	36 },
		//{ 46	,	36	,	54 },//left
			{46,36,45},
			{46,45,54},
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },
		//{ 52	,	62	,	44 },//right
			{52,53,44},
			{52,62,53},
		{ 64	,	56	,	54 },
		//{ 64	,	54	,	72 },//left
			{64,54,63},
			{64,63,72},
		{ 64	,	72	,	74 },//top
		//	{64,72,73},
		//	{64,73,74},
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		{ 66	,	74	,	76 },//top
		//	{66,74,75},
		//	{66,75,76},
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		{ 68	,	76	,	78 },//top
		//	{68,76,77},
		//	{68,77,78},
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		{ 70	,	78	,	80 },//top
		//	{70,78,79},
		//	{70,79,80},
		//{ 70	,	80	,	62 }//right
			{70,71,62},
			{70,80,71}
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_blr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_blr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_blr->Unlock();

		return true;
	}
	bool CCell::CreateIndexBufferLodLevel1tblr()
	{
		count_lod_1_tblr = 80;
		// 삼각형 64개 + 4 + 4 + 4 + 4
		MyIndex indices[] = {		
		//{	10, 2, 0	},//bot
			{10,1,0},
			{10,2,1},		
		//{ 10	,	0	,	18 },//left
			{10,0,9},
			{10,9,18},
		{ 10	,	18	,	20 },
		{ 10	,	20	,	2 },		
		//{ 12	,	4	,	2 },//bot
			{12,3,2},
			{12,4,3},
		{ 12	,	2	,	20 },
		{ 12	,	20	,	22 },
		{ 12	,	22	,	4 },
		//{ 14	,	6	,	4 },//bot
			{14,5,4},
			{14,6,5},
		{ 14	,	4	,	22 },
		{ 14	,	22	,	24 },
		{ 14	,	24	,	6 },
		//{ 16	,	8	,	6 },//bot
			{16,7,6},
			{16,8,7},
		{ 16	,	6	,	24 },
		{ 16	,	24	,	26 },
		//{ 16	,	26	,	8 },//right
			{16,17,8},
			{16,26,17},
		{ 28	,	20	,	18 },		
		//{ 28	,	18	,	36 },//left
			{28,18,27},
			{28,27,36},
		{ 28	,	36	,	38 },
		{ 28	,	38	,	20 },
		{ 30	,	22	,	20 },
		{ 30	,	20	,	38 },
		{ 30	,	38	,	40 },
		{ 30	,	40	,	22 },
		{ 32	,	24	,	22 },
		{ 32	,	22	,	40 },
		{ 32	,	40	,	42 },
		{ 32	,	42	,	24 },
		{ 34	,	26	,	24 },
		{ 34	,	24	,	42 },
		{ 34	,	42	,	44 },
		//{ 34	,	44	,	26 },//right
			{34,35,26},
			{34,44,35},
		{ 46	,	38	,	36 },		
		//{ 46	,	36	,	54 },//left
			{46,36,45},
			{46,45,54},
		{ 46	,	54	,	56 },
		{ 46	,	56	,	38 },
		{ 48	,	40	,	38 },
		{ 48	,	38	,	56 },
		{ 48	,	56	,	58 },
		{ 48	,	58	,	40 },
		{ 50	,	42	,	40 },
		{ 50	,	40	,	58 },
		{ 50	,	58	,	60 },
		{ 50	,	60	,	42 },
		{ 52	,	44	,	42 },
		{ 52	,	42	,	60 },
		{ 52	,	60	,	62 },
		//{ 52	,	62	,	44 },//right
			{52,53,44},
			{52,62,53},
		{ 64	,	56	,	54 },		
		//{ 64	,	54	,	72 },//left
			{64,54,63},
			{64,63,72},
		//{ 64	,	72	,	74 },//top
			{64,72,73},
			{64,73,74},
		{ 64	,	74	,	56 },
		{ 66	,	58	,	56 },
		{ 66	,	56	,	74 },
		//{ 66	,	74	,	76 },//top
			{66,74,75},
			{66,75,76},
		{ 66	,	76	,	58 },
		{ 68	,	60	,	58 },
		{ 68	,	58	,	76 },
		//{ 68	,	76	,	78 },//top
			{68,76,77},
			{68,77,78},
		{ 68	,	78	,	60 },
		{ 70	,	62	,	60 },
		{ 70	,	60	,	78 },
		//{ 70	,	78	,	80 },//top
			{70,78,79},
			{70,79,80},
		//{ 70	,	80	,	62 }//right
			{70,71,62},
			{70,80,71}
		};

		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_1_tblr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_1_tblr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_1_tblr->Unlock();

		return true;
	}

	bool CCell::CreateIndexBufferLodLevel2none()
	{
		count_lod_2 = 16;
		// 삼각형 16개
		MyIndex indices[] = {
		{	20, 4, 0	},
		{ 20	,	0	,	36 },
		{ 20	,	36	,	40 },
		{ 20	,	40	,	4 },
		{ 24	,	8	,	4 },
		{ 24	,	4	,	40 },
		{ 24	,	40	,	44 },
		{ 24	,	44	,	8 },
		{ 56	,	40	,	36 },
		{ 56	,	36	,	72 },
		{ 56	,	72	,	76 },
		{ 56	,	76	,	40 },
		{ 60	,	44	,	40 },
		{ 60	,	40	,	76 },
		{ 60	,	76	,	80 },
		{ 60	,	80	,	44 }
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2t()
	{
		count_lod_2_t = 18;
		// 삼각형 16개 + 2
		MyIndex indices[] = {
			{	20, 4, 0	},//bot
			//	{20,2,0},
			//	{20,4,2},
			{ 20	,	0	,	36 },//left
			//	{20,0,18},
			//	{20,18,36},
			{ 20	,	36	,	40 },
			{ 20	,	40	,	4 },
			{ 24	,	8	,	4 },//bot
			//	{24,6,4},
			//	{24,8,6},
			{ 24	,	4	,	40 },
			{ 24	,	40	,	44 },
			{ 24	,	44	,	8 },//right
			//	{24,26,8},
			//	{24,44,26},
			{ 56	,	40	,	36 },
			{ 56	,	36	,	72 },//left
			//	{56,36,54},
			//	{56,54,72},
			//{ 56	,	72	,	76 },//top
				{56,72,74},
				{56,74,76},
			{ 56	,	76	,	40 },
			{ 60	,	44	,	40 },
			{ 60	,	40	,	76 },
			//{ 60	,	76	,	80 },//top
				{60,76,78},
				{60,78,80},
			{ 60	,	80	,	44 }//right
			//	{60,62,44},
			//	{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_t, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_t->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_t->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2b()
	{
		count_lod_2_b = 18;
		// 삼각형 16개 + 2
		MyIndex indices[] = {
			//{	20, 4, 0	},//bot
				{20,2,0},
				{20,4,2},
			{ 20	,	0	,	36 },//left
			//	{20,0,18},
			//	{20,18,36},
			{ 20	,	36	,	40 },
			{ 20	,	40	,	4 },
			//{ 24	,	8	,	4 },//bot
				{24,6,4},
				{24,8,6},
			{ 24	,	4	,	40 },
			{ 24	,	40	,	44 },
			{ 24	,	44	,	8 },//right
			//	{24,26,8},
			//	{24,44,26},
			{ 56	,	40	,	36 },
			{ 56	,	36	,	72 },//left
			//	{56,36,54},
			//	{56,54,72},
			{ 56	,	72	,	76 },//top
			//	{56,72,74},
			//	{56,74,76},
			{ 56	,	76	,	40 },
			{ 60	,	44	,	40 },
			{ 60	,	40	,	76 },
			{ 60	,	76	,	80 },//top
			//	{60,76,78},
			//	{60,78,80},
			{ 60	,	80	,	44 }//right
			//	{60,62,44},
			//	{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_b, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_b->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_b->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2l()
	{
		count_lod_2_l = 18;
		// 삼각형 16개 + 2
		MyIndex indices[] = {
			{	20, 4, 0	},//bot
			//	{20,2,0},
			//	{20,4,2},
			//{ 20	,	0	,	36 },//left
				{20,0,18},
				{20,18,36},
			{ 20	,	36	,	40 },
			{ 20	,	40	,	4 },
			{ 24	,	8	,	4 },//bot
			//	{24,6,4},
			//	{24,8,6},
			{ 24	,	4	,	40 },
			{ 24	,	40	,	44 },
			{ 24	,	44	,	8 },//right
			//	{24,26,8},
			//	{24,44,26},
			{ 56	,	40	,	36 },
			//{ 56	,	36	,	72 },//left
				{56,36,54},
				{56,54,72},
			{ 56	,	72	,	76 },//top
			//	{56,72,74},
			//	{56,74,76},
			{ 56	,	76	,	40 },
			{ 60	,	44	,	40 },
			{ 60	,	40	,	76 },
			{ 60	,	76	,	80 },//top
			//	{60,76,78},
			//	{60,78,80},
			{ 60	,	80	,	44 }//right
			//	{60,62,44},
			//	{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_l, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_l->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_l->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2r()
	{
		count_lod_2_r = 18;
		// 삼각형 16개 + 2
		MyIndex indices[] = {
			{	20, 4, 0	},//bot
			//	{20,2,0},
			//	{20,4,2},
			{ 20	,	0	,	36 },//left
			//	{20,0,18},
			//	{20,18,36},
			{ 20	,	36	,	40 },
			{ 20	,	40	,	4 },
			{ 24	,	8	,	4 },//bot
			//	{24,6,4},
			//	{24,8,6},
			{ 24	,	4	,	40 },
			{ 24	,	40	,	44 },
			//{ 24	,	44	,	8 },//right
				{24,26,8},
				{24,44,26},
			{ 56	,	40	,	36 },
			{ 56	,	36	,	72 },//left
			//	{56,36,54},
			//	{56,54,72},
			{ 56	,	72	,	76 },//top
			//	{56,72,74},
			//	{56,74,76},
			{ 56	,	76	,	40 },
			{ 60	,	44	,	40 },
			{ 60	,	40	,	76 },
			{ 60	,	76	,	80 },//top
			//	{60,76,78},
			//	{60,78,80},
			//{ 60	,	80	,	44 }//right
				{60,62,44},
				{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_r, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_r->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_r->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2tb()
	{
		count_lod_2_tb = 20;
		// 삼각형 16개 + 2 + 2
		MyIndex indices[] = {
			//{	20, 4, 0	},//bot
				{20,2,0},
				{20,4,2},
			{ 20	,	0	,	36 },//left
			//	{20,0,18},
			//	{20,18,36},
			{ 20	,	36	,	40 },
			{ 20	,	40	,	4 },
			//{ 24	,	8	,	4 },//bot
				{24,6,4},
				{24,8,6},
			{ 24	,	4	,	40 },
			{ 24	,	40	,	44 },
			{ 24	,	44	,	8 },//right
			//	{24,26,8},
			//	{24,44,26},
			{ 56	,	40	,	36 },
			{ 56	,	36	,	72 },//left
			//	{56,36,54},
			//	{56,54,72},
			//{ 56	,	72	,	76 },//top
				{56,72,74},
				{56,74,76},
			{ 56	,	76	,	40 },
			{ 60	,	44	,	40 },
			{ 60	,	40	,	76 },
			//{ 60	,	76	,	80 },//top
				{60,76,78},
				{60,78,80},
			{ 60	,	80	,	44 }//right
			//	{60,62,44},
			//	{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_tb, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_tb->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_tb->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2lr()
	{
		count_lod_2_lr = 20;
		// 삼각형 16개 + 2 + 2
		MyIndex indices[] = {
			{	20, 4, 0	},//bot
			//	{20,2,0},
			//	{20,4,2},
			//{ 20	,	0	,	36 },//left
				{20,0,18},
				{20,18,36},
			{ 20	,	36	,	40 },
			{ 20	,	40	,	4 },
			{ 24	,	8	,	4 },//bot
			//	{24,6,4},
			//	{24,8,6},
			{ 24	,	4	,	40 },
			{ 24	,	40	,	44 },
			//{ 24	,	44	,	8 },//right
				{24,26,8},
				{24,44,26},
			{ 56	,	40	,	36 },
			//{ 56	,	36	,	72 },//left
				{56,36,54},
				{56,54,72},
			{ 56	,	72	,	76 },//top
			//	{56,72,74},
			//	{56,74,76},
			{ 56	,	76	,	40 },
			{ 60	,	44	,	40 },
			{ 60	,	40	,	76 },
			{ 60	,	76	,	80 },//top
			//	{60,76,78},
			//	{60,78,80},
			//{ 60	,	80	,	44 },//right
				{60,62,44},
				{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_lr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_lr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_lr->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2tl()
	{
		count_lod_2_tl = 20;
		// 삼각형 16개 + 2 + 2
		MyIndex indices[] = {
			{	20, 4, 0	},//bot
			//	{20,2,0},
			//	{20,4,2},
			//{ 20	,	0	,	36 },//left
				{20,0,18},
				{20,18,36},
			{ 20	,	36	,	40 },
			{ 20	,	40	,	4 },
			{ 24	,	8	,	4 },//bot
			//	{24,6,4},
			//	{24,8,6},
			{ 24	,	4	,	40 },
			{ 24	,	40	,	44 },
			{ 24	,	44	,	8 },//right
			//	{24,26,8},
			//	{24,44,26},
			{ 56	,	40	,	36 },
			//{ 56	,	36	,	72 },//left
				{56,36,54},
				{56,54,72},
			//{ 56	,	72	,	76 },//top
				{56,72,74},
				{56,74,76},
			{ 56	,	76	,	40 },
			{ 60	,	44	,	40 },
			{ 60	,	40	,	76 },
			//{ 60	,	76	,	80 },//top
				{60,76,78},
				{60,78,80},
			{ 60	,	80	,	44 }//right
			//	{60,62,44},
			//	{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_tl, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_tl->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_tl->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2tr()
	{
		count_lod_2_tr = 20;
		// 삼각형 16개 + 2 + 2
		MyIndex indices[] = {
			{	20, 4, 0	},//bot
			//	{20,2,0},
			//	{20,4,2},
			{ 20	,	0	,	36 },//left
			//	{20,0,18},
			//	{20,18,36},
			{ 20	,	36	,	40 },
			{ 20	,	40	,	4 },
			{ 24	,	8	,	4 },//bot
			//	{24,6,4},
			//	{24,8,6},
			{ 24	,	4	,	40 },
			{ 24	,	40	,	44 },
			//{ 24	,	44	,	8 },//right
				{24,26,8},
				{24,44,26},
			{ 56	,	40	,	36 },
			{ 56	,	36	,	72 },//left
			//	{56,36,54},
			//	{56,54,72},
			//{ 56	,	72	,	76 },//top
				{56,72,74},
				{56,74,76},
			{ 56	,	76	,	40 },
			{ 60	,	44	,	40 },
			{ 60	,	40	,	76 },
			//{ 60	,	76	,	80 },//top
				{60,76,78},
				{60,78,80},
			//{ 60	,	80	,	44 },//right
				{60,62,44},
				{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_tr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_tr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_tr->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2bl()
	{
		count_lod_2_bl = 20;
		// 삼각형 16개 + 2 + 2
		MyIndex indices[] = {
			//{	20, 4, 0	},//bot
				{20,2,0},
				{20,4,2},
			//{ 20	,	0	,	36 },//left
				{20,0,18},
				{20,18,36},
			{ 20	,	36	,	40 },
			{ 20	,	40	,	4 },
			//{ 24	,	8	,	4 },//bot
				{24,6,4},
				{24,8,6},
			{ 24	,	4	,	40 },
			{ 24	,	40	,	44 },
			{ 24	,	44	,	8 },//right
			//	{24,26,8},
			//	{24,44,26},
			{ 56	,	40	,	36 },
			//{ 56	,	36	,	72 },//left
				{56,36,54},
				{56,54,72},
			{ 56	,	72	,	76 },//top
			//	{56,72,74},
			//	{56,74,76},
			{ 56	,	76	,	40 },
			{ 60	,	44	,	40 },
			{ 60	,	40	,	76 },
			{ 60	,	76	,	80 },//top
			//	{60,76,78},
			//	{60,78,80},
			{ 60	,	80	,	44 }//right
			//	{60,62,44},
			//	{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_bl, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_bl->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_bl->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2br()
	{
		count_lod_2_br = 20;
		// 삼각형 16개 + 2 + 2
		MyIndex indices[] = {
			//{	20, 4, 0	},//bot
				{20,2,0},
				{20,4,2},
			{ 20	,	0	,	36 },//left
			//	{20,0,18},
			//	{20,18,36},
			{ 20	,	36	,	40 },
			{ 20	,	40	,	4 },
			//{ 24	,	8	,	4 },//bot
				{24,6,4},
				{24,8,6},
			{ 24	,	4	,	40 },
			{ 24	,	40	,	44 },
			//{ 24	,	44	,	8 },//right
				{24,26,8},
				{24,44,26},
			{ 56	,	40	,	36 },
			{ 56	,	36	,	72 },//left
			//	{56,36,54},
			//	{56,54,72},
			{ 56	,	72	,	76 },//top
			//	{56,72,74},
			//	{56,74,76},
			{ 56	,	76	,	40 },
			{ 60	,	44	,	40 },
			{ 60	,	40	,	76 },
			{ 60	,	76	,	80 },//top
			//	{60,76,78},
			//	{60,78,80},
			//{ 60	,	80	,	44 }//right
				{60,62,44},
				{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_br, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_br->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_br->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2tbl()
	{
		count_lod_2_tbl = 22;
		// 삼각형 16개 + 2 + 2 + 2
		MyIndex indices[] = {
			//{	20, 4, 0	},//bot
				{20,2,0},
				{20,4,2},
			//{ 20	,	0	,	36 },//left
				{20,0,18},
				{20,18,36},
			{ 20	,	36	,	40 },
			{ 20	,	40	,	4 },
			//{ 24	,	8	,	4 },//bot
				{24,6,4},
				{24,8,6},
			{ 24	,	4	,	40 },
			{ 24	,	40	,	44 },
			{ 24	,	44	,	8 },//right
			//	{24,26,8},
			//	{24,44,26},
			{ 56	,	40	,	36 },
			//{ 56	,	36	,	72 },//left
				{56,36,54},
				{56,54,72},
			//{ 56	,	72	,	76 },//top
				{56,72,74},
				{56,74,76},
			{ 56	,	76	,	40 },
			{ 60	,	44	,	40 },
			{ 60	,	40	,	76 },
			//{ 60	,	76	,	80 },//top
				{60,76,78},
				{60,78,80},
			{ 60	,	80	,	44 }//right
			//	{60,62,44},
			//	{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_tbl, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_tbl->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_tbl->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2tbr()
	{
		count_lod_2_tbr = 22;
		// 삼각형 16개 + 2 + 2 + 2
		MyIndex indices[] = {
			//{	20, 4, 0	},//bot
				{20,2,0},
				{20,4,2},
			{ 20	,	0	,	36 },//left
			//	{20,0,18},
			//	{20,18,36},
			{ 20	,	36	,	40 },
			{ 20	,	40	,	4 },
			//{ 24	,	8	,	4 },//bot
				{24,6,4},
				{24,8,6},
			{ 24	,	4	,	40 },
			{ 24	,	40	,	44 },
			//{ 24	,	44	,	8 },//right
				{24,26,8},
				{24,44,26},
			{ 56	,	40	,	36 },
			{ 56	,	36	,	72 },//left
			//	{56,36,54},
			//	{56,54,72},
			//{ 56	,	72	,	76 },//top
				{56,72,74},
				{56,74,76},
			{ 56	,	76	,	40 },
			{ 60	,	44	,	40 },
			{ 60	,	40	,	76 },
			//{ 60	,	76	,	80 },//top
				{60,76,78},
				{60,78,80},
			//{ 60	,	80	,	44 }//right
				{60,62,44},
				{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_tbr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_tbr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_tbr->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2tlr()
	{
		count_lod_2_tlr = 22;
		// 삼각형 16개 + 2 + 2 + 2
		MyIndex indices[] = {
			{	20, 4, 0	},//bot
			//	{20,2,0},
			//	{20,4,2},
			//{ 20	,	0	,	36 },//left
				{20,0,18},
				{20,18,36},
			{ 20	,	36	,	40 },
			{ 20	,	40	,	4 },
			{ 24	,	8	,	4 },//bot
			//	{24,6,4},
			//	{24,8,6},
			{ 24	,	4	,	40 },
			{ 24	,	40	,	44 },
			//{ 24	,	44	,	8 },//right
				{24,26,8},
				{24,44,26},
			{ 56	,	40	,	36 },
			//{ 56	,	36	,	72 },//left
				{56,36,54},
				{56,54,72},
			//{ 56	,	72	,	76 },//top
				{56,72,74},
				{56,74,76},
			{ 56	,	76	,	40 },
			{ 60	,	44	,	40 },
			{ 60	,	40	,	76 },
			//{ 60	,	76	,	80 },//top
				{60,76,78},
				{60,78,80},
			//{ 60	,	80	,	44 }//right
				{60,62,44},
				{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_tlr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_tlr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_tlr->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2blr()
	{
		count_lod_2_blr = 22;
		// 삼각형 16개 + 2 + 2 + 2
		MyIndex indices[] = {
			//{	20, 4, 0	},//bot
			{20,2,0},
			{20,4,2},
			//{ 20	,	0	,	36 },//left
				{20,0,18},
				{20,18,36},
			{ 20	,	36	,	40 },
			{ 20	,	40	,	4 },
			//{ 24	,	8	,	4 },//bot
				{24,6,4},
				{24,8,6},
			{ 24	,	4	,	40 },
			{ 24	,	40	,	44 },
			//{ 24	,	44	,	8 },//right
				{24,26,8},
				{24,44,26},
			{ 56	,	40	,	36 },
			//{ 56	,	36	,	72 },//left
			{56,36,54},
			{56,54,72},
			{ 56	,	72	,	76 },//top
			//	{56,72,74},
			//	{56,74,76},
			{ 56	,	76	,	40 },
			{ 60	,	44	,	40 },
			{ 60	,	40	,	76 },
			{ 60	,	76	,	80 },//top
			//	{60,76,78},
			//	{60,78,80},
			//{ 60	,	80	,	44 }//right
				{60,62,44},
				{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_blr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_blr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_blr->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel2tblr()
	{
		count_lod_2_tblr = 24;
		// 삼각형 16개 + 2 + 2 + 2 + 2
		MyIndex indices[] = {
		//{	20, 4, 0	},//bot
			{20,2,0},
			{20,4,2},
		//{ 20	,	0	,	36 },//left
			{20,0,18},
			{20,18,36},
		{ 20	,	36	,	40 },
		{ 20	,	40	,	4 },
		//{ 24	,	8	,	4 },//bot
			{24,6,4},
			{24,8,6},
		{ 24	,	4	,	40 },
		{ 24	,	40	,	44 },
		//{ 24	,	44	,	8 },//right
			{24,26,8},
			{24,44,26},
		{ 56	,	40	,	36 },
		//{ 56	,	36	,	72 },//left
			{56,36,54},
			{56,54,72},
		//{ 56	,	72	,	76 },//top
			{56,72,74},
			{56,74,76},
		{ 56	,	76	,	40 },
		{ 60	,	44	,	40 },
		{ 60	,	40	,	76 },
		//{ 60	,	76	,	80 },//top
			{60,76,78},
			{60,78,80},
		//{ 60	,	80	,	44 }//right
			{60,62,44},
			{60,80,62}
		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_2_tblr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_2_tblr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_2_tblr->Unlock();
		return true;
	}

	bool CCell::CreateIndexBufferLodLevel3none()
	{
		count_lod_3 = 4;
		// 삼각형 4개
		MyIndex indices[] = {
		{	40	,	8	,	0	},
		{	40	,	0	,	72	},
		{	40	,	72	,	80	},
		{	40	,	80	,	8	}

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3t()
	{
		count_lod_3_t = 5;
		// 삼각형 4개+1
		MyIndex indices[] = {
		{	40	,	8	,	0	},//bot {40,4,0},{40,8,4},
		{	40	,	0	,	72	},//left {40,0,36},{40,36,72},
		//{	40	,	72	,	80	},//top 
			{40,72,76},{40,76,80},
		{	40	,	80	,	8	}//right {40,44,8},{40,80,44},

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_t, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_t->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_t->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3b()
	{
		count_lod_3_b = 5;
		// 삼각형 4개+1
		MyIndex indices[] = {
		//{	40	,	8	,	0	},//bot 
			{40,4,0},{40,8,4},
		{	40	,	0	,	72	},//left {40,0,36},{40,36,72},
		{	40	,	72	,	80	},//top {40,72,76},{40,76,80},
		{	40	,	80	,	8	}//right {40,44,8},{40,80,44},

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_b, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_b->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_b->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3l()
	{
		count_lod_3_l = 5;
		// 삼각형 4개+1
		MyIndex indices[] = {
		{	40	,	8	,	0	},//bot {40,4,0},{40,8,4},
		//{	40	,	0	,	72	},//left 
			{40,0,36},{40,36,72},
		{	40	,	72	,	80	},//top {40,72,76},{40,76,80},
		{	40	,	80	,	8	}//right {40,44,8},{40,80,44},

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_l, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_l->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_l->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3r()
	{
		count_lod_3_r = 5;
		// 삼각형 4개+1
		MyIndex indices[] = {
		{	40	,	8	,	0	},//bot {40,4,0},{40,8,4},
		{	40	,	0	,	72	},//left {40,0,36},{40,36,72},
		{	40	,	72	,	80	},//top {40,72,76},{40,76,80},
		//{	40	,	80	,	8	}//right 
			{40,44,8},{40,80,44}

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_r, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_r->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_r->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3tb()
	{
		count_lod_3_tb = 6;
		// 삼각형 4개+1+1
		MyIndex indices[] = {
		//{	40	,	8	,	0	},//bot 
			{40,4,0},{40,8,4},
		{	40	,	0	,	72	},//left {40,0,36},{40,36,72},
		//{	40	,	72	,	80	},//top 
			{40,72,76},{40,76,80},
		{	40	,	80	,	8	}//right {40,44,8},{40,80,44},

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_tb, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_tb->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_tb->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3lr()
	{
		count_lod_3_lr = 6;
		// 삼각형 4개+1+1
		MyIndex indices[] = {
		{	40	,	8	,	0	},//bot {40,4,0},{40,8,4},
		//{	40	,	0	,	72	},//left 
			{40,0,36},{40,36,72},
		{	40	,	72	,	80	},//top {40,72,76},{40,76,80},
		//{	40	,	80	,	8	}//right 
			{40,44,8},{40,80,44}

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_lr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_lr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_lr->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3tl()
	{
		count_lod_3_tl = 6;
		// 삼각형 4개+1+1
		MyIndex indices[] = {
		{	40	,	8	,	0	},//bot {40,4,0},{40,8,4},
		//{	40	,	0	,	72	},//left 
			{40,0,36},{40,36,72},
		//{	40	,	72	,	80	},//top 
			{40,72,76},{40,76,80},
		{	40	,	80	,	8	}//right {40,44,8},{40,80,44},

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_tl, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_tl->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_tl->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3tr()
	{
		count_lod_3_tr = 6;
		// 삼각형 4개+1+1
		MyIndex indices[] = {
		{	40	,	8	,	0	},//bot {40,4,0},{40,8,4},
		{	40	,	0	,	72	},//left {40,0,36},{40,36,72},
		//{	40	,	72	,	80	},//top 
			{40,72,76},{40,76,80},
		//{	40	,	80	,	8	},//right 
			{40,44,8},{40,80,44}

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_tr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_tr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_tr->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3bl()
	{
		count_lod_3_bl = 6;
		// 삼각형 4개+1+1
		MyIndex indices[] = {
		//{	40	,	8	,	0	},//bot 
			{40,4,0},{40,8,4},
		//{	40	,	0	,	72	},//left 
			{40,0,36},{40,36,72},
		{	40	,	72	,	80	},//top {40,72,76},{40,76,80},
		{	40	,	80	,	8	}//right {40,44,8},{40,80,44},

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_bl, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_bl->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_bl->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3br()
	{
		count_lod_3_br = 6;
		// 삼각형 4개+1+1
		MyIndex indices[] = {
		//{	40	,	8	,	0	},//bot 
			{40,4,0},{40,8,4},
		{	40	,	0	,	72	},//left {40,0,36},{40,36,72},
		{	40	,	72	,	80	},//top {40,72,76},{40,76,80},
		//{	40	,	80	,	8	}//right 
			{40,44,8},{40,80,44}

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_br, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_br->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_br->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3tbl()
	{
		count_lod_3_tbl = 7;
		// 삼각형 4개+1+1+1
		MyIndex indices[] = {
		//{	40	,	8	,	0	},//bot 
			{40,4,0},{40,8,4},
		//{	40	,	0	,	72	},//left 
			{40,0,36},{40,36,72},
		//{	40	,	72	,	80	},//top 
			{40,72,76},{40,76,80},
		{	40	,	80	,	8	}//right {40,44,8},{40,80,44},

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_tbl, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_tbl->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_tbl->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3tbr()
	{
		count_lod_3_tbr = 7;
		// 삼각형 4개+1+1+1
		MyIndex indices[] = {
		//{	40	,	8	,	0	},//bot 
			{40,4,0},{40,8,4},
		{	40	,	0	,	72	},//left {40,0,36},{40,36,72},
		//{	40	,	72	,	80	},//top 
			{40,72,76},{40,76,80},
		//{	40	,	80	,	8	},//right 
			{40,44,8},{40,80,44}

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_tbr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_tbr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_tbr->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3tlr()
	{
		count_lod_3_tlr = 7;
		// 삼각형 4개+1+1+1
		MyIndex indices[] = {
		{	40	,	8	,	0	},//bot {40,4,0},{40,8,4},
		//{	40	,	0	,	72	},//left 
			{40,0,36},{40,36,72},
		//{	40	,	72	,	80	},//top 
			{40,72,76},{40,76,80},
		//{	40	,	80	,	8	}//right 
			{40,44,8},{40,80,44}

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_tlr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_tlr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_tlr->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3blr()
	{
		count_lod_3_blr = 7;
		// 삼각형 4개+1+1+1
		MyIndex indices[] = {
		//{	40	,	8	,	0	},//bot 
			{40,4,0},{40,8,4},
		//{	40	,	0	,	72	},//left 
			{40,0,36},{40,36,72},
		{	40	,	72	,	80	},//top {40,72,76},{40,76,80},
		//{	40	,	80	,	8	}//right 
			{40,44,8},{40,80,44}

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_blr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_blr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_blr->Unlock();
		return true;
	}
	bool CCell::CreateIndexBufferLodLevel3tblr()
	{
		count_lod_3_tblr = 8;
		// 삼각형 4개+1+1+1+1
		MyIndex indices[] = {
		//{	40	,	8	,	0	},//bot 
			{40,4,0},{40,8,4},
		//{	40	,	0	,	72	},//left 
			{40,0,36},{40,36,72},
		//{	40	,	72	,	80	},//top 
			{40,72,76},{40,76,80},
		//{	40	,	80	,	8	},//right 
			{40,44,8},{40,80,44}

		};
		if (FAILED(m_pd3dDevice->CreateIndexBuffer(sizeof(indices), 0, D3DFMT_INDEX16,
			D3DPOOL_DEFAULT, &m_pIB_lod_3_tblr, NULL)))
		{
			return false;
		}

		VOID* pIndices;
		if (FAILED(m_pIB_lod_3_tblr->Lock(0, sizeof(indices), (void**)&pIndices, 0)))
		{
			return false;
		}

		memcpy(pIndices, indices, sizeof(indices));
		m_pIB_lod_3_tblr->Unlock();
		return true;
	}

	void CCell::Render(int lodLevel, int patch)
	{
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); // 와이어 프레임으로 보기

		m_pd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(SVertex3D));
		m_pd3dDevice->SetFVF(SVertex3D::FVF);

		if (lodLevel == 0) {
			m_pd3dDevice->SetIndices(m_pIB_lod_0);
			m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_0);
		} else if (lodLevel == 1) {
			if (patch == CCell::none) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1);
			}
			else if (patch == CCell::patch_TOP) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_t);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_t);
			}
			else if (patch == CCell::patch_BOT) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_b);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_b);
			} 
			else if (patch == CCell::patch_LEFT) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_l);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_l);
			}
			else if (patch == CCell::patch_RIGHT) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_r);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_r);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_BOT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_tb);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_tb);
			}
			else if (patch == (CCell::patch_LEFT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_lr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_lr);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_LEFT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_tl);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_tl);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_tr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_tr);
			}
			else if (patch == (CCell::patch_BOT | CCell::patch_LEFT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_bl);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_bl);
			}
			else if (patch == (CCell::patch_BOT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_br);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_br);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_BOT | CCell::patch_LEFT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_tbl);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_tbl);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_BOT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_tbr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_tbr);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_LEFT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_tlr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_tlr);
			}
			else if (patch == (CCell::patch_BOT | CCell::patch_LEFT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_blr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_blr);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_BOT | CCell::patch_LEFT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_1_tblr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_1_tblr);
			}
		} else if (lodLevel == 2) {
			if (patch == CCell::none) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2);
			}
			else if (patch == CCell::patch_TOP) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_t);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_t);
			}
			else if (patch == CCell::patch_BOT) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_b);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_b);
			}
			else if (patch == CCell::patch_LEFT) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_l);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_l);
			}
			else if (patch == CCell::patch_RIGHT) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_r);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_r);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_BOT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_tb);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_tb);
			}
			else if (patch == (CCell::patch_LEFT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_lr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_lr);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_LEFT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_tl);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_tl);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_tr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_tr);
			}
			else if (patch == (CCell::patch_BOT | CCell::patch_LEFT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_bl);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_bl);
			}
			else if (patch == (CCell::patch_BOT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_br);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_br);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_BOT | CCell::patch_LEFT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_tbl);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_tbl);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_BOT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_tbr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_tbr);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_LEFT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_tlr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_tlr);
			}
			else if (patch == (CCell::patch_BOT | CCell::patch_LEFT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_blr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_blr);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_BOT | CCell::patch_LEFT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_2_tblr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_2_tblr);
			}
		} else if (lodLevel == 3) {
			if (patch == CCell::none) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3);
			}
			else if (patch == CCell::patch_TOP) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_t);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_t);
			}
			else if (patch == CCell::patch_BOT) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_b);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_b);
			}
			else if (patch == CCell::patch_LEFT) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_l);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_l);
			}
			else if (patch == CCell::patch_RIGHT) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_r);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_r);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_BOT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_tb);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_tb);
			}
			else if (patch == (CCell::patch_LEFT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_lr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_lr);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_LEFT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_tl);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_tl);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_tr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_tr);
			}
			else if (patch == (CCell::patch_BOT | CCell::patch_LEFT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_bl);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_bl);
			}
			else if (patch == (CCell::patch_BOT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_br);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_br);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_BOT | CCell::patch_LEFT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_tbl);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_tbl);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_BOT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_tbr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_tbr);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_LEFT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_tlr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_tlr);
			}
			else if (patch == (CCell::patch_BOT | CCell::patch_LEFT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_blr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_blr);
			}
			else if (patch == (CCell::patch_TOP | CCell::patch_BOT | CCell::patch_LEFT | CCell::patch_RIGHT)) {
				m_pd3dDevice->SetIndices(m_pIB_lod_3_tblr);
				m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 81, 0, count_lod_3_tblr);
			}
		}
	}

}