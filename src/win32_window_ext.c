//========================================================================
// GLFW 3.2 Win32 - www.glfw.org
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2010 Camilla Berglund <elmindreda@elmindreda.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

#include "internal.h"
#include "internal_ext.h"

#include <windowsx.h>
#include <shellapi.h>

// added 3/10/2015 by Gustav Sterbrant
// allows us to create a GLFW window from another window system
int _glfwPlatformCreateWindowFromAlien(_GLFWwindow* window,
	_GLFWalienWindow* alienWindow,
	const _GLFWwndconfig* wndconfig,
	const _GLFWctxconfig* ctxconfig,
	const _GLFWfbconfig* fbconfig)
{
	// just copy color map and window handle
	if (!_glfwPlatformCreateWindow(window, wndconfig, ctxconfig, fbconfig))
	{
		return GLFW_FALSE;
	}

	// just copy window handle
	SetParent(window->win32.handle, alienWindow->win32.hwnd);
	MoveWindow(window->win32.handle, 0, 0, alienWindow->width, alienWindow->height, FALSE);
	return GLFW_TRUE;
}

// Merged from branch SetWindowMonitor
static getWindowStyle(const _GLFWwindow* window)
{
	DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (window->decorated && window->monitor == NULL)
	{
		style |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

		if (window->resizable)
			style |= WS_MAXIMIZEBOX | WS_SIZEBOX;
	}
	else
		style |= WS_POPUP;

	return style;
}

// Merged from branch SetWindowMonitor
// Returns the extended window style for the specified window configuration
//
static getWindowExStyle(const _GLFWwindow* window)
{
	DWORD style = WS_EX_APPWINDOW;

	if (window->decorated && window->monitor == NULL)
		style |= WS_EX_WINDOWEDGE;

	return style;
}

// Merged from branch SetWindowMonitor
static void getFullWindowSize(_GLFWwindow* window,
	int clientWidth, int clientHeight,
	int* fullWidth, int* fullHeight)
{
	RECT rect = { 0, 0, clientWidth, clientHeight };
	AdjustWindowRectEx(&rect, getWindowStyle(window),
		FALSE, getWindowExStyle(window));
	*fullWidth = rect.right - rect.left;
	*fullHeight = rect.bottom - rect.top;
}

// Merged from branch SetWindowMonitor
// Change the current video mode
//
GLFWbool _glfwSetVideoMode(_GLFWmonitor* monitor, const GLFWvidmode* desired)
{
	GLFWvidmode current;
	const GLFWvidmode* best;
	DEVMODEW dm;

	best = _glfwChooseVideoMode(monitor, desired);
	_glfwPlatformGetVideoMode(monitor, &current);
	if (_glfwCompareVideoModes(&current, best) == 0)
		return GLFW_TRUE;

	ZeroMemory(&dm, sizeof(dm));
	dm.dmSize = sizeof(DEVMODEW);
	dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL |
		DM_DISPLAYFREQUENCY;
	dm.dmPelsWidth = best->width;
	dm.dmPelsHeight = best->height;
	dm.dmBitsPerPel = best->redBits + best->greenBits + best->blueBits;
	dm.dmDisplayFrequency = best->refreshRate;

	if (dm.dmBitsPerPel < 15 || dm.dmBitsPerPel >= 24)
		dm.dmBitsPerPel = 32;

	if (ChangeDisplaySettingsExW(monitor->win32.adapterName,
		&dm,
		NULL,
		CDS_FULLSCREEN,
		NULL) != DISP_CHANGE_SUCCESSFUL)
	{
		_glfwInputError(GLFW_PLATFORM_ERROR, "Win32: Failed to set video mode");
		return GLFW_FALSE;
	}

	monitor->win32.modeChanged = GLFW_TRUE;
	return GLFW_TRUE;
}

// Merged from branch SetWindowMonitor
// Restore the previously saved (original) video mode
//
void _glfwRestoreVideoMode(_GLFWmonitor* monitor)
{
	if (monitor->win32.modeChanged)
	{
		ChangeDisplaySettingsExW(monitor->win32.adapterName,
			NULL, NULL, CDS_FULLSCREEN, NULL);
		monitor->win32.modeChanged = GLFW_FALSE;
	}
}
