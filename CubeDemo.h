#ifndef _H_CUBEDEMO
#define _H_CUBEDEMO

#include "D3DApp.h"
#include "D3DStats.h"

class CubeDemo : public D3DApp
{
public:
	CubeDemo(HINSTANCE hInstance, _WndParams wndParams, D3DDEVTYPE devType, DWORD requestedVP);
	~CubeDemo();

	virtual void updateScene(float dt);
	virtual void drawScene();
	virtual void onLostDevice();
	virtual void onResetDevice();
	virtual bool checkDevCaps();

	void buildVertexBuffer();
	void buildIndexBuffer();
	void buildProjMtx();
	void buildViewMtx();
	IDirect3DVertexBuffer9 *m_pVB;
	IDirect3DIndexBuffer9 *m_pIB;
	float m_fCameraRotationY;
	float m_fCameraRadius;
	float m_fCameraHeight;
	D3DXMATRIX m_mtxView;
	D3DXMATRIX m_mtxProj;

	GfsStats *m_pGfs;
};

#endif