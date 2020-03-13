#ifndef __gwm_h__
#define __gwm_h__

/*
 * Generic Window Manager
 *
 * A simple common wrapper over multiple window managers.
 */

// Headers
#define UNICODE

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <gl/GL.h>
#include <gl/GLU.h>

// Linker Options
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

typedef void (*InitializeCallback)      (void);
typedef void (*UninitializeCallback)    (void);
typedef void (*KeyboardCallback)        (unsigned int key);
typedef void (*MouseMoveCallback)       (int x, int y);
typedef void (*MouseClickCallback)      (unsigned int key, int x, int y);
typedef void (*KeyboardCallback)        (unsigned int key);
typedef void (*DisplayCallback)         (void);
typedef void (*UpdateCallback)          (void);
typedef void (*ReshapeCallback)         (int width, int height);


void gwmCreateWindow(const char *title, int x, int y, int width, int height);
void gwmDestroyWindow(void);

void gwmInitializeCallback(InitializeCallback callback);
void gwmUninitializeCallback(UninitializeCallback callback); 

void gwmFullScreen();
void gwmExitFullScreen();

void gwmEventLoop();
void gwmExitEventLoop();

void gwmKeyboardCallback(KeyboardCallback callback);
void gwmMouseMoveCallback(MouseMoveCallback callback);
void gwmMouseClickCallback(MouseClickCallback callback);

void gwmDisplayCallback(DisplayCallback callback);
void gwmUpdateCallback(UpdateCallback callback);
void gwmReshapeCallback(ReshapeCallback callback);
void gwmSwapBuffers(void);

// void gwmLog(const char *log);
#define gwmLog(...)   fprintf(_gpFile, __VA_ARGS__);fprintf(_gpFile, "\n")

bool gwmLoadTexture(GLuint *texture, TCHAR imageResourceID[]);


// Global Variables
FILE  *_gpFile = NULL;
bool  _gbActiveWindow = false;
bool  _gbIsFullScreen = false;
HDC   _ghDC = NULL;
HGLRC _ghRC = NULL;
HWND  _ghWnd = NULL;
DWORD _dwStyle;

int _gWidth = 800;
int _gHeight = 600;

WINDOWPLACEMENT _wpPrev = { sizeof(WINDOWPLACEMENT) };

// Callbacks
InitializeCallback   _initializeCallback  = NULL;
KeyboardCallback     _keyboardCallback    = NULL;  
MouseMoveCallback    _mouseMoveCallback   = NULL; 
MouseClickCallback   _mouseClickCallback  = NULL;
DisplayCallback      _displayCallback     = NULL;   
UpdateCallback       _updateCallback      = NULL;    
ReshapeCallback      _reshapeCallback     = NULL;   
UninitializeCallback _uninitializeCallback = NULL;
 
