#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f; 
float lz = 0.0f; 
float ly = 0.0f;
// XY position of the camera
float x = 0.0f;
float z = 5.0f;
float y = 1.75f;
// the key states. These variables will be zero
// when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
//This is for keeping track of the mouse
int xOrigin = -1;
// this is for the menus
int mainWindow, subWindow1, subWindow2, subWindow3;
int w; 
int h; 
int border =6;

// variables to compute frames per second
int frame;
long time, timebase;
char s[50];
void setProjection(int w1, int h1) 
{
    float ratio;
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    ratio = 1.0f * w1/ h1;
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // set the viewport to be the entire window
    glViewport(0,0,w1,h1);
    // Set the clipping volume
    gluPerspective(45, ratio, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
}
// Draws snowmen
void drawSnowMan() {

    // Draw Body
        glTranslatef(0.0f, 0.75f, 0.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glutSolidSphere(0.75f, 20, 20);
    
    // Draw Head
        glTranslatef(0.0f, 1.0f, 0.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glutSolidSphere(0.25f, 20, 20);
    
    // Draw Eyes
        glPushMatrix();
        glColor3f(0.0f, 0.0f, 0.0f);
        glTranslatef(0.05f, 0.10f, 0.18f);
        glutSolidSphere(0.05f, 10, 10);
        glTranslatef(-0.1f, 0.0f, 0.0f);
        glutSolidSphere(0.05f, 10, 10);
        glPopMatrix();

    // Draw Nose
        glColor3f(1.0f, 0.5f, 0.5f);
        glutSolidCone(0.08f, 0.5f, 10, 2);
}
// deals with window sizing
void changeSize(int w1, int h1) {

    if(h1 == 0)
		h1 = 1;

	// we're keeping these values cause we'll need them latter
	w = w1;
	h = h1;

	// set subwindow 1 as the active window
	glutSetWindow(subWindow1);
	// resize and reposition the sub window
	glutPositionWindow(border,border);
	glutReshapeWindow(w-2*border, h/2 - border*3/2);
	setProjection(w-2*border, h/2 - border*3/2);

	// set subwindow 2 as the active window
	glutSetWindow(subWindow2);
	// resize and reposition the sub window
	glutPositionWindow(border,(h+border)/2);
	glutReshapeWindow(w/2-border*3/2, h/2 - border*3/2);
	setProjection(w/2-border*3/2,h/2 - border*3/2);

	// set subwindow 3 as the active window
	glutSetWindow(subWindow3);
	// resize and reposition the sub window
	glutPositionWindow((w+border)/2,(h+border)/2);
	glutReshapeWindow(w/2-border*3/2,h/2 - border*3/2);
	setProjection(w/2-border*3/2,h/2 - border*3/2);    

   /* //Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0){
        h = 1;
    }
    float ratio = 1.0 * w / h;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(45, ratio, 1, 1000);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);*/
}

void computePos(float deltaMove) {

    x += deltaMove * lx * 0.1f;
    z += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle) {

    angle += deltaAngle;
    lx = sin(angle);
    lz = -cos(angle);
}
void renderScene2() {

    // Draw ground
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
        glVertex3f(-100.0f, 0.0f, -100.0f);
        glVertex3f(-100.0f, 0.0f, 100.0f);
        glVertex3f(100.0f, 0.0f, 100.0f);
        glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();

    // Draw 36 Snowmen
    char number[3];
    for(int i = -3; i < 3; i++) {
        for(int j = -3; j < 3; j++) {
            glPushMatrix();
            glTranslatef(i*10.0, 0, j * 10.0);
            drawSnowMan();
            glPopMatrix();
        }
    }
}

void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {

	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}
void restorePerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();

	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}
void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, w, h, 0);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}
// Draws the scene which is kinda important
void renderScene() {
    glutSetWindow(mainWindow);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

// Display func for sub window 1
void renderScenesw1(void) {
    if (deltaMove){
        computePos(deltaMove);
    }
    if (deltaAngle){
        computeDir(deltaAngle);
    }
    glutSetWindow(subWindow1);
    // Clear Color Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();
    // Set the camera
    gluLookAt(  x, 1.0f, z,             // camera position
                x+lx, 1.0f, z+lz,        // look at point (line of sight?)
                0.0f, 1.0f, 0.0f);      // indicates the up vector
    
    renderScene2();

    // display fps in the top window
    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);
    if (time - timebase > 1000) {
        sprintf(s, "Lighthouse3d - FPS:%4.2f",
                    frame*1000.0/(time-timebase));
        timebase = time;
        frame = 0;
    }

    setOrthographicProjection();

    glPushMatrix();
    glLoadIdentity();
    renderBitmapString(5,30,0,GLUT_BITMAP_HELVETICA_12, s);
    glPopMatrix();

    restorePerspectiveProjection();

    glutSwapBuffers();
}

