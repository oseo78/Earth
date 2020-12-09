#include "EarthEngine.h"

namespace Earth
{
	CCamera* g_pCamera;// 나중에 SceneManager 에 의해 얻도록 교체할예정
	EarthEngine::EarthEngine(IEventReceiver* receiver)
	{
		camera = new CCamera();
		g_pCamera = camera;

		window = new CWindowAPI(receiver);
		directX = new CDirectX();
		axis = new CAxis();		
		cube = new CCube();
		cell = new CCell();

		window->Initialize();
		directX->InitD3D(window->GetHwnd());
		axis->Init(directX->GetDevice());		
		cube->Init(directX->GetDevice());
		cell->Init(directX->GetDevice());
	}

	EarthEngine::~EarthEngine()
	{
		directX->Cleanup();
		delete window;
		delete directX;
		delete axis;
		delete cube;
		delete cell;
	}

	bool EarthEngine::ProcessMessage()
	{
		return window->ProcessMessage();
	}

	void EarthEngine::Process()
	{
		camera->Process(window->GetHwnd());
	}

	bool EarthEngine::BegineScene()
	{
		return directX->BeginScene();
	}
	
	void EarthEngine::Render()
	{
		camera->SetViewMatrix(directX->GetDevice());
		axis->Render();
		cube->Render();

		cell->Render(0, CCell::none);

		//cell->Render(1, CCell::none);
		//cell->Render(1, CCell::patch_TOP);
		//cell->Render(1, CCell::patch_BOT);
		//cell->Render(1, CCell::patch_LEFT);
		//cell->Render(1, CCell::patch_RIGHT);
		//cell->Render(1, CCell::patch_TOP | CCell::patch_BOT);
		//cell->Render(1, CCell::patch_TOP | CCell::patch_LEFT);
		//cell->Render(1, CCell::patch_TOP | CCell::patch_RIGHT);
		//cell->Render(1, CCell::patch_LEFT | CCell::patch_RIGHT);
		//cell->Render(1, CCell::patch_BOT | CCell::patch_LEFT);
		//cell->Render(1, CCell::patch_BOT | CCell::patch_RIGHT);
		//cell->Render(1, CCell::patch_TOP | CCell::patch_BOT | CCell::patch_LEFT);		
		//cell->Render(1, CCell::patch_TOP | CCell::patch_BOT | CCell::patch_RIGHT);
		//cell->Render(1, CCell::patch_TOP | CCell::patch_LEFT | CCell::patch_RIGHT);
		//cell->Render(1, CCell::patch_BOT | CCell::patch_LEFT | CCell::patch_RIGHT);
		//cell->Render(1, CCell::patch_TOP | CCell::patch_BOT | CCell::patch_LEFT | CCell::patch_RIGHT);

		//cell->Render(2, CCell::none);
		//cell->Render(2, CCell::patch_TOP);
		//cell->Render(2, CCell::patch_BOT);
		//cell->Render(2, CCell::patch_LEFT);
		//cell->Render(2, CCell::patch_RIGHT);		
		//cell->Render(2, CCell::patch_TOP | CCell::patch_BOT);		
		//cell->Render(2, CCell::patch_TOP | CCell::patch_LEFT);
		//cell->Render(2, CCell::patch_TOP | CCell::patch_RIGHT);
		//cell->Render(2, CCell::patch_LEFT | CCell::patch_RIGHT);
		//cell->Render(2, CCell::patch_BOT | CCell::patch_LEFT);
		//cell->Render(2, CCell::patch_BOT | CCell::patch_RIGHT);
		//cell->Render(2, CCell::patch_TOP | CCell::patch_BOT | CCell::patch_LEFT);		
		//cell->Render(2, CCell::patch_TOP | CCell::patch_BOT | CCell::patch_RIGHT);
		//cell->Render(2, CCell::patch_TOP | CCell::patch_LEFT | CCell::patch_RIGHT);
		//cell->Render(2, CCell::patch_BOT | CCell::patch_LEFT | CCell::patch_RIGHT);
		//cell->Render(2, CCell::patch_TOP | CCell::patch_BOT | CCell::patch_LEFT | CCell::patch_RIGHT);

		//cell->Render(3, CCell::none);
		//cell->Render(3, CCell::patch_TOP);
		//cell->Render(3, CCell::patch_BOT);
		//cell->Render(3, CCell::patch_LEFT);
		//cell->Render(3, CCell::patch_RIGHT);
		//cell->Render(3, CCell::patch_TOP | CCell::patch_BOT);
		//cell->Render(3, CCell::patch_TOP | CCell::patch_LEFT);
		//cell->Render(3, CCell::patch_TOP | CCell::patch_RIGHT);
		//cell->Render(3, CCell::patch_LEFT | CCell::patch_RIGHT);
		//cell->Render(3, CCell::patch_BOT | CCell::patch_LEFT);
		//cell->Render(3, CCell::patch_BOT | CCell::patch_RIGHT);
		//cell->Render(3, CCell::patch_TOP | CCell::patch_BOT | CCell::patch_LEFT);		
		//cell->Render(3, CCell::patch_TOP | CCell::patch_BOT | CCell::patch_RIGHT);
		//cell->Render(3, CCell::patch_TOP | CCell::patch_LEFT | CCell::patch_RIGHT);
		//cell->Render(3, CCell::patch_BOT | CCell::patch_LEFT | CCell::patch_RIGHT);
		//cell->Render(3, CCell::patch_TOP | CCell::patch_BOT | CCell::patch_LEFT | CCell::patch_RIGHT);
	}

	void EarthEngine::EndScene()
	{
		directX->EndScene();
	}
}