// Global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void    ToggleFullScreen(void);

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declarations
	void ToggleFullScreen(void);
	void _uninitialize(void);

	// code
	switch (iMsg)
	{

	case WM_SETFOCUS:
		_gbActiveWindow = true;
		break;

	case WM_KILLFOCUS:
		_gbActiveWindow = false;
		break;

	case WM_SIZE:
        if (_reshapeCallback)
		    _reshapeCallback(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_KEYDOWN:
        if(_keyboardCallback) 
		{
            _keyboardCallback(wParam);
		}

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
        _uninitialize();
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen()
{
	MONITORINFO MI;

	if (_gbIsFullScreen == false)
	{
		_dwStyle = GetWindowLong(_ghWnd, GWL_STYLE);
		if (_dwStyle & WS_OVERLAPPEDWINDOW)
		{
			MI = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(_ghWnd, &_wpPrev)
				&& GetMonitorInfo(MonitorFromWindow(_ghWnd, MONITORINFOF_PRIMARY), &MI))
			{
				SetWindowLong(_ghWnd, GWL_STYLE, _dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(_ghWnd,
					HWND_TOP,
					MI.rcMonitor.left,
					MI.rcMonitor.top,
					MI.rcMonitor.right - MI.rcMonitor.left,
					MI.rcMonitor.bottom - MI.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
		_gbIsFullScreen = true;
	}
	else
	{
		SetWindowLong(_ghWnd, GWL_STYLE, _dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(_ghWnd, &_wpPrev);
		SetWindowPos(_ghWnd,
			HWND_TOP,
			0,
			0,
			0,
			0,
			SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);

		ShowCursor(TRUE);
		_gbIsFullScreen = false;
	}
}

int initialize(void)
{
	// function declarations
	void _uninitialize(void);

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

	_ghDC = GetDC(_ghWnd);

	iPixelFormatIndex = ChoosePixelFormat(_ghDC, &pfd);
	// iPixelFormatIndex is 1 based, so 0 indicates error
	if (iPixelFormatIndex == 0)
	{
		return(-1);
	}

	if (SetPixelFormat(_ghDC, iPixelFormatIndex, &pfd) == FALSE)
	{
		return(-2);
	}

	_ghRC = wglCreateContext(_ghDC);
	if (_ghRC == NULL)
	{
		return(-3);
	}

	if (wglMakeCurrent(_ghDC, _ghRC) == FALSE)
	{
		return(-4);
	}

	// clear the depth buffer
	glClearDepth(1.0f);

	// clear the screen by OpenGL
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// enable depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    if(_initializeCallback)
        _initializeCallback();

    if(_reshapeCallback) {
        _reshapeCallback(_gWidth, _gHeight);
	}

	return(0);
}

void _uninitialize(void)
{
    if (_uninitializeCallback)
        _uninitializeCallback();

    	// fullscreen check
	if (_gbIsFullScreen == true)
	{
		SetWindowLong(_ghWnd, GWL_STYLE, _dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(_ghWnd, &_wpPrev);
		SetWindowPos(_ghWnd,
			HWND_TOP,
			0,
			0,
			0,
			0,
			SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);

		ShowCursor(TRUE);
	}

	// break the current context
	if (wglGetCurrentContext() == _ghRC)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (_ghRC)
	{
		wglDeleteContext(_ghRC);
	}

	if (_ghDC)
	{
		ReleaseDC(_ghWnd, _ghDC);
		_ghDC = NULL;
	}

	if (_gpFile)
	{
		fprintf(_gpFile, "Log file is closed...\n");
		fclose(_gpFile);
		_gpFile = NULL;	
    }
}

void gwmCreateWindow(const char *title, int x, int y, int width, int height)
{
    // variables 
	int iRet = 0;
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;

    _gWidth = width;
    _gHeight = height;

    HINSTANCE hInstance = (HINSTANCE) GetModuleHandle(NULL);

    size_t size = strlen(title) + 1;
	TCHAR *szClassName = new TCHAR[size];
    mbstowcs_s(&size, szClassName, size, title, _TRUNCATE);

	// code
	// create file for logging
	if (fopen_s(&_gpFile, "log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Cannot Create log file!"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	else
	{
		fprintf(_gpFile, "Log.txt file created...\n");
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

	_ghWnd = hwnd;

	iRet = initialize();
	if (iRet == -1)
	{
		fprintf(_gpFile, "ChoosePixelFormat failed...\n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -2)
	{
		fprintf(_gpFile, "SetPixelFormat failed...\n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -3)
	{
		fprintf(_gpFile, "wglCreateContext failed...\n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -4)
	{
		fprintf(_gpFile, "wglMakeCurrent failed...\n");
		DestroyWindow(hwnd);
	}
	else
	{
		fprintf(_gpFile, "initialize() successful...\n");
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
    if (!_gbIsFullScreen)
        ToggleFullScreen();
}

void gwmExitFullScreen()
{
    if (_gbIsFullScreen)
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
			if (_gbActiveWindow == true) 
            {
                if (_updateCallback)
                    _updateCallback();
            }

            if (_displayCallback)
			{
			    _displayCallback();
			}
		}
    }    
}

void gwmExitEventLoop()
{
    // TODO: post WM_QUIT message
    DestroyWindow(_ghWnd);
}


void gwmInitializeCallback(InitializeCallback callback)
{
    _initializeCallback = callback;
}

void gwmUninitializeCallback(UninitializeCallback callback)
{
    _uninitializeCallback = callback;
}

void gwmKeyboardCallback(KeyboardCallback callback)
{
    _keyboardCallback = callback;
}

void gwmMouseMoveCallback(MouseMoveCallback callback)
{
    _mouseMoveCallback = callback;
}

void gwmMouseClickCallback(MouseClickCallback callback)
{
    _mouseClickCallback = callback;
}


void gwmDisplayCallback(DisplayCallback callback)
{
    _displayCallback = callback;
}
void gwmUpdateCallback(UpdateCallback callback)
{
    _updateCallback = callback;
}
void gwmReshapeCallback(ReshapeCallback callback)
{
    _reshapeCallback = callback;
}

void gwmSwapBuffers(void)
{
    SwapBuffers(_ghDC);
}

bool gwmLoadTexture(GLuint *texture, TCHAR imageResourceID[])
{
	// variables
	HBITMAP hBitmap = NULL;
	BITMAP bmp;
	bool bStatus = false;

	// data
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL),
		imageResourceID,
		IMAGE_BITMAP,
		0, 0,
		LR_CREATEDIBSECTION
	);

	if (hBitmap)
	{
		bStatus = true;
		GetObject(hBitmap, sizeof(BITMAP), &bmp);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);

		DeleteObject(hBitmap);
	}

	return bStatus;
}

#endif
