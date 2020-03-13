#include "../gwm.h"

#include "res.h"

GLuint texture_moon;
GLUquadric *quadric = NULL;

void main (void)
{
    // declarations
    void initFunc(void);
    void keyboardFunc(unsigned int);
    void displayFunc(void);
    void reshapeFunc(int, int);
    void uninitFunc(void);

    // code
    gwmInitializeCallback(initFunc);
    gwmKeyboardCallback(keyboardFunc);
    gwmDisplayCallback(displayFunc);
    gwmReshapeCallback(reshapeFunc);
    gwmUninitializeCallback(uninitFunc);

    gwmCreateWindow("Texture", 100, 100, 800, 600);
    gwmEventLoop();
}

void initFunc(void)
{
    // Texture 
	glEnable(GL_TEXTURE_2D);
	gwmLoadTexture(&texture_moon, MAKEINTRESOURCE(IDBITMAP_MOON));
}

void keyboardFunc(unsigned int key)
{
    switch(key)
    {
        // VK_ESCAPE
        case 0x1B:
            gwmExitEventLoop();
            break;
    }
}

void reshapeFunc(int width, int height)
{
    if (height == 0)
	{
		height = 1;
	}

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void displayFunc(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Load ModelView Matrix */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();		/* Reset to Identity Matrix | center */

	/* Multiply it by Translation Matrix */
	glTranslatef(0.0f, 0.0f, -4.0f);

	glBegin(GL_QUADS);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(1.0f, 1.0f);
	
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-1.0f, 1.0f);
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-1.0f, -1.0f);
	
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(1.0f, -1.0f);

	glEnd();


    gwmSwapBuffers();
}

void uninitFunc(void)
{
    if (quadric)
	{
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}
}
