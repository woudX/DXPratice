#ifndef _H_VERTEX
#define _H_VERTEX

#include "D3DApp.h"

struct VertexPos
{
	VertexPos() :pos(0.0f, 0.0f, 0.0f) {}
	VertexPos(float fx, float fy, float fz) :pos(fx, fy, fz) {}
	VertexPos(const D3DXVECTOR3& rhs) :pos(rhs) {}

	D3DXVECTOR3 pos;
	static IDirect3DVertexDeclaration9 *Decl;
};

void InitAllVertexDeclarations();
void DestroyAllVertexDeclarations();

#endif