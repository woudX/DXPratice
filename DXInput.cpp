#include "DXInput.h"
#include "D3DApp.h"

DirectInput *gdxInput = 0;

DirectInput::DirectInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags)
{
	ZeroMemory(m_cKeyboardState, sizeof(m_cKeyboardState));
	ZeroMemory(&m_obMouseState, sizeof(m_obMouseState));
	
	HR(DirectInput8Create(gd3dApp->getAppInst(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDInput, 0));

	HR(m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, 0));
	HR(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard));
	HR(m_pKeyboard->SetCooperativeLevel(gd3dApp->getMainWnd(), keyboardCoopFlags));
	HR(m_pKeyboard->Acquire());

	HR(m_pDInput->CreateDevice(GUID_SysMouse, &m_pMouse, 0));
	HR(m_pMouse->SetDataFormat(&c_dfDIMouse2));
	HR(m_pMouse->SetCooperativeLevel(gd3dApp->getMainWnd(), mouseCoopFlags));
	HR(m_pMouse->Acquire());
}

void DirectInput::poll()
{
	HRESULT hr = m_pKeyboard->GetDeviceState(sizeof(m_cKeyboardState), (void**)&m_cKeyboardState);
	if (FAILED(hr))
	{
		ZeroMemory(m_cKeyboardState, sizeof(m_cKeyboardState));
		hr = m_pKeyboard->Acquire();
	}

	hr = m_pMouse->GetDeviceState(sizeof(m_obMouseState), (void**)&m_obMouseState);
	if (FAILED(hr))
	{
		ZeroMemory(&m_obMouseState, sizeof(m_obMouseState));
		hr = m_pMouse->Acquire();
	}
}

bool DirectInput::keyDown(char key)
{
	return (m_cKeyboardState[key] & 0x80) != 0;
}

bool DirectInput::mouseButtonDown(int button)
{
	return (m_obMouseState.rgbButtons[button] & 0x80) != 0;
}

float DirectInput::mouseDX()
{
	return (float)m_obMouseState.lX;
}

float DirectInput::mouseDY()
{
	return (float)m_obMouseState.lY;
}

float DirectInput::mouseDZ()
{
	return (float)m_obMouseState.lZ;
}

DirectInput::~DirectInput()
{
	ReleaseCom(m_pDInput);
	m_pKeyboard->Unacquire();
	m_pMouse->Unacquire();
	ReleaseCom(m_pKeyboard);
	ReleaseCom(m_pMouse);
}

