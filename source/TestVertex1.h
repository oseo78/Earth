#pragma once

#include "DirectX.h"

namespace Earth
{
#define SAFE_RELEASE(p) { if ( (p) ) { (p)->Release(); (p) = 0; } }

	// 2D 삼각형
	struct SVertex2D
	{
		FLOAT x, y, z, rhw;
		DWORD color;
		enum { FVF = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE) };
	};
	
	class CTriangle2D
	{
	public:
		CTriangle2D();
		~CTriangle2D();

		bool InitVertexBuffer(LPDIRECT3DDEVICE9 pd3dDevice);
		void Render();

	private:
		SVertex2D vertices[3];
		LPDIRECT3DDEVICE9       m_pd3dDevice = NULL; // Our rendering device
		LPDIRECT3DVERTEXBUFFER9 m_pVB = NULL; // Buffer to hold vertices
	};

	// 3D 삼각형
	struct SVertex3D
	{
		FLOAT x, y, z;
		DWORD color;
		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE) };
	};

	struct SVertex3Db
	{
		D3DXVECTOR3 position;
		DWORD color;
		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE) };
	};

	class CTriangle3D
	{
	public:
		CTriangle3D();
		~CTriangle3D();
		bool InitGeometry(LPDIRECT3DDEVICE9 pd3dDevice);
		VOID SetupMatrices();
		void SetRenderState();
		VOID Render();

	private:
		LPDIRECT3DDEVICE9 m_pd3dDevice = NULL;
		LPDIRECT3DVERTEXBUFFER9 m_pVB = NULL;
	};

	// Cylinder 원통(빛을 켜다)
	struct SVertex3N
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal; // 노말은 빛을 계산하기 위해서 필요합니다.
		// 빛과 면의 각도에 따라 빛에 대한 색이 달라지기 때문입니다.
		// 노말 벡터는 면에 수직인 벡터이여야 합니다.
		// 면에 수직인 벡터 방향의 면은 빛에 반응하여 보이게 되며,
		// 그 반대쪽 면은 안보이게 됩니다.
		// 이것을 컬링이라고 하는데, 컬링모드를 변경시켜서 모두(앞뒤면) 보이게도 할 수 있고,
		// 노말벡터 방향의 면만 보이게 할 수 있습니다.
		enum { FVF = (D3DFVF_XYZ | D3DFVF_NORMAL) };
	};

	class CCylinder3N
	{
	public:
		CCylinder3N();
		~CCylinder3N();
		bool InitGeometry(LPDIRECT3DDEVICE9 pd3dDevice);
		VOID SetupMatrices();
		VOID SetupLights();
		void SetRenderState();
		VOID Render();

	private:
		LPDIRECT3DDEVICE9 m_pd3dDevice = NULL;
		LPDIRECT3DVERTEXBUFFER9 m_pVB = NULL;
	};

	// Texture (이미지를 입히다)
	struct SVertex3DT
	{
		D3DXVECTOR3 position; // The position
		D3DCOLOR color;    // The color
		FLOAT tu, tv;   // The texture coordinates
		enum { FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1) };
	};

	class CCylinder3DT
	{
	public:
		CCylinder3DT();
		~CCylinder3DT();

		bool InitGeometry(LPDIRECT3DDEVICE9 pd3dDevice);
		bool LoadTexture();
		VOID SetupMatrices();
		void SetRenderState();
		void SetTexture();
		VOID Render();

	private:
		LPDIRECT3DDEVICE9       m_pd3dDevice = NULL; // Our rendering device
		LPDIRECT3DVERTEXBUFFER9 m_pVB = NULL; // Buffer to hold vertices
		LPDIRECT3DTEXTURE9      m_pTexture = NULL; // Our texture
	};

	// Axis
	class CAxis
	{
	public:
		CAxis();
		~CAxis();

		bool Init(LPDIRECT3DDEVICE9 pd3dDevice);
		void Render();

	private:
		LPDIRECT3DDEVICE9       m_pd3dDevice = NULL; // Our rendering device
		LPDIRECT3DVERTEXBUFFER9 m_pVB = NULL; // Buffer to hold vertices
	};

	// index buffer tutorial(cube)
	struct MyIndex
	{
		WORD _0, _1, _2;
	};

	class CCube
	{
	public:
		CCube();
		~CCube();

		bool Init(LPDIRECT3DDEVICE9 pd3dDevice);
		void Render();

	private:
		LPDIRECT3DDEVICE9 m_pd3dDevice = NULL;
		LPDIRECT3DVERTEXBUFFER9 m_pVB = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB = NULL;
	};

	// Cell 	
	class CCell
	{
	public:
		CCell();
		~CCell();

		enum {
			none = 0,
			patch_TOP = 1,
			patch_BOT = 2,
			patch_LEFT = 4,
			patch_RIGHT = 8
		};

		bool Init(LPDIRECT3DDEVICE9 pd3dDevice);
		void Render(int lodLevel, int patch);
		// ex) call Render
		// Render(1, CCell::patch_TOP | CCel::patch_LEFT);

	private:
		LPDIRECT3DDEVICE9 m_pd3dDevice = NULL;
		LPDIRECT3DVERTEXBUFFER9 m_pVB = NULL;

		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_0 = NULL;
		
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1 = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_t = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_b = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_l = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_r = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_tb = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_lr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_tl = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_tr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_bl = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_br = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_tbl = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_tbr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_tlr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_blr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_1_tblr = NULL;

		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2 = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_t = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_b = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_l = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_r = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_tb = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_lr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_tl = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_tr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_bl = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_br = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_tbl = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_tbr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_tlr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_blr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_2_tblr = NULL;

		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3 = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_t = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_b = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_l = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_r = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_tb = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_lr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_tl = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_tr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_bl = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_br = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_tbl = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_tbr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_tlr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_blr = NULL;
		LPDIRECT3DINDEXBUFFER9 m_pIB_lod_3_tblr = NULL;

		bool CreateIndexBufferLodLevel1none();
		bool CreateIndexBufferLodLevel1t();
		bool CreateIndexBufferLodLevel1b();
		bool CreateIndexBufferLodLevel1l();
		bool CreateIndexBufferLodLevel1r();
		bool CreateIndexBufferLodLevel1tb();
		bool CreateIndexBufferLodLevel1lr();
		bool CreateIndexBufferLodLevel1tl();
		bool CreateIndexBufferLodLevel1tr();
		bool CreateIndexBufferLodLevel1bl();
		bool CreateIndexBufferLodLevel1br();
		bool CreateIndexBufferLodLevel1tbl();
		bool CreateIndexBufferLodLevel1tbr();
		bool CreateIndexBufferLodLevel1tlr();
		bool CreateIndexBufferLodLevel1blr();
		bool CreateIndexBufferLodLevel1tblr();

		bool CreateIndexBufferLodLevel2none();
		bool CreateIndexBufferLodLevel2t();
		bool CreateIndexBufferLodLevel2b();
		bool CreateIndexBufferLodLevel2l();
		bool CreateIndexBufferLodLevel2r();
		bool CreateIndexBufferLodLevel2tb();
		bool CreateIndexBufferLodLevel2lr();
		bool CreateIndexBufferLodLevel2tl();
		bool CreateIndexBufferLodLevel2tr();
		bool CreateIndexBufferLodLevel2bl();
		bool CreateIndexBufferLodLevel2br();
		bool CreateIndexBufferLodLevel2tbl();
		bool CreateIndexBufferLodLevel2tbr();
		bool CreateIndexBufferLodLevel2tlr();
		bool CreateIndexBufferLodLevel2blr();
		bool CreateIndexBufferLodLevel2tblr();

		bool CreateIndexBufferLodLevel3none();
		bool CreateIndexBufferLodLevel3t();
		bool CreateIndexBufferLodLevel3b();
		bool CreateIndexBufferLodLevel3l();
		bool CreateIndexBufferLodLevel3r();
		bool CreateIndexBufferLodLevel3tb();
		bool CreateIndexBufferLodLevel3lr();
		bool CreateIndexBufferLodLevel3tl();
		bool CreateIndexBufferLodLevel3tr();
		bool CreateIndexBufferLodLevel3bl();
		bool CreateIndexBufferLodLevel3br();
		bool CreateIndexBufferLodLevel3tbl();
		bool CreateIndexBufferLodLevel3tbr();
		bool CreateIndexBufferLodLevel3tlr();
		bool CreateIndexBufferLodLevel3blr();
		bool CreateIndexBufferLodLevel3tblr();

		int count_lod_0 = 0;

		int count_lod_1 = 0;
		int count_lod_1_t = 0;
		int count_lod_1_b = 0;
		int count_lod_1_l = 0;
		int count_lod_1_r = 0;
		int count_lod_1_tb = 0;
		int count_lod_1_lr = 0;
		int count_lod_1_tl = 0;
		int count_lod_1_tr = 0;
		int count_lod_1_bl = 0;
		int count_lod_1_br = 0;
		int count_lod_1_tbl = 0;
		int count_lod_1_tbr = 0;
		int count_lod_1_tlr = 0;
		int count_lod_1_blr = 0;
		int count_lod_1_tblr = 0;

		int count_lod_2 = 0;
		int count_lod_2_t = 0;
		int count_lod_2_b = 0;
		int count_lod_2_l = 0;
		int count_lod_2_r = 0;
		int count_lod_2_tb = 0;
		int count_lod_2_lr = 0;
		int count_lod_2_tl = 0;
		int count_lod_2_tr = 0;
		int count_lod_2_bl = 0;
		int count_lod_2_br = 0;
		int count_lod_2_tbl = 0;
		int count_lod_2_tbr = 0;
		int count_lod_2_tlr = 0;
		int count_lod_2_blr = 0;
		int count_lod_2_tblr = 0;

		int count_lod_3 = 0;
		int count_lod_3_t = 0;
		int count_lod_3_b = 0;
		int count_lod_3_l = 0;
		int count_lod_3_r = 0;
		int count_lod_3_tb = 0;
		int count_lod_3_lr = 0;
		int count_lod_3_tl = 0;
		int count_lod_3_tr = 0;
		int count_lod_3_bl = 0;
		int count_lod_3_br = 0;
		int count_lod_3_tbl = 0;
		int count_lod_3_tbr = 0;
		int count_lod_3_tlr = 0;
		int count_lod_3_blr = 0;
		int count_lod_3_tblr = 0;
	};
}
