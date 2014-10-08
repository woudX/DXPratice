//======================================================================
//	D3DStats.h by Woud (C) 2014 All Rights Reserved.
//======================================================================
#include "D3DUnicodeSupport.h"
#include "D3DStats.h"

GfsStats::GfsStats()
	:m_pFont(NULL)
	,m_dwTotalTrangles(0)
	,m_dwTotalVertices(0)
	,m_fMillionSecondPerFrame(0.0f)
	,m_fFps(0.0f)
{
	D3DXFONT_DESC fontDesc;
	fontDesc.Height			= 20;
	fontDesc.Width			= 8;
	fontDesc.Weight			= FW_NORMAL;
	fontDesc.MipLevels		= 0;
	fontDesc.Italic			= false;
	fontDesc.CharSet		= DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality		= DEFAULT_QUALITY;
	fontDesc.PitchAndFamily	= DEFAULT_PITCH | FF_DONTCARE;
	//_tcscpy(fontDesc.FaceName, __T("Times New Roman"));
	wcscpy_s(fontDesc.FaceName, sizeof("Times New Roman"), _T("Times New Roman"));
	HR(D3DXCreateFontIndirect(gd3dDevice, &fontDesc, &m_pFont));
}

void GfsStats::onLostDevice()
{
	HR(m_pFont->OnLostDevice());
}

void GfsStats::onResetDevice()
{
	HR(m_pFont->OnResetDevice());
}

void GfsStats::update(float dt)
{
	static float numFrames = 0.0f;
	static float timeElapsed = 0.0f;

	numFrames += 1.0f;
	timeElapsed += dt;

	if (timeElapsed >= 1.0f)
	{
		m_fFps = numFrames;
		m_fMillionSecondPerFrame = timeElapsed * 1000.0f / m_fFps;

		numFrames = 0.0f;
		timeElapsed = 0.0f;
	}
}

void GfsStats::display()
{
	//	makes static so memory is not allocated every frame
	static char buffer[256];

	sprintf_s(buffer, 256, "Frames Per Second = %.02f\n"
		"Milliseconds Per Frame = %.4f\n"
		"Triangle Count = %d\n"
		"Vertex Count = %d\n", m_fFps, m_fMillionSecondPerFrame, m_dwTotalTrangles, m_dwTotalVertices);

	RECT R = {5, 5, 0, 0};
	
	WCHAR wszClassName[256];  
	memset(wszClassName,0,sizeof(wszClassName));  
	MultiByteToWideChar(CP_ACP,0,buffer,strlen(buffer)+1,wszClassName,  
		sizeof(wszClassName)/sizeof(wszClassName[0]));  

	m_pFont->DrawTextW(0, wszClassName, -1, &R, DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));
}

GfsStats::~GfsStats()
{
	ReleaseCom(m_pFont);
}