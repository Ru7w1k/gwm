// Headers
#define UNICODE

#include <Windows.h>
#include <stdio.h>
#include <string>

#include "../../include/gwm/gwm.h"

// Linker Options
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

// Global Variables
FILE  *gpFile = NULL;
bool  gbActiveWindow = false;
bool  gbIsFullScreen = false;
HDC   ghDC = NULL;
HGLRC ghRC = NULL;
HWND  ghWnd = NULL;
DWORD dwStyle;

int gWidth = 800;
int gHeight = 600;

WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

// Callbacks
InitializeCallback   initializeCallback  = NULL;
KeyboardCallback     keyboardCallback    = NULL;  
MouseMoveCallback    mouseMoveCallback   = NULL; 
MouseClickCallback   mouseClickCallback  = NULL;
DisplayCallback      displayCallback     = NULL;   
UpdateCallback       updateCallback      = NULL;    
ReshapeCallback      reshapeCallback     = NULL;   
UninitializeCallback uninitializeCallback = NULL;
 
// Global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void    ToggleFullScreen(void);

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declarations
	void ToggleFullScreen(void);

	// code
	switch (iMsg)
	{

	case WM_SETFOCUS:
		gbActiveWindow = true;
		break;

	case WM_KILLFOCUS:
		gbActiveWindow = false;
		break;

	case WM_SIZE:
        if (reshapeCallback)
		    reshapeCallback(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_KEYDOWN:
        if(keyboardCallback) 
            keyboardCallback(wParam);

		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;

		case 0x46:
			ToggleFullScreen();
			break;
		}
		break;

		// returned from here, to block DefWindowProc
		// We have our own painter
	case WM_ERASEBKGND:
		return(0);
		break;

	case WM_DESTROY:
        uninitialize();
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen()
{
	MONITORINFO MI;

	if (gbIsFullScreen == false)
	{
		dwStyle = GetWindowLong(ghWnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			MI = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(ghWnd, &wpPrev)
				&& GetMonitorInfo(MonitorFromWindow(ghWnd, MONITORINFOF_PRIMARY), &MI))
			{
				SetWindowLong(ghWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghWnd,
					HWND_TOP,
					MI.rcMonitor.left,
					MI.rcMonitor.top,
					MI.rcMonitor.right - MI.rcMonitor.left,
					MI.rcMonitor.bottom - MI.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
		gbIsFullScreen = true;
	}
	else
	{
		SetWindowLong(ghWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghWnd, &wpPrev);
		SetWindowPos(ghWnd,
			HWND_TOP,
			0,
			0,
			0,
			0,
			SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);

		ShowCursor(TRUE);
		gbIsFullScreen = false;
	}
}

int initialize(void)
{
	// function declarations
	void resize(int, int);
	void uninitialize(void);

	// variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;
	GLenum result;

	// code
	// initialize pdf structure
	ZeroMemory((void *)&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	ghDC = GetDC(ghWnd);

	iPixelFormatIndex = ChoosePixelFormat(ghDC, &pfd);
	// iPixelFormatIndex is 1 based, so 0 indicates error
	if (iPixelFormatIndex == 0)
	{
		return(-1);
	}

	if (SetPixelFormat(ghDC, iPixelFormatIndex, &pfd) == FALSE)
	{
		return(-2);
	}

	ghRC = wglCreateContext(ghDC);
	if (ghRC == NULL)
	{
		return(-3);
	}

	if (wglMakeCurrent(ghDC, ghRC) == FALSE)
	{
		return(-4);
	}

	//// Programable Pipeline ////////////////////////////////////////////

	result = glewInit();
	if (result != GLEW_OK) {
		fprintf(gpFile, "GLEW initialization failed..\n");
		uninitialize();
		DestroyWindow(ghWnd);
	}

    if(initializeCallback)
        initializeCallback();

	// clear the depth buffer
	glClearDepth(1.0f);

	// clear the screen by OpenGL
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// enable depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    if(reshapeCallback)
        reshapeCallback(gWidth, gHeight);

	return(0);
}

void uninitialize(void)
{
    if (uninitializeCallback)
        uninitializeCallback();

    	// fullscreen check
	if (gbIsFullScreen == true)
	{
		SetWindowLong(ghWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghWnd, &wpPrev);
		SetWindowPos(ghWnd,
			HWND_TOP,
			0,
			0,
			0,
			0,
			SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);

		ShowCursor(TRUE);
	}

	// break the current context
	if (wglGetCurrentContext() == ghRC)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (ghRC)
	{
		wglDeleteContext(ghRC);
	}

	if (ghDC)
	{
		ReleaseDC(ghWnd, ghDC);
		ghDC = NULL;
	}

	if (gpFile)
	{
		fprintf(gpFile, "Log file is closed...\n");
		fclose(gpFile);
		gpFile = NULL;	
    }
}

void gwmCreateWindow(const char *title, int x, int y, int width, int height)
{
    // variables 
	int iRet = 0;
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;

    gWidth = width;
    gHeight = height;

    HINSTANCE hInstance = (HINSTANCE) GetModuleHandle(NULL);

    size_t size = strlen(title) + 1;
	TCHAR *szClassName = new TCHAR[size];
    mbstowcs_s(&size, szClassName, size, title, _TRUNCATE);

	// code
	// create file for logging
	if (fopen_s(&gpFile, "log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Cannot Create log file!"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log.txt file created...\n");
	}

	// initialization of WNDCLASSEX
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// register class
	RegisterClassEx(&wndclass);

	// create window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szClassName,
		szClassName,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		x,
		y,
		width,
		height,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghWnd = hwnd;

	iRet = initialize();
	if (iRet == -1)
	{
		fprintf(gpFile, "ChoosePixelFormat failed...\n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -2)
	{
		fprintf(gpFile, "SetPixelFormat failed...\n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -3)
	{
		fprintf(gpFile, "wglCreateContext failed...\n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -4)
	{
		fprintf(gpFile, "wglMakeCurrent failed...\n");
		DestroyWindow(hwnd);
	}
	else
	{
		fprintf(gpFile, "initialize() successful...\n");
	}

	ShowWindow(hwnd, SW_NORMAL);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
}

void gwmDestroyWindow(void)
{
    // 
}


void gwmFullScreen()
{
    if (!gbIsFullScreen)
        ToggleFullScreen();
}

void gwmExitFullScreen()
{
    if (gbIsFullScreen)
        ToggleFullScreen();
}


void gwmEventLoop()
{
    // variables
    bool bDone = false;
    MSG msg;

    // code
    while (bDone == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == true) 
            {
                if (updateCallback)
                    updateCallback();
            }

            if (displayCallback)
			{
			    displayCallback();
				fprintf(gpFile, "display called...\n");
			}
		}
    }    
}

void gwmExitEventLoop()
{
    // TODO: post WM_QUIT message
    DestroyWindow(ghWnd);
}


void gwmInitializeCallback(InitializeCallback callback)
{
    initializeCallback = callback;
}


void gwmKeyboardCallback(KeyboardCallback callback)
{
    keyboardCallback = callback;
}

void gwmMouseMoveCallback(MouseMoveCallback callback)
{
    mouseMoveCallback = callback;
}

void gwmMouseClickCallback(MouseClickCallback callback)
{
    mouseClickCallback = callback;
}


void gwmDisplayCallback(DisplayCallback callback)
{
    displayCallback = callback;
}
void gwmUpdateCallback(UpdateCallback callback)
{
    updateCallback = callback;
}
void gwmReshapeCallback(ReshapeCallback callback)
{
    reshapeCallback = callback;
}

void gwmSwapBuffers(void)
{
    SwapBuffers(ghDC);
}
