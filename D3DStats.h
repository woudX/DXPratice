//======================================================================
//	D3DStats.h by Woud (C) 2014 All Rights Reserved.
//	
//	Contains Class which can get D3D running infos
//======================================================================

#ifndef _H_D3DSTATS
#define _H_D3DSTATS

#include "D3DUtil.h"

//============================================================
//	Graphics stats can statistic trangle and vertex using, also it can 
//	calculate the FPS and time-per-frame
class GfsStats
{
public:
	GfsStats();
	~GfsStats();

	void onLostDevice();
	void onResetDevice();

	void addVertices(DWORD n) { m_dwTotalVertices += n; }
	void addTrangles(DWORD n) { m_dwTotalTrangles += n; }
	void subVertices(DWORD n) { m_dwTotalVertices -= n; }
	void subTrangles(DWORD n) { m_dwTotalTrangles -= n; }
	void setVertices(DWORD n) { m_dwTotalVertices = n; }
	void setTranglesCount(DWORD n) { m_dwTotalTrangles = n; }

	void update(float dt);
	void display();
	
	
private:
	ID3DXFont *m_pFont;

	float m_fFps;
	float m_fMillionSecondPerFrame;
	
	DWORD m_dwTotalVertices;
	DWORD m_dwTotalTrangles;
};

#endif // !_H_D3DSTATS
