#include <iostream>
#include "D3DApp.h"
#include "CubeDemo.h"
#include "DXInput.h"

int WINAPI WinMain(  HINSTANCE hInstance,  HINSTANCE hPrevInstance,  
				   LPSTR lpCmdLine,  int nShowCmd )
{
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif
	

	_WndParams wndParam = {800, 600 , "TestWindow"};
	CubeDemo app(hInstance, wndParam, D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gd3dApp = &app;

	DirectInput di(DISCL_NONEXCLUSIVE | DISCL_FOREGROUND, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	gdxInput = &di;

	return gd3dApp->run();
}