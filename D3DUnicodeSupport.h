//======================================================================
//	D3DUnicodeSupport.h by Woud (C) 2014 All Rights Reserved.
//	
//	Provide all kinds of method to manage the unicode
//======================================================================

#ifndef _H_D3DUNICODESUPPORT
#define _H_D3DUNICODESUPPORT

#include "D3DUtil.h"

NS_D3D_BEGIN

extern bool strToWStr(const std::string &str, std::wstring &wstr);
extern WCHAR* charToWChar(char *src);
//extern bool wStrToStr(const std::wstring &Str, std::string &str);

NS_D3D_END

#endif