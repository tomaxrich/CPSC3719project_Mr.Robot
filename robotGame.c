#define PROGRAM_TITLE "CPSC 3710 TERM"
#define DISPLAY_INFO "Robot Game"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

//Global Variables
int Window_ID;
int Window_Width = 600;
int Window_Height = 400;

int pause = 0;
GLboolean f2down, f3down, zdown = false;

//camera controls relative to the origin (world)
int deltaX = 0;
int deltaY = 0;
int deltaZ = 0;

//camera control storage variables relative to the robot
//Might want to increase these
int camStorX = -10;
int camStorY = 8;
int camStorZ = 0;

//view port camera controls
int camX = -8;
int camY = 8;
int camZ = 0;

int view = 0;

//Robot Global Variables
int RobotX = 0.0;
int RobotY = 3.0;
int RobotZ = 0.0;
int RobotOrient = 0;

float antDeg = 0;
float antSpeed = 0.1;
float headDeg = 0.0;


#define BUILDINGS 20
#define SIZE 512
int MaxDistance = BUILDINGS * 10;    //This may change to facilitate intersection points

//Building Map

int Building_Map[BUILDINGS*BUILDINGS];

//Lighting
GLfloat lightDiffuse[] = {0.9, 0.9, 0.9, 5.0};
GLfloat lightPosition[] = {100.0, 50.0, 100.0, 0.5};

GLUquadric *quadratic;
GLuint texture[5];

