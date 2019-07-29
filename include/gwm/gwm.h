#ifndef __gwm_h__
#define __gwm_h__

/*
 * Generic Window Manager
 *
 * A simple common wrapper over multiple windom managers.
 */
#include <stdio.h>
#include <GL/glew.h>
#include <gl/GL.h>
#include "../vmath.h"

using namespace vmath;

typedef void (*InitializeCallback) (void);
typedef void (*KeyboardCallback)   (unsigned int key);
typedef void (*MouseMoveCallback)  (int x, int y);
typedef void (*MouseClickCallback) (unsigned int key, int x, int y);
typedef void (*KeyboardCallback)   (unsigned int key);
typedef void (*DisplayCallback)    (void);
typedef void (*UpdateCallback)     (void);
typedef void (*ReshapeCallback)    (int width, int height);


void gwmCreateWindow(const char *title, int x, int y, int width, int height);
void gwmDestroyWindow(void);

void gwmInitializeCallback(InitializeCallback callback);

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

void gwmLog(const char *log);


#endif
