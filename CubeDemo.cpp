#include "CubeDemo.h"
#include "Vertex.h"
#include "DXInput.h"

CubeDemo::CubeDemo(HINSTANCE hInstance, _WndParams wndParams, D3DDEVTYPE devType, DWORD requestedVP)
	:D3DApp(hInstance, wndParams, devType, requestedVP)
{
	srand(time_t(0));

	if (!checkDevCaps())
	{
		MessageBox(0, L"checkDevCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}

	m_pGfs = new GfsStats();

	m_fCameraRadius = 10.0f;
	m_fCameraHeight = 5.0f;
	m_fCameraRotationY = 1.2 * D3DX_PI;

	buildVertexBuffer();
	buildIndexBuffer();

	onResetDevice();
	InitAllVertexDeclarations();
}

bool CubeDemo::checkDevCaps()
{
	D3DCAPS9 caps;
	HR(gd3dDevice->GetDeviceCaps(&caps));

	if (caps.VertexShaderVersion < D3DVS_VERSION(2, 0))
		return false;
	if (caps.PixelShaderVersion < D3DVS_VERSION(2, 0))
		return false;

	return true;
}

void CubeDemo::buildVertexBuffer()
{
	HR(gd3dDevice->CreateVertexBuffer(8 * sizeof(VertexPos), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVB, 0));

	VertexPos *vertices = 0;
	HR(m_pVB->Lock(0, 0, (void**)&vertices, 0));

	vertices[0] = VertexPos(-1.0f, -1.0f, -1.0f);
	vertices[1] = VertexPos(-1.0f, 1.0f, -1.0f);
	vertices[2] = VertexPos(1.0f, 1.0f, -1.0f);
	vertices[3] = VertexPos(1.0f, -1.0f, -1.0f);
	vertices[4] = VertexPos(-1.0f, -1.0f, 1.0f);
	vertices[5] = VertexPos(-1.0f, 1.0f, 1.0f);
	vertices[6] = VertexPos(1.0f, 1.0f, 1.0f);
	vertices[7] = VertexPos(1.0f, -1.0f, 1.0f);

	HR(m_pVB->Unlock());
}

void CubeDemo::buildIndexBuffer()
{
	HR(gd3dDevice->CreateIndexBuffer(36 * sizeof(VertexPos), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, 0));

	WORD *indices = 0;
	HR(m_pIB->Lock(0, 0, (void**)&indices, 0));

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// back side
	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 4; indices[10] = 7; indices[11] = 6;

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

	HR(m_pIB->Unlock());
}

void CubeDemo::buildProjMtx()
{
	float w = (float)md3dPP.BackBufferWidth;
	float h = (float)md3dPP.BackBufferHeight;

	D3DXMatrixPerspectiveFovLH(&m_mtxProj, D3DX_PI * 0.25, w / h, 1.0f, 5000.0f);
}

void CubeDemo::buildViewMtx()
{
	float x = m_fCameraRadius * cosf(m_fCameraRotationY);
	float z = m_fCameraRadius * sinf(m_fCameraRotationY);

	D3DXVECTOR3 pos(x, m_fCameraHeight, z);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&m_mtxView, &pos, &target, &up);
}

void CubeDemo::updateScene(float dt)
{
	//	Update calculate info
	m_pGfs->setVertices(8);
	m_pGfs->setTranglesCount(12);
	m_pGfs->update(dt);

	//	Catch input and re-position camera
	gdxInput->poll();

	if (gdxInput->keyDown(DIK_W))
		m_fCameraHeight += 25.0f * dt;
	else if (gdxInput->keyDown(DIK_S))
		m_fCameraHeight -= 25.0f * dt;

	m_fCameraRotationY += gdxInput->mouseDX() / 50.0f;
	m_fCameraRadius += gdxInput->mouseDY() / 50.0f;

	if (fabs(m_fCameraRotationY) >= 2.0f * D3DX_PI)
		m_fCameraRotationY = 0;
	
	m_fCameraRadius = (m_fCameraRadius < 5.0f) ? 5.0f : m_fCameraRadius;

	buildViewMtx();
}

void CubeDemo::drawScene()
{
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0));

	RECT formatRect;
	GetClientRect(mhMainWnd, &formatRect);

	HR(gd3dDevice->BeginScene());

	HR(gd3dDevice->SetStreamSource(0, m_pVB, 0, sizeof(VertexPos)));
	HR(gd3dDevice->SetIndices(m_pIB));
	HR(gd3dDevice->SetVertexDeclaration(VertexPos::Decl));

	D3DXMATRIX mtxW;
	D3DXMatrixIdentity(&mtxW);
	HR(gd3dDevice->SetTransform(D3DTS_WORLD, &mtxW))
	HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProj));
	HR(gd3dDevice->SetTransform(D3DTS_VIEW, &m_mtxView));
	HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME));
	HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12));

	m_pGfs->display();

	HR(gd3dDevice->EndScene());
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

void CubeDemo::onLostDevice()
{
	m_pGfs->onLostDevice();
}

void CubeDemo::onResetDevice()
{
	m_pGfs->onResetDevice();
	buildProjMtx();
}

CubeDemo::~CubeDemo()
{
	delete m_pGfs;
	ReleaseCom(m_pVB);
	ReleaseCom(m_pIB);
	DestroyAllVertexDeclarations();
}
