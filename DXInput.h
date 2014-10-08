//======================================================================
//	DXInput.h by Woud (C) 2014 All Rights Reserved.
//	
//	Provide input function, such as mouse and keyboard
//======================================================================

#ifndef _H_DXINPUT
#define _H_DXINPUT

#include "D3DUtil.h"

DirectInput *gdxInput;

class DirectInput
{
public:
	DirectInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags);
	~DirectInput();

	void poll();
	bool keyDown(char key);
	bool mouseButtonDown(int button);
	float mouseDX();
	float mouseDY();
	float mouseDZ();

private:
	DirectInput(const DirectInput &rhs);
	DirectInput &operator = (const DirectInput& rhs);

	IDirectInput8 *m_pDInput;
	IDirectInputDevice8 *m_pKeyboard;
	char m_cKeyboardState[256];
	IDirectInputDevice8 *m_pMouse;
	DIMOUSESTATE2 m_obMouseState;

};

#endif // !_H_DXINPUT
