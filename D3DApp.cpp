//======================================================================
//	D3DApp.h by Woud 2014 (C) All Rights Reserved.
//======================================================================

#include "D3DApp.h"
#include <windows.h>

D3DApp *gd3dApp = 0;
IDirect3DDevice9 *gd3dDevice = 0;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (gd3dApp != 0)
		return gd3dApp->WndProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

D3DApp::D3DApp(HINSTANCE hInstance, _WndParams wndParams, D3DDEVTYPE devType, DWORD requestedVP)
	:mhAppInst(hInstance)
	,mMainWndCaption(wndParams.wndName)
	,mWidth(wndParams.width)
	,mHeight(wndParams.height)
	,mDevType(devType)
	,mRequestedVP(requestedVP)
	,mAppPaused(false)
{
	ZeroMemory(&md3dPP, sizeof(md3dPP));

	initMainWindow();
	initDirect3D();
}

void D3DApp::drawScene()
{

}

HINSTANCE D3DApp::getAppInst()
{
	return mhAppInst;
}

HWND D3DApp::getMainWnd()
{
	return mhMainWnd;
}

void D3DApp::initMainWindow()
{
	//	create the main app window
	WNDCLASS wc;

	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= MainWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= mhAppInst;
	wc.hIcon			= LoadIcon(0, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= L"Direct3DAppTest";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass failed!", 0, 0);
		return ;
	}

	mhMainWnd = 0;
	mhMainWnd = CreateWindow(L"Direct3DAppTest", L"Direct3DAppTest", 
		WS_EX_TOPMOST, 100, 100, mWidth, mHeight, 0, 0, mhAppInst, 0);

	if (!mhMainWnd)
	{
		MessageBox(0, L"CreateWindow failed!", 0, 0);
		return ;
	}

	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);
}

LRESULT D3DApp::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT clientRect;
	switch (msg)
	{
	case WM_SIZE:
		{
			md3dPP.BackBufferWidth = LOWORD(lParam);
			md3dPP.BackBufferHeight = HIWORD(lParam);
			onLostDevice();
			gd3dDevice->Reset(&md3dPP);
			onResetDevice();
			break;
		}
	case WM_EXITSIZEMOVE:
		{
			GetClientRect(mhMainWnd, &clientRect);
			md3dPP.BackBufferWidth = clientRect.right;
			md3dPP.BackBufferHeight = clientRect.bottom;
			onLostDevice();
			HR(gd3dDevice->Reset(&md3dPP));
			onResetDevice();
			break;
		}
	case WM_CLOSE:
		{
			DestroyWindow(mhMainWnd);
			return 0;
		}
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE)
				enableFullScreenMode(false);
			else if (wParam == 'F')
				enableFullScreenMode(true);
			else if (wParam == 'Q')
			{
				DestroyWindow(mhMainWnd);
				return 0;
			}
		}
	default:
		break;
	}
	
	return DefWindowProc(mhMainWnd, msg, wParam,lParam);
}

