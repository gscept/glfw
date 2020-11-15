#ifndef _glfw3_internal_ext_h_
#define _glfw3_internal_ext_h_
#if 0
// added 3/10/2015 by Gustav Sterbrant
// allows us to create a GLFW window from another window system
typedef struct _GLFWalienWindow _GLFWalienWindow;

/*! @brief Alien window
*
*  Added 3/10/2015 by Gustav Sterbrant
*
*/
struct _GLFWalienWindow
{
	int           width;
	int           height;

	_GLFW_PLATFORM_ALIEN_WINDOW_STATE;
};

/*! @ingroup platform
*
*  Added 3/10/2015 by Gustav Sterbrant
*
*/
int _glfwPlatformCreateWindowFromAlien(_GLFWwindow* window,
	_GLFWalienWindow* alienWindow,
	const _GLFWwndconfig* wndconfig,
	const _GLFWctxconfig* ctxconfig,
	const _GLFWfbconfig* fbconfig);
#endif
#endif // _glfw3_internal_ext_h_