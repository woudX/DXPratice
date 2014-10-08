#ifndef _H_D3DWINAPP
#define _H_D3DWINAPP

#include "D3DApp.h"
#include "D3DStats.h"

class D3DWinApp : public D3DApp
{
public:
	D3DWinApp(HINSTANCE hInstance, _WndParams wndParams, D3DDEVTYPE devType, DWORD requestedVP);
	virtual ~D3DWinApp();

	virtual bool checkDevCaps();
	virtual void onLostDevice();
	virtual void onResetDevice();
	virtual void updateScene(float dt);
	virtual void drawScene();

	ID3DXFont *mFont;
	GfsStats *m_pGfs;
};

#endif

