#include "D3DWinApp.h"

D3DWinApp::D3DWinApp(HINSTANCE hInstance, _WndParams wndParams, D3DDEVTYPE devType,DWORD requestedVP)
	:D3DApp(hInstance, wndParams, devType, requestedVP)
{
	srand(time_t(0));

	if (!checkDevCaps())
	{
		MessageBox(0, L"checkDevCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}

	D3DXFONT_DESC fontDesc;
	fontDesc.Height			= 80;
	fontDesc.Width			= 40;
	fontDesc.Weight			= FW_BOLD;
	fontDesc.MipLevels		= 0;
	fontDesc.Italic			= true;
	fontDesc.CharSet		= DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality		= DEFAULT_QUALITY;
	fontDesc.PitchAndFamily	= DEFAULT_PITCH | FF_DONTCARE;
	//_tcscpy(fontDesc.FaceName, __T("Times New Roman"));
	wcscpy_s(fontDesc.FaceName, sizeof("Times New Roman"), _T("Times New Roman"));
	HR(D3DXCreateFontIndirect(gd3dDevice, &fontDesc, &mFont));

	m_pGfs = new GfsStats();

}

bool D3DWinApp::checkDevCaps()
{
	D3DCAPS9 caps;
	md3dObject->GetDeviceCaps(D3DADAPTER_DEFAULT, mDevType, &caps);

	DWORD devBehaviorFlags = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		devBehaviorFlags |= mRequestedVP;
	else
		devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	if (caps.DevCaps & D3DDEVCAPS_PUREDEVICE)
		devBehaviorFlags |= D3DCREATE_PUREDEVICE;

	return true;
}

void D3DWinApp::onLostDevice()
{
	m_pGfs->onLostDevice();
	HR(mFont->OnLostDevice());
}

void D3DWinApp::onResetDevice()
{
	m_pGfs->onResetDevice();
	HR(mFont->OnResetDevice());
}

void D3DWinApp::updateScene(float dt)
{
	m_pGfs->update(dt);
}

void D3DWinApp::drawScene()
{
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255,255,255), 1.0f, 0));

	RECT formatRect;
	GetClientRect(mhMainWnd, &formatRect);

	HR(gd3dDevice->BeginScene());

	mFont->DrawTextW(0, _T("Hello Direct3D"), -1, &formatRect, DT_CENTER | DT_VCENTER
		, D3DCOLOR_XRGB(rand() % 256, rand() % 256, rand() % 256));
	m_pGfs->display();

	HR(gd3dDevice->EndScene());
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

D3DWinApp::~D3DWinApp()
{
	ReleaseCom(mFont);
	delete m_pGfs;
}