void D3DApp::initDirect3D()
{
	//	No.1 Create IDirect3D9
	md3dObject = Direct3DCreate9(D3D_SDK_VERSION);

	//	No.2 Checking HAL support
	D3DDISPLAYMODE mode;
	md3dObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
	HR(md3dObject->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format, mode.Format, true));
	HR(md3dObject->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false));

	//	No.3 Checking for hardware processing
	D3DCAPS9 caps;
	HR(md3dObject->GetDeviceCaps(D3DADAPTER_DEFAULT, mDevType, &caps));
	DWORD devBehaviorFlag = 0;
	if (caps.DevCaps && D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		devBehaviorFlag |= mRequestedVP;
	else
		devBehaviorFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//	if pure device and HW T&L supported
	if (caps.DevCaps && D3DDEVCAPS_PUREDEVICE && devBehaviorFlag && D3DCREATE_HARDWARE_VERTEXPROCESSING)
		devBehaviorFlag |= D3DCREATE_PUREDEVICE;

	//	No.4 Init D3DPresent_Parameters
	md3dPP.BackBufferWidth		= 800;
	md3dPP.BackBufferHeight		= 600;
	md3dPP.BackBufferFormat		= D3DFMT_X8R8G8B8;
	md3dPP.BackBufferCount		= 1;
	md3dPP.MultiSampleType		= D3DMULTISAMPLE_NONE;
	md3dPP.MultiSampleQuality	= 0;
	md3dPP.SwapEffect			= D3DSWAPEFFECT_DISCARD;
	md3dPP.hDeviceWindow		= mhMainWnd;
	md3dPP.Windowed				= true;
	md3dPP.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;
	md3dPP.AutoDepthStencilFormat = D3DFMT_D24S8;
	md3dPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	md3dPP.EnableAutoDepthStencil = true;

	//	No.5 Create IDirect3DDevice9
	
	gd3dDevice = 0;
	HR(md3dObject->CreateDevice(D3DADAPTER_DEFAULT, mDevType, mhMainWnd, devBehaviorFlag, &md3dPP, &gd3dDevice));

}

void D3DApp::enableFullScreenMode(bool enable)
{
	if (enable)
	{
		//	Switch to full screen
		if (!md3dPP.Windowed)
			return ;
		
		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		md3dPP.BackBufferWidth = width;
		md3dPP.BackBufferHeight = height;
		md3dPP.BackBufferFormat = D3DFMT_X8R8G8B8;
		md3dPP.Windowed = false;

		SetWindowLongPtr(mhMainWnd, GWL_STYLE, WS_POPUP);
		SetWindowPos(mhMainWnd, HWND_TOP, 0, 0, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);
	}
	else
	{
		if (md3dPP.Windowed)
			return ;

		RECT R = {0, 0, 800, 600};
		AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

		md3dPP.BackBufferWidth = 800;
		md3dPP.BackBufferHeight = 600;
		md3dPP.BackBufferFormat = D3DFMT_UNKNOWN;
		md3dPP.Windowed = true;

		SetWindowLongPtr(mhMainWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(mhMainWnd, HWND_TOP, 100, 100, R.right, R.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);
	}

	onLostDevice();
	HR(gd3dDevice->Reset(&md3dPP));
	onResetDevice();
}

bool D3DApp::isDeviceLost()
{
	HRESULT hr = gd3dDevice->TestCooperativeLevel();

	if (hr == D3DERR_DEVICELOST)
	{
		Sleep(20);
		return true;
	}
	else if (hr == D3DERR_DRIVERINTERNALERROR)
	{
		MessageBox(0, L"Internal Driver Error .. Exiting", 0, 0);
		PostQuitMessage(0);
		return true;
	}
	else if (hr == D3DERR_DEVICENOTRESET)
	{
		onLostDevice();
		HR(gd3dDevice->Reset(&md3dPP));
		onResetDevice();
		return false;
	}
	else
		return false;

}

int D3DApp::run()
{
	LARGE_INTEGER prevTimeStamp;
	LARGE_INTEGER nowTimeStamp;
	LARGE_INTEGER timeStampFreq;

	QueryPerformanceCounter(&prevTimeStamp);
	QueryPerformanceFrequency(&timeStampFreq);
	float secPerCnt =  1.0f / timeStampFreq.QuadPart;

	MSG msg;
	msg.message = WM_NULL;

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (mAppPaused)
			{
				Sleep(20);
				continue;
			}
			
			if (!isDeviceLost())
			{
				QueryPerformanceCounter(&nowTimeStamp);
				float dt = (nowTimeStamp.QuadPart - prevTimeStamp.QuadPart) * secPerCnt;

				if (dt > 1 / 60.0f)
				{
					updateScene(dt);	
					drawScene();

					prevTimeStamp.QuadPart = nowTimeStamp.QuadPart;		
				}
				
			}
		}
		
	}

	return msg.wParam;
}

D3DApp::~D3DApp()
{
	ReleaseCom(md3dObject);
	ReleaseCom(gd3dDevice);
}