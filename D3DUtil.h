//======================================================================
//	D3DUtil.h by Woud (C) 2014 All Rights Reserved.
//	
//	Contains many useful macros
//======================================================================
#ifndef _H_D3DUTIL
#define _H_D3DUTIL

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <DxErr.h>
#include <string>
#include <tchar.h>

//============================================================
//	Namespace Marcos

#define NS_D3D_BEGIN	namespace D3D_LIB {
#define NS_D3D_END		}
#define USING_NS_D3D	using namespace D3D_LIB;

//============================================================
//	Globals for convenient access
class D3DApp;
class DirectInput;

extern D3DApp *gd3dApp;
extern IDirect3DDevice9 *gd3dDevice;
extern DirectInput *gdxInput;
//============================================================
//	Clean up
#define ReleaseCom(x) {if (x) {(x)->Release(); (x) = 0;} }

//============================================================
//	Useful Macro
#define UN_USED_PARAM(var);		//	used for vars which are not used in function

//============================================================
//	Char/WChar in Unicode Mode
#if defined(UNICODE) | defined(_UNICODE)
	#ifndef __T
		#define __T(x) L##x
	#endif
#else
	#ifndef __T
		#define __T(x) x;
	#endif // !__T
#endif

//============================================================
//	Debug
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
		#define HR(x)											\
		{														\
			HRESULT hr = x;										\
			if (FAILED(hr))										\
			{													\
				DXTrace(__FILE__, __LINE__, hr, __T(#x), true);	\
			}													\
		}
	#endif
#else
	#ifndef HR
		#define HR(x) x;
	#endif // !HR
#endif

#endif // !_H_D3DUTIL