void drawRobot()
{	
		//Move to Robot
		glTranslatef(RobotX,RobotY,RobotZ);

		//Turn the Robot
		glRotatef(-RobotOrient,0.0f,1.0f,0.0f);
	
		//Draw Head
		glColor4f(0.75f,0.75f,0.75f,0.0f);
		glRotatef(-headDeg,0.0f,1.0f,0.0f);
		glutSolidCube(0.5f);

		glColor4f(0.0f,1.0f,1.0f,0.0f);
		glRotatef(90.0,0.0,1.0,0.0);
		glTranslatef(-0.125,0.0,0.251);
		gluCylinder(quadratic,0.1f,0.0f,0.01f,5,5);
		glTranslatef(0.25,0.0,0.0);
		gluCylinder(quadratic,0.1f,0.0f,0.01f,5,5);
		glTranslatef(-0.125,0.0,-0.251);
		glRotatef(-90.0,0.0,1.0,0.0);
		glRotatef(headDeg,0.0f,1.0f,0.0f);

		//Draw Antenna
		glColor4f(0.0f,0.5f,0.5f,0.0f);	
		glRotatef(-90.0f,1.0f,0.0f,0.0f);
		glRotatef(-antDeg,0.0f,0.0f,1.0f);
		gluCylinder(quadratic,0.1f,0.1f,0.5f,5,5);
		glRotatef(antDeg,0.0f,0.0f,1.0f);
		glRotatef(90.0f,1.0f,0.0f,0.0f);
		

		//Draw Body
		glColor4f(0.75f,0.75f,0.75f,0.0f);
		glBegin(GL_QUADS);		        
		//Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -2.5f,  0.5f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -2.5f,  0.5f);      
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.5f);      
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f,  0.5f);      
		//Back Face
		glNormal3f( 0.0f, 0.0f,-1.0f);	        
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -2.5f, -0.5f);      
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);      
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -2.5f, -0.5f);      
		//Top Face
		glNormal3f( 0.0f, 1.0f, 0.0f);		
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f,  0.5f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -0.5f,  0.5f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);	
		//Bottom Face
		glNormal3f( 0.0f,-1.0f, 0.0f);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -2.5f, -0.5f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -2.5f, -0.5f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -2.5f,  0.5f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -2.5f,  0.5f);	
		//Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);	        
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.5f, -2.5f, -0.5f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.5f, -0.5f, -0.5f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.5f, -0.5f,  0.5f);      
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.5f, -2.5f,  0.5f);	
		//Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -2.5f, -0.5f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -2.5f,  0.5f);      
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, -0.5f,  0.5f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);	
		glEnd();				        

		//Draw Neck
		glBegin(GL_QUADS);			
		//Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);	        
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, -0.8f,  0.1f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.1f, -0.8f,  0.1f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.1f, 0.0f,  0.1f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.1f, 0.0f,  0.1f);	
		//Back Face
		glNormal3f( 0.0f, 0.0f,-1.0f);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1f, -0.8f, -0.1f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.1f, 0.0f, -0.1f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.1f, 0.0f, -0.1f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.8f, -0.1f);	
		//Top Face
		glNormal3f( 0.0f, 1.0f, 0.0f);		
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.1f, 0.0f, -0.1f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, 0.0f,  0.1f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.1f, 0.0f,  0.1f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.1f, 0.0f, -0.1f);	
		//Bottom Face
		glNormal3f( 0.0f,-1.0f, 0.0f);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.1f, -0.8f, -0.1f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.1f, -0.8f, -0.1f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.8f,  0.1f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1f, -0.8f,  0.1f);	
		//Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.1f, -0.8f, -0.1f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.1f, 0.0f, -0.1f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.1f, 0.0f,  0.1f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.1f, -0.8f,  0.1f);	
		//Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.1f, -0.8f, -0.1f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.1f, -0.8f,  0.1f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.1f, 0.0f,  0.1f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.1f, 0.0f, -0.1f);	
		glEnd();					
		//Square on Front
		glColor4f(1.0f,0.0f,0.0f,0.0f);
		glBegin(GL_QUADS);
		glNormal3f( 1.0f, 0.0f, 0.0f);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 0.51f, -2.0f, -0.35f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 0.51f, -1.0f, -0.35f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.51f, -1.0f,  0.35f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.51f, -2.0f,  0.35f);    
		glEnd();

		//Triangles on Back
		glColor4f(0.0f,0.0f,1.0f,0.0f);
		glBegin(GL_TRIANGLES);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -0.51f, -1.5f, -0.35f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -0.51f, -1.0f, 0.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -0.51f, -1.5f,  0.35f);
		glEnd();

		glBegin(GL_TRIANGLES);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( -0.51f, -2.0f, -0.35f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( -0.51f, -1.5f, 0.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( -0.51f, -2.0f,  0.35f);   
		glEnd();

		glRotatef(RobotOrient,0.0f,1.0f,0.0f);
	
	glTranslatef(-RobotX,-RobotY,-RobotZ);
}

//House one
void drawHouse(int i) {
    int temp = i;
    float colorRed, colorGreen;
    if(temp == 1){
        colorRed = 1.0;
        colorGreen = 0.0;
    }
    else{
        colorRed = 0.0;
        colorGreen = 1.0;
    }
    //Draw main square of house
    glBegin(GL_QUADS);
        //back face
        glNormal3f(0.0f,0.0f,0.0f);
        glColor4f(colorRed,colorGreen,0.0f,1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, 0.0f, 2.0);     // Bottom Left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, 0.0f, 2.0f);     // Bottom Right corner
        glTexCoord2f(1.0, 1.0); glVertex3f(2.5f, 4.0f, 2.0);       // Top Right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 4.0f, 2.0f);     // Top Left corner
        //front face
        glNormal3f(0.0f,0.0f,0.0f);
        glColor4f(colorRed,colorGreen,0.0f,1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, 0.0f, -1.0f);     // Bottom Left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, 0.0f, -1.0f);     // Bottom Right corner
        glTexCoord2f(1.0, 1.0); glVertex3f(2.5f, 4.0f, -1.0f);       // Top Right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 4.0f, -1.0f);     // Top Left corner
        //right face
        glNormal3f(0.0f,0.0f,0.0f);
        glColor4f(colorRed,colorGreen,0.0f,1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(2.5f, 0.0f, -1.0f);     // Bottom Left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, 0.0f, 2.0f);     // Bottom Right corner
        glTexCoord2f(1.0, 1.0); glVertex3f(2.5f, 4.0f, 2.0f);      // Top Right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5f, 4.0f, -1.0f);     // Top Left corner
        //left face
        glNormal3f(0.0f,0.0f,0.0f);
        glColor4f(colorRed,colorGreen,0.0f,1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, 0.0f, 2.0f);     // Bottom Left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.5f, 0.0f, -1.0f);     // Bottom Right corner
        glTexCoord2f(1.0, 1.0); glVertex3f(-2.5f, 4.0f, -1.0f);      // Top Right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 4.0f, 2.0f);     // Top Left corner
    glEnd();
}

void drawRoof(){
    glBegin(GL_TRIANGLES);
        //Front
        glColor3f(0.6f, 0.3f, 0.0f);
        glVertex3f(-3.0f,4.0f,-1.5f);   //left bottom corner
        glVertex3f(3.0f,4.0f,-1.5f);    //right bottom corner
        glVertex3f(0.0f,5.0f,0.5f);     //top / Apex
        //Right
        glColor3f(0.6f, 0.3f, 0.0f);
        glVertex3f(3.0f,4.0f,-1.5f);    //left bottom corner
        glVertex3f(3.0f,4.0f,2.5f);     //right bottom corner
        glVertex3f(0.0f,5.0f,0.5f);     //top / Apex
        //Back
        glColor3f(0.6f, 0.3f, 0.0f);    
        glVertex3f(3.0f,4.0f,2.5f);     //left bottom corner
        glVertex3f(-3.0f,4.0f,2.5f);    //right bottom corner
        glVertex3f(0.0f,5.0f,0.5f);     //top / Apex
        //Left
        glColor3f(0.6f, 0.3f, 0.0f);    
        glVertex3f(-3.0f,4.0f,2.5f);    //left bottom corner
        glVertex3f(-3.0f,4.0f,-1.5f);   //right bottom corner
        glVertex3f(0.0f,5.0f,0.5f);     //top / Apex
    glEnd();
}

void drawDoor(){
    glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex3f(-0.5f, 0.0f, -1.1f);     //bottom left
        glVertex3f(0.5f, 0.0f, -1.1f);     //bottom right
        glVertex3f(0.5f, 3.0f, -1.1f);     //top right
        glVertex3f(-0.5f, 3.0f, -1.1f);     //top left
    glEnd();
}

void drawWindow1(){
    glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex3f(0.7f, 2.3f, -1.1f);     //bottom left
        glVertex3f(1.2f, 2.3f, -1.1f);     //bottom right
        glVertex3f(1.2f, 2.7f, -1.1f);     //top right
        glVertex3f(0.7f, 2.7f, -1.1f);     //top left
    glEnd();
}

void drawWindow2(){
    glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex3f(-0.7f, 2.3f, -1.1f);     //bottom left
        glVertex3f(-1.2f, 2.3f, -1.1f);     //bottom right
        glVertex3f(-1.2f, 2.7f, -1.1f);     //top right
        glVertex3f(-0.7f, 2.7f, -1.1f);     //top left
    glEnd();
}

void drawHouseOne(int i){
    drawHouse(i);
    drawRoof();
    drawDoor();
    drawWindow1();
    drawWindow2();
}

//House Two
void drawHouseTwo() {
    //Draw main square of house
    glBegin(GL_QUADS);
        //back face
        glNormal3f(0.0f,0.0f,0.0f);
        glColor4f(0.5f,0.0f,1.0f,1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, 0.0f, 2.0);     // Bottom Left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, 0.0f, 2.0f);     // Bottom Right corner
        glTexCoord2f(1.0, 1.0); glVertex3f(2.5f, 8.0f, 2.0);       // Top Right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 8.0f, 2.0f);     // Top Left corner
        //front face
        glNormal3f(0.0f,0.0f,0.0f);
        glColor4f(0.5f,0.0f,1.0f,1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, 0.0f, -1.0f);     // Bottom Left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, 0.0f, -1.0f);     // Bottom Right corner
        glTexCoord2f(1.0, 1.0); glVertex3f(2.5f, 8.0f, -1.0f);       // Top Right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 8.0f, -1.0f);     // Top Left corner
        //right face
        glNormal3f(0.0f,0.0f,0.0f);
        glColor4f(0.5f,0.0f,1.0f,1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(2.5f, 0.0f, -1.0f);     // Bottom Left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, 0.0f, 2.0f);     // Bottom Right corner
        glTexCoord2f(1.0, 1.0); glVertex3f(2.5f, 8.0f, 2.0f);      // Top Right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5f, 8.0f, -1.0f);     // Top Left corner
        //left face
        glNormal3f(0.0f,0.0f,0.0f);
        glColor4f(0.5f,0.0f,1.0f,1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, 0.0f, 2.0f);     // Bottom Left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.5f, 0.0f, -1.0f);     // Bottom Right corner
        glTexCoord2f(1.0, 1.0); glVertex3f(-2.5f, 8.0f, -1.0f);      // Top Right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 8.0f, 2.0f);     // Top Left corner
    glEnd();
}

void drawRoofTwo(){
    glBegin(GL_TRIANGLES);
        //Front
        glColor3f(0.6f, 0.3f, 0.0f);
        glVertex3f(-3.0f,8.0f,-1.5f);   //left bottom corner
        glVertex3f(3.0f,8.0f,-1.5f);    //right bottom corner
        glVertex3f(0.0f,9.0f,0.5f);     //top / Apex
        //Right
        glColor3f(0.6f, 0.3f, 0.0f);
        glVertex3f(3.0f,8.0f,-1.5f);    //left bottom corner
        glVertex3f(3.0f,8.0f,2.5f);     //right bottom corner
        glVertex3f(0.0f,9.0f,0.5f);     //top / Apex
        //Back
        glColor3f(0.6f, 0.3f, 0.0f);    
        glVertex3f(3.0f,8.0f,2.5f);     //left bottom corner
        glVertex3f(-3.0f,8.0f,2.5f);    //right bottom corner
        glVertex3f(0.0f,9.0f,0.5f);     //top / Apex
        //Left
        glColor3f(0.6f, 0.3f, 0.0f);    
        glVertex3f(-3.0f,8.0f,2.5f);    //left bottom corner
        glVertex3f(-3.0f,8.0f,-1.5f);   //right bottom corner
        glVertex3f(0.0f,9.0f,0.5f);     //top / Apex
    glEnd();
}

void drawDoorTwo(){
    glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex3f(-0.5f, 0.0f, -1.1f);     //bottom left
        glVertex3f(0.5f, 0.0f, -1.1f);     //bottom right
        glVertex3f(0.5f, 3.0f, -1.1f);     //top right
        glVertex3f(-0.5f, 3.0f, -1.1f);     //top left
    glEnd();
}

void drawWindow1Two(){
    glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex3f(0.7f, 2.3f, -1.1f);     //bottom left
        glVertex3f(1.2f, 2.3f, -1.1f);     //bottom right
        glVertex3f(1.2f, 2.7f, -1.1f);     //top right
        glVertex3f(0.7f, 2.7f, -1.1f);     //top left
    glEnd();
}

void drawWindow2Two(){
    glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex3f(-0.7f, 2.3f, -1.1f);     //bottom left
        glVertex3f(-1.2f, 2.3f, -1.1f);     //bottom right
        glVertex3f(-1.2f, 2.7f, -1.1f);     //top right
        glVertex3f(-0.7f, 2.7f, -1.1f);     //top left
    glEnd();
}

void drawHouse2Two(){
    drawHouseTwo();
    drawRoofTwo();
    drawDoorTwo();
    drawWindow1Two();
    drawWindow2Two();
    glTranslatef(0.0f, 2.0f, 0.0f);
    drawWindow1Two();
    drawWindow2Two();
    glTranslatef(0.0f, 2.0f, 0.0f);
    drawWindow1Two();
    drawWindow2Two();
    glTranslatef(0.0f, -4.0f, 0.0f);
}

//House Three
void drawHouseThree() {
    //Draw main square of house
    glBegin(GL_QUADS);
        //back face
        glNormal3f(0.0f,0.0f,0.0f);
        glColor4f(0.0f,0.0f,0.6f,1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, 0.0f, 2.0);     // Bottom Left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, 0.0f, 2.0f);     // Bottom Right corner
        glTexCoord2f(1.0, 1.0); glVertex3f(2.5f, 12.0f, 2.0);       // Top Right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 12.0f, 2.0f);     // Top Left corner
        //front face
        glNormal3f(0.0f,0.0f,0.0f);
        glColor4f(0.0f,0.0f,0.6f,1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, 0.0f, -1.0f);     // Bottom Left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, 0.0f, -1.0f);     // Bottom Right corner
        glTexCoord2f(1.0, 1.0); glVertex3f(2.5f, 12.0f, -1.0f);       // Top Right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 12.0f, -1.0f);     // Top Left corner
        //right face
        glNormal3f(0.0f,0.0f,0.0f);
        glColor4f(0.0f,0.0f,0.6f,1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(2.5f, 0.0f, -1.0f);     // Bottom Left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5f, 0.0f, 2.0f);     // Bottom Right corner
        glTexCoord2f(1.0, 1.0); glVertex3f(2.5f, 12.0f, 2.0f);      // Top Right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5f, 12.0f, -1.0f);     // Top Left corner
        //left face
        glNormal3f(0.0f,0.0f,0.0f);
        glColor4f(0.0f,0.0f,0.6f,1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, 0.0f, 2.0f);     // Bottom Left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.5f, 0.0f, -1.0f);     // Bottom Right corner
        glTexCoord2f(1.0, 1.0); glVertex3f(-2.5f, 12.0f, -1.0f);      // Top Right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 12.0f, 2.0f);     // Top Left corner
    glEnd();
}

void drawRoofThree(){
    glBegin(GL_TRIANGLES);
        //Front
        glColor3f(0.6f, 0.3f, 0.0f);
        glVertex3f(-3.0f,12.0f,-1.5f);   //left bottom corner
        glVertex3f(3.0f,12.0f,-1.5f);    //right bottom corner
        glVertex3f(0.0f,14.0f,0.5f);     //top / Apex
        //Right
        glColor3f(0.6f, 0.3f, 0.0f);
        glVertex3f(3.0f,12.0f,-1.5f);    //left bottom corner
        glVertex3f(3.0f,12.0f,2.5f);     //right bottom corner
        glVertex3f(0.0f,14.0f,0.5f);     //top / Apex
        //Back
        glColor3f(0.6f, 0.3f, 0.0f);    
        glVertex3f(3.0f,12.0f,2.5f);     //left bottom corner
        glVertex3f(-3.0f,12.0f,2.5f);    //right bottom corner
        glVertex3f(0.0f,14.0f,0.5f);     //top / Apex
        //Left
        glColor3f(0.6f, 0.3f, 0.0f);    
        glVertex3f(-3.0f,12.0f,2.5f);    //left bottom corner
        glVertex3f(-3.0f,12.0f,-1.5f);   //right bottom corner
        glVertex3f(0.0f,14.0f,0.5f);     //top / Apex
    glEnd();
}

void drawDoorThree(){
    glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex3f(-0.5f, 0.0f, -1.1f);     //bottom left
        glVertex3f(0.5f, 0.0f, -1.1f);     //bottom right
        glVertex3f(0.5f, 3.0f, -1.1f);     //top right
        glVertex3f(-0.5f, 3.0f, -1.1f);     //top left
    glEnd();
}

void drawWindow1Three(){
    glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex3f(0.7f, 2.3f, -1.1f);     //bottom left
        glVertex3f(1.2f, 2.3f, -1.1f);     //bottom right
        glVertex3f(1.2f, 2.7f, -1.1f);     //top right
        glVertex3f(0.7f, 2.7f, -1.1f);     //top left
    glEnd();
}

void drawWindow2Three(){
    glBegin(GL_QUADS);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex3f(-0.7f, 2.3f, -1.1f);     //bottom left
        glVertex3f(-1.2f, 2.3f, -1.1f);     //bottom right
        glVertex3f(-1.2f, 2.7f, -1.1f);     //top right
        glVertex3f(-0.7f, 2.7f, -1.1f);     //top left
    glEnd();
}

void drawHouse3Three(){
    drawHouseThree();
    drawRoofThree();
    drawDoorThree();
    drawWindow1Three();
    drawWindow2Three();
    glTranslatef(0.0f, 2.0f, 0.0f);
    drawWindow1Three();
    drawWindow2Three();
    glTranslatef(0.0f, 2.0f, 0.0f);
    drawWindow1Three();
    drawWindow2Three();
    glTranslatef(0.0f, 2.0f, 0.0f);
    drawWindow1Three();
    drawWindow2Three();
    glTranslatef(0.0f, 2.0f, 0.0f);
    drawWindow1Three();
    drawWindow2Three();
    glTranslatef(0.0f, -8.0f, 0.0f);
}

void initMap(){
    int count;
    count = BUILDINGS * BUILDINGS;
    int check;
    for(int i = 0; i < count; i++){
        int c, n;
        int out = 0;
        for(c = 1; c <= 10; c++){
            n = rand() % 100 +1;
            out = out + n;
        }
        int output = out % 10;
        check = output;
        if(check < 3){
            //build house 1
            Building_Map[i] = 1;
        }
        else{
            if(check < 5){
                //build house 2
                Building_Map[i] = 2;
            }
            else{
                if(check < 8){
                    //build house 3
                    Building_Map[i] = 3;
                }
                else{
                    Building_Map[i] = 4;
                }
            }
        }
    }
}

void robotOrientation(){
    if(view != 0){
        switch(RobotOrient){
            case 0:
                camX = camStorX;
                camY = camStorY;
                camZ = camStorZ;
                break;
            case 90:
                if(view == 2 || view == 4){
                    camX = camStorX;
                    camY = camStorY;
                    camZ = -camStorZ;
                }
                else{
                    camX = -camStorX;
                    camY = camStorY;
                    camZ = camStorZ;
                }
                break;
            case 180:
                camX = -camStorX;
                camY = camStorY;
                camZ = -camStorZ;
                break;
            case 270:
                if(view == 2 || view == 4){
                    camX = -camStorX;
                    camY = camStorY;
                    camZ = camStorZ;
                }
                else{
                    camX = camStorX;
                    camY = camStorY;
                    camZ = -camStorZ;
                }
                break;
        }
    }
    else{
        switch(RobotOrient){
            case 0:
                camX = camStorX;
                camY = camStorY;
                camZ = camStorZ;
                break;
            case 90:
                camX = camStorZ;
                camY = camStorY;
                camZ = camStorX;
                break;
            case 180:
                camX = -camStorX;
                camY = camStorY;
                camZ = -camStorZ;
                break;
            case 270:
                camX = -camStorZ;
                camY = camStorY;
                camZ = -camStorX;
                break;
        }
    }
    if(RobotX > (MaxDistance)){ RobotX = (MaxDistance)-2; deltaX = (MaxDistance)-2;}
	if(RobotX < 0)  { RobotX = 2; deltaX = 2;}
	if(RobotZ > (MaxDistance)){ RobotZ = (MaxDistance)-2; deltaZ = (MaxDistance)-2;}
	if(RobotZ < 0)  { RobotZ = 2; deltaZ = 2;}    
}

void drawBuildings(GLenum mode){
    //changed y from 2.5 to 0.0
    glTranslatef(5.0f, 0.0f, 5.0f);

    int count = 1;
    for(int j = 0; j < BUILDINGS; j++){
        if(j != 0){
            glTranslatef(-MaxDistance, 0.0f, 10.0f);
        }
        int index;
        for(int i = 0; i < BUILDINGS; i++){
            if(mode == GL_SELECT){
                glLoadName(count);
            }
            
            index = (BUILDINGS * j) + i;

            glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
            //if previously shot don't draw

            if(Building_Map[index] == 1){
                drawHouseOne(1);
            }
            else if(Building_Map[index] == 2){
                drawHouse2Two();
            }
            else if(Building_Map[index] == 3){
                drawHouse3Three();
            }
            else if(Building_Map[index] == 4){
                drawHouseOne(2);
            }
            glTranslatef(10.0f, 0.0f, 0.0f);
            count++;
        }
    }
    //This will probably need some work
    //reset position
    glTranslatef(-(MaxDistance) - 5.0f, 0.0f, -95.0f);
}

void drawRoadOne(){
    glBegin(GL_QUADS);
        glNormal3f(0.0f,1.0f,0.0f);
        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.2f, 0.0f);     // Bottom Left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.2f, 5.0f);     // Bottom Right corner
        glTexCoord2f(1.0, 1.0); glVertex3f(MaxDistance, 0.2f, 5.0f);       // Top Right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(MaxDistance, 0.2f, 0.0f);     // Top Left corner
    glEnd();
}
void drawRoadTwo(){
    glBegin(GL_QUADS);
        glNormal3f(0.0f,1.0f,0.0f);
        glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.2f, 0.0f);     // Bottom Left corner
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, 0.2f, 205.0f);     // Bottom Right corner
        glTexCoord2f(1.0, 1.0); glVertex3f(5.0f, 0.2f, 205.0f);       // Top Right corner
        glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0f, 0.2f, 0.0f);     // Top Left corner
    glEnd();
}
void drawRoads(){
    //Draws Horizontal roads
    glTranslatef(-2.5f, 0.0f, -2.5f);
    for(int i = 0; i < 21; i++){
        drawRoadOne();
        glTranslatef(0.0f, 0.0f, 10.0f);
    }
    glTranslatef(2.5f, 0.0f, -207.5f);
    //Draws Vertical roads
    glTranslatef(-2.5f, 0.0f, -2.5f);
    for(int i = 0; i < 21; i++){
        drawRoadTwo();
        glTranslatef(10.0f, 0.0f, 0.0f);
    }
    glTranslatef(-207.5f, 0.0f, 2.5f);
    
}

