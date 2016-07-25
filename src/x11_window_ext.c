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
        return GL_FALSE;
    }

    // simply reparent and we should be fine!
    XReparentWindow(_glfw.x11.display, window->x11.handle, alienWindow->x11.handle, 0, 0);
    XResizeWindow(_glfw.x11.display, window->x11.handle, alienWindow->width, alienWindow->height);
    return GL_TRUE;
}