void renderScenesw2() {

    glutSetWindow(subWindow2);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(x, y+15, z,
                x, y-1, z,
                lx, 0, lz);
    
    // Draw red cone at the location of the main camera
    glPushMatrix();
    glColor3f(1.0,0.0,0.0);
    glTranslated(x,y,z);
    glRotatef(180-(angle+deltaAngle)*180.0/3.14,0.0,1.0,0.0);
    glutSolidCone(0.2,0.8f,4,4);
    glPopMatrix();

    renderScene2();

    glutSwapBuffers();
}

void renderScenesw3() {

    glutSetWindow(subWindow3);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(x-lz*10, y, z+lx*10,
                x, y ,z ,
                0.0f, 1.0f, 0.0f);
    // Draw red cone at the location of the main camera
    glPushMatrix();
    glColor3f(1.0,0.0,0.0);
    glTranslatef(x,y,z);
    glRotatef(180-(angle+deltaAngle)*180.0/3.14, 0.0, 1.0, 0.0);
    glutSolidCone(0.2, 0.8f, 4, 4);
    glPopMatrix();
    
    renderScene2();

    glutSwapBuffers();
}
void renderSceneAll() {

    // check for keyboard movement
    if (deltaMove){
        computePos(deltaMove);
    }
    renderScenesw1();
    renderScenesw2();
    renderScenesw3();
}
// deals with normal key functions like 'a', 'w'
void processNormalKeys(unsigned char key, int x, int y) {

    if (key == 27){
        glutDestroyWindow(subWindow1);
		glutDestroyWindow(subWindow2);
		glutDestroyWindow(subWindow3);
		glutDestroyWindow(mainWindow);
        exit(0);
    }
}
// deals with "Special" key functions like UP_ARROW
void processSpecialKeys(int key, int xx, int yy) {

    //float fraction = 0.1f;

    switch (key) {
        case GLUT_KEY_LEFT :
            /*angle -= 0.01f;       // This is for movement one button press at 
            lx = sin(angle);        // a Time
            lz = -cos(angle);*/
            deltaAngle = -0.01f;    // This is for holding down a button
            break;
        case GLUT_KEY_RIGHT :
            /*angle += 0.01f;
            lx = sin(angle);
            lz = -cos(angle);*/
            deltaAngle = 0.01f;
            break;
        case GLUT_KEY_UP :
            /*x += lx * fraction;
            z += lz * fraction;*/
            deltaMove = 0.5f;
            break;
        case GLUT_KEY_DOWN :
            /*x += lx * fraction;
            z -= lz * fraction;*/
            deltaMove = -0.5f;
            break;
    }
}
// for key release for specialUpFunc
void releaseKey(int key, int x, int y) {

    switch (key) {
        case GLUT_KEY_LEFT :
        case GLUT_KEY_RIGHT :
            deltaAngle = 0.0f;
            break;
        case GLUT_KEY_UP :
        case GLUT_KEY_DOWN :
            deltaMove = 0;
            break;
    }
}

// responsible for processing the button state changes
void mouseButton(int button, int state, int x, int y) {

    // only start motion if the left button is pressed
    if (button == GLUT_LEFT_BUTTON) {
        // when the button is released
        if (state == GLUT_UP) {
            angle += deltaAngle;
            xOrigin = -1;
        }
        else {
            // state = GLUT_DOWN
            xOrigin = x;
        }
    }
}
// process the motion of the mouse 
void mouseMove(int x, int y) {

    // This will only be true when the left button is down
    if (xOrigin >= 0) {

        // update deltaAngle
        deltaAngle = (x - xOrigin) * 0.0005f;

        // update camera's direction
        lx = sin(angle + deltaAngle);
        lz = -cos(angle + deltaAngle);
    }
}
//for multiple windows
void init() {

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // register callbacks
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(&processNormalKeys);
    glutSpecialFunc(&processSpecialKeys);
    glutSpecialUpFunc(&releaseKey);
    glutMouseFunc(&mouseButton);
    glutMotionFunc(&mouseMove);
}

int main(int argc, char **argv) {

    // init GLUT and create main window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1000, 1000);
    mainWindow = glutCreateWindow("Lighthouse3D - GLUT Tutorial");

    // callbacks for main window
    glutDisplayFunc(&renderScene);
    glutReshapeFunc(&changeSize);
    glutIdleFunc(&renderSceneAll);
    init();

    // Sub windows
    subWindow1 = glutCreateSubWindow(mainWindow, border, border, w-2*border, h/2 - border*3/2);
    glutDisplayFunc(&renderScenesw1);
    init();

    subWindow2 = glutCreateSubWindow(mainWindow, border,(h+border)/2,w/2-border*3/2, h/2 - border*3/2);
    glutDisplayFunc(&renderScenesw2);
    init();

    subWindow3 = glutCreateSubWindow(mainWindow, (w+border)/2,(h+border)/2,w/2-border*3/2,h/2 - border*3/2);
    glutDisplayFunc(&renderScenesw3);

    // for key release info
    glutIgnoreKeyRepeat(1);
    glutSpecialUpFunc(&processSpecialKeys);

    // These are for mouse movement
    glutMouseFunc(&mouseButton);
    glutMotionFunc(&mouseMove);

    // OpenGL init
    glEnable(GL_DEPTH_TEST);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 1;
}