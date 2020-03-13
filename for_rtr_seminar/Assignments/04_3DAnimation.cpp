#include "gwm.h"

int gWidth = 0;
int gHeight = 0;
float gAngle = 0.0f;

bool animate = false;

void main (void)
{
    // declarations
    void keyboardFunc(unsigned int);
    void displayFunc(void);
    void updateFunc(void);
    void reshapeFunc(int, int);

    // code
    gwmKeyboardCallback(keyboardFunc);
    gwmDisplayCallback(displayFunc);
    gwmUpdateCallback(updateFunc);
    gwmReshapeCallback(reshapeFunc);

    gwmCreateWindow("3D Animation", 100, 100, 800, 600);
    gwmEventLoop();
}

void keyboardFunc(unsigned int key)
{
    switch(key)
    {
        // VK_ESCAPE
        case 0x1B:
            gwmExitEventLoop();
            break;

		// 0x20
		case VK_SPACE:
			if (animate == true)
			{
				animate = false;
			}
			else
			{
				animate = true;
			}
			break;

    }
}

void reshapeFunc(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gWidth = width;
    gHeight = height;

    glViewport(0, 0, gWidth, gHeight);
    gluPerspective(45.0, (float)width / (float)height, 0.1f, 100.0f);
}

void displayFunc(void)
{
    // code
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -6.0f);
    glScalef(0.8f, 0.8f, 0.8f);
	glRotatef(gAngle, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);

	/* Top */
	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	/* Bottom */
	glColor3f(0.0f, 1.0f, 0.0f);

	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f,- 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	/* Front */
	glColor3f(0.0f, 1.0f, 1.0f);

	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	/* Back */
	glColor3f(0.0f, 0.0f, 1.0f);

	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	/* Right */
	glColor3f(1.0f, 0.0f, 1.0f);

	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	/* Left */
	glColor3f(1.0f, 1.0f, 0.0f);

	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glEnd();

    gwmSwapBuffers();
}

void updateFunc(void)
{
	if (animate == true)
	{
		gAngle = gAngle + 0.5f;
	}
}
