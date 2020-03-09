#include "gwm.h"

void main (void)
{
    // declarations
    void keyboardFunc(unsigned int);
    void displayFunc(void);
    void reshapeFunc(int, int);

    // code
    gwmCreateWindow("gwm demo", 100, 100, 300, 300);

    gwmKeyboardCallback(keyboardFunc);
    gwmDisplayCallback(displayFunc);
    gwmReshapeCallback(reshapeFunc);

    gwmEventLoop();
}

void keyboardFunc(unsigned int key)
{
    // TODO: common keymaps for all platforms
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
    glOrtho(-1, 1, -1, 1, -1, 1);
}

void displayFunc(void)
{
    // code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBegin(GL_TRIANGLES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(0.0f, 1.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(-1.0f, -1.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex2f(1.0f, -1.0f);
	
	glEnd();

    gwmSwapBuffers();
}

