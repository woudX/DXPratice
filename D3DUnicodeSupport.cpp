//======================================================================
//	D3DUnicodeSupport.h by Woud (C) 2014 All Rights Reserved.
//	
//	Provide all kinds of method to manage the unicode
//======================================================================
#include "D3DUnicodeSupport.h"

USING_NS_D3D

bool strToWStr(const std::string &str, std::wstring &wstr)
{
	bool bRet = true;
	int iLen = (int)str.length();
	wstr.resize(iLen, L' ');

	int nResult = MultiByteToWideChar(CP_ACP, 0,
		(LPCSTR)str.c_str(), iLen, (LPWSTR)wstr.c_str(), iLen);

	if (nResult == 0)
		bRet = false;

	return bRet;
}

WCHAR *charToWChar(char *src)
{
	bool bRet = true;
	WCHAR wszClassName[256]; 
	memset(wszClassName,0,sizeof(wszClassName));  
	MultiByteToWideChar(CP_ACP, 0, src, strlen(src) + 1, wszClassName,  
		sizeof(wszClassName)/sizeof(wszClassName[0]));  
	return wszClassName;
}