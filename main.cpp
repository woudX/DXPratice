#include <Windows.h>
#include <d3dx9.h>
#include <MMSystem.h>

/**
 *	Global
 */ 

IDirect3DDevice9  *device = 0;
typedef bool (*ptr_display)(float dt);

IDirect3DVertexBuffer9 *pVB = 0;
IDirect3DIndexBuffer9 *pIB = 0;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

/**
 *	Data Structure
 */

struct Vertex
{
	Vertex() {};
	Vertex(float x, float y, float z):m_x(x),m_y(y),m_z(z) { }

	float m_x, m_y, m_z;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ;



namespace dxtest {

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	bool InitialD3D(HINSTANCE hInstance, 
		int width, int height, 
		bool windowed, 
		D3DDEVTYPE deviceType,
		IDirect3DDevice9** device);

	int EnterMessageLoop(ptr_display sel);
}

int dxtest::EnterMessageLoop(ptr_display sel)
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	static float lastTime = (float)timeGetTime();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0 ,0 ,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float currTime = (float)timeGetTime();
			float deltaTime = (currTime - lastTime) * 0.001f;

			sel(deltaTime);
			lastTime = currTime;
		}
	}

	return msg.wParam;
}

bool dxtest::InitialD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device)
{
	//	create the main app window
	WNDCLASS wc;
	
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= (WNDPROC)dxtest::WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(0, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= L"Direct3DAppTest";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass failed!", 0, 0);
		return false;
	}

	HWND hwnd = 0;
	hwnd = CreateWindow(L"Direct3DAppTest", L"Direct3DAppTest", 
		WS_EX_TOPMOST, 100, 100, width, height, 0, 0, hInstance, 0);

	if (!hwnd)
	{
		MessageBox(0, L"CreateWindow failed!", 0, 0);
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	// Init D3D

	HRESULT hr = 0;

	// Step 1: Create IDirect3D9 object
	IDirect3D9 *d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d9)
	{
		MessageBox(0, L"Direct3Dcreate9 failed!", 0, 0);
		return false;
	}

	// Step 2: check for hardware vp
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth			= width;
	d3dpp.BackBufferHeight			= height;
	d3dpp.BackBufferFormat			= D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount			= 1;
	d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality		= 0;
	d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow				= hwnd;
	d3dpp.Windowed					= windowed;
	d3dpp.EnableAutoDepthStencil	= true;
	d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
	d3dpp.Flags						= 0;
	d3dpp.FullScreen_RefreshRateInHz= D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 4: Create device
	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		deviceType,
		hwnd,
		vp,
		&d3dpp,
		device);

	if (FAILED(hr))
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, device);

		if (FAILED(hr))
		{
			d3d9->Release();
			MessageBox(0, L"CreateDevice failed!", 0, 0);
			return false;
		}
	}

	d3d9->Release();

	return true;
}

LRESULT CALLBACK dxtest::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(hwnd);
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool MainDisplay(float dt)
{
	if (device)
	{
		D3DXMATRIX rX, rY;

		D3DXMatrixRotationX(&rX, D3DX_PI / 4.0f);
		static float y = 0.0f;
		D3DXMatrixRotationY(&rY, y);
		y += dt;

		if (y > D3DX_PI * 2.0f)
			y = 0.0f;

		D3DXMATRIX p = rX * rY;
		device->SetTransform(D3DTS_WORLD, &p);

		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		device->BeginScene();

		device->SetStreamSource(0, pVB, 0, sizeof(Vertex));
		device->SetIndices(pIB);
		device->SetFVF(Vertex::FVF);

		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
		device->EndScene();
		device->Present(0, 0, 0 ,0);
	}

	return true;
}

bool Setup()
{
	//	create buffer
	
	device->CreateVertexBuffer(
		8 * sizeof(Vertex), 
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&pVB,
		0);

	device->CreateIndexBuffer(
		8 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pIB,
		0);

	//	write vertex and index

	Vertex *vertices;
	pVB->Lock(0, 0, (void**) &vertices, 0);
	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f);
	vertices[1] = Vertex(-1.0f,  1.0f, -1.0f);
	vertices[2] = Vertex( 1.0f,  1.0f, -1.0f);
	vertices[3] = Vertex( 1.0f, -1.0f, -1.0f);
	vertices[4] = Vertex(-1.0f, -1.0f,  1.0f);
	vertices[5] = Vertex(-1.0f,  1.0f,  1.0f);
	vertices[6] = Vertex( 1.0f,  1.0f,  1.0f);
	vertices[7] = Vertex( 1.0f, -1.0f,  1.0f);
	pVB->Unlock();

	WORD *indices = 0;
	pIB->Lock(0, 0, (void**) &indices, 0);

	// front side
	indices[0]  = 0; indices[1]  = 1; indices[2]  = 2;
	indices[3]  = 0; indices[4]  = 2; indices[5]  = 3;

	// back side
	indices[6]  = 4; indices[7]  = 6; indices[8]  = 5;
	indices[9]  = 4; indices[10] = 7; indices[11] = 6;

	// left side
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	// right side
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	// top
	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;

	// bottom
	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	pIB->Unlock();

	//	set position
	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &position, &target, &up);

	device->SetTransform(D3DTS_VIEW, &V);

	//	projection matrix
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f,
		(float) SCREEN_WIDTH / (float) SCREEN_HEIGHT,
		1.0f,
		1000.0f);

	device->SetTransform(D3DTS_PROJECTION, &proj);

	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	return true;
}

bool CleanUp()
{
	//delete pVB;
	//delete pIB;

	return true;
}

int WINAPI WinMain( 
	__in HINSTANCE hInstance, 
	__in_opt HINSTANCE hPrevInstance, 
	__in LPSTR lpCmdLine, 
	__in int nShowCmd )
{
	if (!dxtest::InitialD3D(hInstance, 800, 600, true, D3DDEVTYPE_HAL, &device))
	{
		MessageBox(0, L"InitialD3D failed", 0, 0);
		return 0;
	}

	Setup();
	dxtest::EnterMessageLoop(MainDisplay);
	CleanUp();
	return 0;
}