void display(void){
    if(pause == 0){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        robotOrientation();
        gluLookAt(deltaX+camX, deltaY+camY, deltaZ+camZ, 0.0+deltaX, 0.0, 0.0+deltaZ, 0.0, 1.0, 0.0);

        //Draw ground
        glBegin(GL_QUADS);
            glColor4f(0.0f,1.0f,0.0f,0.0f); //Green
			glVertex3f(-5.0f, 0.1f, -5.0f);
			glVertex3f(-5.0f, 0.1f, (float)(MaxDistance)+5.0f);
			glVertex3f((float)(MaxDistance)+5.0f,0.0f,(float)(MaxDistance)+5.0f);
			glVertex3f((float)(MaxDistance)+5.0f,0.0f,-5.0f);
		glEnd();

        //Draw Roads
        drawRoads();
        //Draw Robot
        drawRobot();
        //Draw Buildings
        drawBuildings(GL_RENDER);

        glutSwapBuffers();
    }
}

void init(int width, int height){
    glClearColor(0.0f,0.0f,0.7f,1.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	
	glShadeModel(GL_SMOOTH);

	//setup quadratic
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);
	
	//Set up the sun.
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	
	glEnable(GL_DEPTH_TEST);
}
void releaseKey(int key, int x, int y)
{
   switch(key)
   {
      case GLUT_KEY_F2:
	 f2down = false;
	 headDeg=0.0;
	 break;
      case GLUT_KEY_F3:
	 f3down = false;
	 headDeg=0.0;
	 break;
   }
}
void keyPressControl(int key, int x, int y){
    if(pause == 0){
        switch(key){
            case GLUT_KEY_F1:
                //Turn head to face forward
                printf("KEY: F1\n");
                headDeg = 0.0;
                break;
            case GLUT_KEY_F2:
                //Turn head to the right if released back to centre
                printf("KEY: F2\n");
                f2down = true;
                break;
            case GLUT_KEY_F3:
                //Turn head to the left if released back to centre
                printf("KEY: F3\n");
                f3down = true;
                break;
            case GLUT_KEY_F4:
                //returns to default view
                printf("KEY: F4\n");
                camStorX = -8;//camX;
                camStorY = 8;//camY;
                camStorZ = 0;//camZ;
                view = 0;
                break;
            case GLUT_KEY_F5:
                //view at 225 degrees, close view
                printf("KEY: F5\n");
                camStorX = -8;
                camStorY = 10;
                camStorZ = -12;
                view = 1;
                break;
            case GLUT_KEY_F6:
                //view at 315 degrees, close view
                printf("KEY: F6\n");
                camStorX = -8;
                camStorY = 10;
                camStorZ = 12;
                view = 2;
                break;
            case GLUT_KEY_F7:
                //view at 45 degrees, close view
                printf("KEY: F7\n");
                camStorX = 8;
                camStorY = 10;
                camStorZ = 12;
                view = 3;
                break;
            case GLUT_KEY_F8:
                //view at 135 degrees, close view
                printf("KEY: F8\n");
                camStorX = 8;
                camStorY = 10;
                camStorZ = -12;
                view = 4;
                break;
            case GLUT_KEY_F9:
                //view at 225 degrees, far view
                printf("KEY: F9\n");
                camStorX = -10;
                camStorY = 14;
                camStorZ = -16;
                view = 5;
                break;
            case GLUT_KEY_F10:
                //view at 315 degrees, far view
                printf("KEY: F10\n");
                camStorX = -10;
                camStorY = 14;
                camStorZ = 16;
                view = 6;
                break;
            case GLUT_KEY_F11:
                //view at 45 degrees, far view
                printf("KEY: F11\n");
                camStorX = 10;
                camStorY = 14;
                camStorZ = 16;
                view = 7;
                break;
            case GLUT_KEY_F12:
                //view at 135 degrees, far view
                printf("KEY: F12\n");
                camStorX = 10;
                camStorY = 14;
                camStorZ = -16;
                view = 8;
                break;
        }
    }
}
void upKey(unsigned char key, int x, int y)
{
   switch(key)
   {
      case 'z':
	 zdown = false;
	 break;
   }
}
void pressKey(unsigned char key, int x, int y){
    if(pause == 0){
        switch(key){
            case 'p':
                printf("KEY: p press detected [PAUSE GAME]\n");
                pause = 1;
                break;
            case 'z':
				zdown = true;
                break;
            case 'r':
                printf ("KEY: r [RESET ROBOT]");
                deltaX = 0;
                deltaZ = 0;

                initMap();

                RobotX = 0.0;
                RobotZ = 0.0;
                RobotOrient = 0.0;
                antDeg = 0.0;
                antSpeed = 0.1;
                break;
            case 'q':
                printf("KEY: Q [LEFT TURN]\n");
                //These need to be modified to account for new roads
                if(RobotX % 10 <= 2 || RobotX % 10 >= 8){
					if(RobotZ % 10 <= 2 || RobotZ % 10 >= 8){
						RobotOrient -= 90.0;
					}
					else{
                        printf("Sorry, Not a valid street\n");
                        }
				}
				else{
                    printf("Sorry, Not a valid street\n");
                    }
				if(RobotOrient < 0.0){
					RobotOrient += 360;
				}
			    break;
            case 'a':
                printf("KEY: A [RIGHT TURN]\n");
				if(RobotX % 10 <= 2 || RobotX % 10 >= 8){
					if(RobotZ % 10 <= 2 || RobotZ % 10 >= 8){
						RobotOrient += 90.0;
					}
					else{
                        printf("Sorry, Not a valid street\n");
                        }
				}
				else{printf("Sorry, Not a valid street\n");
                }
				if(RobotOrient >= 360){
					RobotOrient -= 360;
				}
			    break;
            case 'c':
                printf ("Current Coordinates X(%i) Z(%i)\n", deltaX, deltaZ);
                break;
            default:
                break;
        }
    }
    else{
		switch(key)
		{
			case 'p':
				printf ("KEY: p press detected [UNPAUSE GAME]\n");
				pause = 0;
			    break;
			default:
				printf("Game Paused press 'p' to unpause\n");
			    break;
		 }
	}
}
void update()
{
   if (f2down)
      headDeg += 2.0;
   if (f3down)
      headDeg -= 2.0;
   if (zdown){
      if(RobotOrient==0.0){
	 if(deltaX>=0 && deltaX<=(MaxDistance)){
	    deltaX++;
	    RobotX+=1.0;
	 }
	 else{
	    deltaX--;
	    RobotX-=1.0;
	 }
      }
      else if(RobotOrient==90.0){
	 if(deltaZ>=0 && deltaZ<=(MaxDistance)){
	    deltaZ++;
	    RobotZ+=1.0;
	 }
	 else{
	    deltaZ--;
	    RobotZ-=1.0;
	 }
      }
      else if(RobotOrient==180.0){
	 if(deltaX>=0&&deltaX<=(MaxDistance)){
	    deltaX--;
	    RobotX-=1.0;
	 }
	 else{
	    deltaX++;
	    RobotX+=1.0;
	 }
      }
      else if(RobotOrient==270.0){
	 if(deltaZ>=0&&deltaZ<=(float)(MaxDistance)){
	    deltaZ--;
	    RobotZ-=1.0;
	 }
	 else{
	    deltaZ++;
	    RobotZ+=1.0;
	 }
      }
      else{
	 printf("ERROR UNDEFINED DIRECTION\n");
      }
      antDeg+=antSpeed;
   }
   display();
}
void processHits(GLint hits, GLuint buffer[]){
    GLint *ptr;

    ptr = (GLint *) buffer;
    ptr += 3;
    
    if(view == 0){
        int check = Building_Map[*ptr-1];
        //Remove Building
        if(check < 4){
            if(hits == 1){
                if(check == 1){
                    Building_Map[*ptr-1] = 0;
                }
                else if(check == 2){
                    Building_Map[*ptr-1] = 1;
                }
                else if(check == 3){
                    Building_Map[*ptr-1] = 2;
                }
            }
            else if(hits == 2){
                if(check == 1){
                    Building_Map[*ptr-1] = 0;
                }
                else if(check == 2){
                    Building_Map[*ptr-1] = 0;
                }
                else if(check == 3){
                    Building_Map[*ptr-1] = 1;
                }
            }
            else if (hits >= 3){
                if(check == 1){
                    Building_Map[*ptr-1] = 0;
                }
                else if(check == 2){
                    Building_Map[*ptr-1] = 0;
                }
                else if(check == 3){
                    Building_Map[*ptr-1] = 0;
                }
            }
        }
    }
    else{

    }
}

