//======================================================================
//	D3DApp.h by Woud 2014 (C) All Rights Reserved.
//======================================================================

#ifndef _H_D3DAPP
#define	_H_D3DAPP

#include "D3DUtil.h"

//============================================================
//	_WndParams contains windows params
class _WndParams
{
public:
	int width;
	int height;

	std::string wndName;
};


//============================================================
//	D3DApp is an important class which contains basic methods and datas
class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance, _WndParams wndParams, D3DDEVTYPE devType, DWORD requestedVP);
	virtual ~D3DApp();

	HINSTANCE getAppInst();
	HWND	  getMainWnd();

	virtual bool checkDevCaps() { return true; }
	virtual void onLostDevice() { }
	virtual void onResetDevice() { }
	virtual void updateScene(float dt) { UN_USED_PARAM(dt); }
	virtual void drawScene();

	virtual void initMainWindow();
	virtual void initDirect3D();
	virtual LRESULT WndProc(UINT msg, WPARAM wParam, LPARAM lParam);
	virtual int run();

	void enableFullScreenMode(bool enable);
	bool isDeviceLost();

protected:

	std::string mMainWndCaption;
	D3DDEVTYPE  mDevType;
	DWORD		mRequestedVP;

	int mWidth;
	int mHeight;

	HINSTANCE	mhAppInst;
	HWND		mhMainWnd;
	IDirect3D9	*md3dObject;
	bool		mAppPaused;
	D3DPRESENT_PARAMETERS md3dPP;
};

#endif // ! _H_D3DAPP




