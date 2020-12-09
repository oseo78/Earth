#pragma once

#include "WindowAPI.h"
#include "DirectX.h"
#include "TestVertex1.h" // for CAxis, for CCube, for CCell
#include "CCamera.h"
#include "IEventReceiver.h"

namespace Earth
{
	class EarthEngine
	{
	public:
		EarthEngine(IEventReceiver* receiver=0);
		~EarthEngine();

		bool ProcessMessage();
		void Process();
		bool BegineScene();
		void Render();
		void EndScene();

		CCamera* GetCamera() { return camera; }

	private:
		CWindowAPI* window = NULL;
		CDirectX* directX = NULL;

		// CAxis, CCamera �� ���߿� CSceneManager �� �ű� ����
		CAxis* axis = NULL;
		CCamera* camera = NULL;

		CCube* cube = NULL;
		CCell* cell = NULL;
	};
}