#include <windows.h>
#include <tchar.h>

//#include "source/WindowAPI.h"
//#include "source/DirectX.h"
//#include "source/TestVertex1.h"
//#include "include/quaternion.h"

#include "source/EarthEngine.h"

// ������Ʈ �Ӽ� - ��Ŀ - �Է� - �߰����Ӽ�
// d3d9.lib
// d3dx9d.lib
// ���� �� �ΰ��� �߰� �ؾ��ϴµ�, �Ʒ��� �ڵ�� �߰� �� ���� �ִ�.
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9d.lib")

// ������Ʈ �Ӽ� - VC ���͸� - ���Ե��͸�
// $(DXSDK_DIR)Include

// ������Ʈ �Ӽ� - VC ���͸� - ���̺귯�����͸�
// $(DXSDK_DIR)Lib\x86

int main() {


	Earth::EarthEngine* engine = new Earth::EarthEngine();

	//Earth::CWindowAPI* window = new Earth::CWindowAPI();
	//Earth::CDirectX* directX = new Earth::CDirectX();

	//window->Initialize();
	//directX->InitD3D(window->GetHwnd());

	//Earth::CAxis* axis = new Earth::CAxis();
	//axis->Init(directX->GetDevice());

	//Earth::core::quaternion q;

	/*
	Earth::CTriangle2D* triangle = new Earth::CTriangle2D();
	Earth::CTriangle3D* t3 = new Earth::CTriangle3D();
	Earth::CCylinder3N* cylinder = new Earth::CCylinder3N();
	Earth::CCylinder3DT* cylinder2 = new Earth::CCylinder3DT();
	triangle->InitVertexBuffer(directX->GetDevice());
	t3->InitGeometry(directX->GetDevice());
	cylinder->InitGeometry(directX->GetDevice());
	cylinder2->InitGeometry(directX->GetDevice());
	cylinder2->LoadTexture();
	*/

	while (true)
	{
		if (engine->ProcessMessage())
			break;
		//if (window->ProcessMessage()==true)
		//	break;// ����

		// Game FrameWork
		//directX->Render();
		//if (directX->BeginScene())
		//{
			/*
			triangle->Render();

			t3->SetupMatrices();
			t3->SetRenderState();
			t3->Render();

			cylinder->SetupMatrices();
			cylinder->SetupLights();
			cylinder->SetRenderState();
			cylinder->Render();

			cylinder2->SetupMatrices();
			cylinder2->SetRenderState();
			cylinder2->SetTexture();
			cylinder2->Render();
			*/
			//axis->Render();

			//directX->EndScene();
		//}

		engine->Process();

		if (engine->BegineScene())
		{
			engine->Render();
			engine->EndScene();
		}

	}

	//directX->Cleanup();

	//delete window;
	//delete directX;
	//delete axis;

	/*
	delete window;
	delete directX;
	delete triangle;
	delete t3;
	delete cylinder;
	delete cylinder2;
	*/

	return 0;
}
