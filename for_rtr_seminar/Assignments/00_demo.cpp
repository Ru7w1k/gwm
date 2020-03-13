#include "gwm.h"

#define _USE_MATH_DEFINES 1
#include <math.h>

void main (void)
{
    // declarations
    void initFunc(void);
    void keyboardFunc(unsigned int);
    void displayFunc(void);
    void reshapeFunc(int, int);

    // code
    gwmInitializeCallback(initFunc);
    gwmKeyboardCallback(keyboardFunc);
    gwmDisplayCallback(displayFunc);
    gwmReshapeCallback(reshapeFunc);

    gwmCreateWindow("Sun", 100, 100, 800, 600);
    gwmEventLoop();
}

void initFunc(void)
{
    // clear the depth buffer
	glClearDepth(1.0f);

	// clear the screen by OpenGL
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// enable depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void keyboardFunc(unsigned int key)
{
    // VK_ESCAPE
    if(key == 0x1B)
    {
        gwmExitEventLoop();
    }
}

void reshapeFunc(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, width, height);
    //glOrtho(-2, 2, -2, 2, -1, 1);
	gluPerspective(45.0, (float)width / (float)height, 0.1f, 100.0f);

}

void displayFunc(void)
{
    // code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glTranslatef(0.0f, 0.0f, -3.0f);

	glBegin(GL_TRIANGLES);
    for(float theta = 0.0f; theta <= 2*M_PI; theta += 0.1f)
    {
        glColor3f(1.0f, 0.1f, 0.0f);
        glVertex2f(0.0f, 0.0f);

        glColor3f(1.0f, 0.7f, 0.0f);
        glVertex2f(cos(theta), sin(theta));

        glColor3f(1.0f, 0.7f, 0.0f);
        glVertex2f(cos(theta + 0.1f), sin(theta + 0.1f));
    }
	glEnd();

    gwmSwapBuffers();
}