void resize(int width, int height){
    //Prevent divide by zero errors
	if(height <= 0){
		height = 1;
    }
	glViewport(0,0, (GLsizei)width, (GLsizei)height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(60.0f, (float)width/(float)height, 1.0f, 100.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	Window_Width = width;
	Window_Height = height;
}

void mouse(int button, int state, int x, int y){
	GLuint selectBuf[SIZE];
	GLint hits;
	GLint viewport[4];
	
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		glGetIntegerv (GL_VIEWPORT, viewport);
		glSelectBuffer(SIZE, selectBuf);
		
		glRenderMode(GL_SELECT);
		glInitNames();
		glPushName(0);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		
		gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3]-y), 5.0, 5.0, viewport);
		
		gluPerspective(60.0f, (float)Window_Width/(float)Window_Height, 1.0f, 100.0f);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		robotOrientation();

		gluLookAt(deltaX+camX, deltaY+camY, deltaZ+camZ, 0.0 + deltaX, 0.0, 0.0 + deltaZ, 0.0, 1.0, 0.0);
		
		drawBuildings(GL_SELECT);
		
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glFlush();
		
		glMatrixMode(GL_MODELVIEW);
		
		hits = glRenderMode(GL_RENDER);
		
		processHits(hits, selectBuf);
		
		glutPostRedisplay();
    }
}

int main(int argc, char **argv){
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(Window_Width, Window_Height);
    Window_ID = glutCreateWindow(PROGRAM_TITLE);
    glutInitWindowPosition(0,0);

    glutDisplayFunc(&display);
    //changed from &display
    glutIdleFunc(&update);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    glutSpecialFunc(&keyPressControl);
    glutSpecialUpFunc(&releaseKey);
    glutKeyboardFunc(&pressKey);
    glutKeyboardUpFunc(&upKey);
    glutReshapeFunc(&resize);
    glutMouseFunc(&mouse);

    init(Window_Width, Window_Height);

    initMap();

    init(Window_Width, Window_Height);
    glutMainLoop();
    return 1;
}