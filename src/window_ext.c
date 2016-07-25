//========================================================================
// GLFW 3.2 - www.glfw.org
//------------------------------------------------------------------------
// Copyright (c) 2002-2006 Marcus Geelnard
// Copyright (c) 2006-2010 Camilla Berglund <elmindreda@elmindreda.org>
// Copyright (c) 2012 Torsten Walluhn <tw@mad-cad.net>
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

#include <assert.h>
#include <string.h>
#include <stdlib.h>


// added 3/10/2015 by Gustav Sterbrant
// allows us to create a GLFW window from another window system
GLFWwindow* glfwCreateWindowFromAlien(void* data)
{
	_GLFWalienWindow* alienWindow = (_GLFWalienWindow*)data;

	_GLFWfbconfig fbconfig;
	_GLFWctxconfig ctxconfig;
	_GLFWwndconfig wndconfig;
	_GLFWwindow* window;
	_GLFWwindow* previous;

	_GLFW_REQUIRE_INIT_OR_RETURN(NULL);

	if (alienWindow->width <= 0 || alienWindow->height <= 0)
	{
		_glfwInputError(GLFW_INVALID_VALUE, "Invalid window size");
		return NULL;
	}

	// Set up desired framebuffer config
	fbconfig.redBits		= _glfw.hints.framebuffer.redBits;
	fbconfig.greenBits		= _glfw.hints.framebuffer.greenBits;
	fbconfig.blueBits		= _glfw.hints.framebuffer.blueBits;
	fbconfig.alphaBits		= _glfw.hints.framebuffer.alphaBits;
	fbconfig.depthBits		= _glfw.hints.framebuffer.depthBits;
	fbconfig.stencilBits	= _glfw.hints.framebuffer.stencilBits;
	fbconfig.accumRedBits	= _glfw.hints.framebuffer.accumRedBits;
	fbconfig.accumGreenBits = _glfw.hints.framebuffer.accumGreenBits;
	fbconfig.accumBlueBits	= _glfw.hints.framebuffer.accumBlueBits;
	fbconfig.accumAlphaBits = _glfw.hints.framebuffer.accumAlphaBits;
	fbconfig.auxBuffers		= _glfw.hints.framebuffer.auxBuffers;
	fbconfig.stereo			= _glfw.hints.framebuffer.stereo ? GLFW_TRUE : GLFW_FALSE;
	fbconfig.samples		= _glfw.hints.framebuffer.samples;
	fbconfig.sRGB			= _glfw.hints.framebuffer.sRGB;
	fbconfig.doublebuffer	= _glfw.hints.framebuffer.doublebuffer ? GLFW_TRUE : GLFW_FALSE;

	// Set up desired window config
	wndconfig.width			= alienWindow->width;
	wndconfig.height		= alienWindow->height;
	wndconfig.title			= "";
	wndconfig.resizable		= GLFW_TRUE;
	wndconfig.visible		= _glfw.hints.window.visible ? GLFW_TRUE : GLFW_FALSE;
	wndconfig.decorated		= GLFW_FALSE;

	// Set up desired context config
	ctxconfig.client		= _glfw.hints.context.client;
	ctxconfig.source		= _glfw.hints.context.source;
	ctxconfig.major			= _glfw.hints.context.major;
	ctxconfig.minor			= _glfw.hints.context.minor;
	ctxconfig.forward		= _glfw.hints.context.forward ? GLFW_TRUE : GLFW_FALSE;
	ctxconfig.debug			= _glfw.hints.context.debug ? GLFW_TRUE : GLFW_FALSE;
	ctxconfig.profile		= _glfw.hints.context.profile;
	ctxconfig.robustness	= _glfw.hints.context.robustness;
	ctxconfig.release		= _glfw.hints.context.release;
	ctxconfig.share			= NULL;

	// Check the OpenGL bits of the window config
	if (!_glfwIsValidContextConfig(&ctxconfig))
		return NULL;

	window = calloc(1, sizeof(_GLFWwindow));
	window->next = _glfw.windowListHead;
	_glfw.windowListHead = window;

	window->monitor = NULL;
	window->resizable = wndconfig.resizable;
	window->decorated = wndconfig.decorated;
	window->cursorMode = GLFW_CURSOR_NORMAL;

	// Save the currently current context so it can be restored later
	previous = (_GLFWwindow*)glfwGetCurrentContext();

	// setup alien window, the only thing we do different from the original function
	if (!_glfwPlatformCreateWindowFromAlien(window, alienWindow, &wndconfig, &ctxconfig, &fbconfig))
	{
		glfwDestroyWindow((GLFWwindow*)window);
		glfwMakeContextCurrent((GLFWwindow*)previous);
		return NULL;
	}

	if (ctxconfig.client != GLFW_NO_API)
    {
		glfwMakeContextCurrent((GLFWwindow*)window);

		// Retrieve the actual (as opposed to requested) context attributes
		if (!_glfwRefreshContextAttribs(&ctxconfig))
		{
			glfwDestroyWindow((GLFWwindow*)window);
			glfwMakeContextCurrent((GLFWwindow*)previous);
			return NULL;
		}
	
		glfwMakeContextCurrent((GLFWwindow*)previous);
	}

	if (wndconfig.visible)
	{
		if (wndconfig.focused)
			_glfwPlatformShowWindow(window);
		else
			_glfwPlatformFocusWindow(window);
	}

	return (GLFWwindow*)window;
}
