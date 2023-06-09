// OpenGL/GLUT starter kit for Windows 7 and Visual Studio 2010
// Created spring, 2011
//
// This is a starting point for OpenGl applications.
// Add code to the "display" function below, or otherwise
// modify this file to get your desired results.
//
// For the first assignment, add this file to an empty Windows Console project
//		and then compile and run it as is.
// NOTE: You should also have glut.h,
// glut32.dll, and glut32.lib in the directory of your project.
// OR, see GlutDirectories.txt for a better place to put them.
#pragma once

#include <cmath>
#include <cstring>
#include <ctime>
#include <iostream>
#include "glut.h"
#include "Maze.h"
#include "Rat.h"
#include "HelperFunctions.h"
#include "graphics.h"


// Global Variables (Only what you need!)

double screen_x = 700;
double screen_y = 500;

viewtype current_view = top_view;

//Maze gMaze;
Rat gRat;
bool gMoveForward = false;
bool gMoveBackwards = false;
bool gTurnRight = false;
bool gTurnLeft = false;
double gSpeed = .001;
double gWaterHeight = .25;
double gFloatHeight = .25;
double gTerrainConst1 = 1.0;
double gTerrainConst2 = 1.0;

// global types and variable:



//
// GLUT callback functions
//

// This callback function gets called by the Glut
// system whenever it decides things need to be redrawn.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (current_view == perspective_view)
	{
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		gluLookAt(-6, -6, 50, 40, 40, 0, 0, 0, 1);
	}	//camera position, point looking at, tilt
	else if (current_view == top_view)
	{
		glDisable(GL_DEPTH_TEST);
		glLoadIdentity();
	}
	else // current_view == rat_view
	{
		glEnable(GL_DEPTH_TEST);
		glLoadIdentity();
		double z_level = gRat.GetZ() +.5; //rat.GetZ(x,y);
		double x = gRat.GetX();
		double y = gRat.GetY();
		//double dx = gRat.GetDX(GetDeltaTime()); //2*rat.GetDXUnscaled()
		//double dy = gRat.GetDY(GetDeltaTime()); //2*rat.GetDYUnscaled()

		double radians = gRat.GetDegrees() / 180 * 3.14;// / 15926;
		double dx = cos(radians) * 2;
		double dy = sin(radians) * 2;

		double at_x = x + dx;
		double at_y = y + dy;
		double at_z = getTerrainHeight(at_x, at_y);// +gFloatHeight;
		if (at_z <= gWaterHeight) {
			at_z = gWaterHeight + .1;
		}
		//float z2 = rat.GetZ(at_x, at_y);
		//float downward_tilt = -0.1;
		at_z = at_z;// -downward_tilt;
		gluLookAt(x, y, z_level, at_x, at_y, at_z, 0, 0, 1);
	}

	if (gMoveForward) {
		gRat.Scurry(GetDeltaTime());
	}
	if (gMoveBackwards) gRat.Scurry(GetDeltaTime(), true);
	if (gTurnLeft) gRat.SpinLeft(GetDeltaTime());
	else if (gTurnRight) gRat.SpinRight(GetDeltaTime());
	// Test lines that draw all three shapes and some text.
	// Delete these when you get your code working.
	glColor3d(0,0,1);
	
	//Draw the rat, make your own class with Rat.Draw()
	//Make a method to find start cell and calculate starting position
	//Code_keys.cpp/.h available for download
	

	glColor3d(0,0,0);
	DrawMap();
	
	gRat.Draw();
	glutSwapBuffers();
	glutPostRedisplay();
}

void keyboardup(unsigned char c, int x, int y) 
{
	switch (c)
	{
		case 27: // escape character means to quit the program
			exit(0);
			break;
		case 'b':
			// do something when 'b' character is hit.
			break;
		case 'w':
			gMoveForward = false;
			break;
		case 'a':
			gTurnLeft = false;
			break;
		case 's':
			gMoveBackwards = false;
			break;
		case 'd':
			gTurnRight = false;
			break;
		default:
			return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}




void SetTopView(int w, int h)
{
	// go into 2D mode
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double world_margin_x = 0.5;
	double world_margin_y = 0.5;
	gluOrtho2D(-world_margin_x, 100 + world_margin_x,
		-world_margin_y, 100 + world_margin_y);
	glMatrixMode(GL_MODELVIEW);
}

void SetPerspectiveView(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble)w / (GLdouble)h;
	gluPerspective(
		/* field of view in degree */ 38.0,
		/* aspect ratio */ aspectRatio,
		/* Z near */ .1, /* Z far */ 200.0);
	glMatrixMode(GL_MODELVIEW);
}

void SetRatView(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (GLdouble)w / (GLdouble)h;
	gluPerspective(
		/* field of view in degree */ 38.0,
		/* aspect ratio */ aspectRatio,
		/* Z near */ 0.1, /* Z far */ 50.0);
	glMatrixMode(GL_MODELVIEW);
}


// reshape:
void reshape(int w, int h)
{
	screen_x = w;
	screen_y = h;
	glViewport(0, 0, w, h);

	if (current_view == top_view)
	{
		SetTopView(w, h);
	}
	else if (current_view == perspective_view)
	{
		SetPerspectiveView(w, h);
	}
	else if (current_view == rat_view)
	{
		SetRatView(w, h);
	}
}
/*
// This callback function gets called by the Glut
// system whenever the window is resized by the user.
void reshape(int w, int h)
{
	// Reset our global variables to the new width and height.
	screen_x = w;
	screen_y = h;

	// Set the pixel resolution of the final picture (Screen coordinates).
	glViewport(0, 0, w, h);

	// Set the projection mode to 2D orthographic, and set the world coordinates:
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-.5, M+0.5, -.5, N+.5);
	glMatrixMode(GL_MODELVIEW);

}*/

// This callback function gets called by the Glut
// system whenever a key is pressed.
void keyboard(unsigned char c, int x, int y)
{
	switch (c)
	{
	case 27: // escape character means to quit the program
		exit(0);
		break;
	case 'w':
		gMoveForward = true;
		break;
	case 'a':
		gTurnLeft = true;
		break;
	case 's':
		gMoveBackwards = true;
		break;
	case 'd':
		gTurnRight = true;
		break;
	case 'r':
		current_view = rat_view;
		SetRatView(screen_x, screen_y);
		break;
	case 'p':
		current_view = perspective_view;
		SetPerspectiveView(screen_x, screen_y);
		break;
	case 't':
		current_view = top_view;
		SetTopView(screen_x, screen_y);
		break;
	case '1':
		increaseRuggedness();
		break;
	case '2':
		decreaseRuggedness();
		break;
	case '3':
		raiseWaterLevel();
		break;
	case '4':
		lowerWaterLevel();
		break;
	default:
		return; // if we don't care, return without glutPostRedisplay()
	}

	glutPostRedisplay();
}

// This callback function gets called by the Glut
// system whenever any mouse button goes up or down.
void mouse(int mouse_button, int state, int x, int y)
{
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		gTurnLeft = true;
	}
	if (mouse_button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	{
		gTurnLeft = false;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) 
	{
		gMoveForward = true;
	}
	if (mouse_button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) 
	{
		gMoveForward = false;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		gTurnRight = false;
	}
	if (mouse_button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		gTurnRight = true;
	}
	glutPostRedisplay();
}

// Your initialization code goes here.
void InitializeMyStuff()
{
	int startx = 50, starty = 50;
	srand(time(0));
	gRat.SetPosition(startx, starty, getTerrainHeight(startx, starty) + gFloatHeight, 0);

}


int main(int argc, char **argv)
{
	//PlaySOund("file.wav", NULL, ASync); include windows.h
	glutInit(&argc, argv);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_x, screen_y);
	glutInitWindowPosition(50, 50);

	int fullscreen = 0;
	//int id1, id2;
	if (fullscreen) 
	{
		glutGameModeString("800x600:32");
		glutEnterGameMode();
	} 
	else 
	{
		glutCreateWindow("This appears in the title bar");
		// id2 = glutCreateWindow("Second window");
	}

	//glutSetWindow(id2);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardup);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	glColor3d(0,0,0); // forground color
	glClearColor(1, 1, 1, 0); // background color
	InitializeMyStuff();

	glutMainLoop();

	return 0;
}


/*
void AdjustWaterHeight(double t) {
	waterHeight = -2 + .2 * sin(t);
}*/

void DrawMap() {
	//Draw Water
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glColor4d(0.1, 0.2, 0.9, 0.8);
	int R = 10;
	int G = 30;
	int B = 200;
	glColor3ub(R, G, B);
	glBegin(GL_QUADS);
	glVertex3d(0, 0, gWaterHeight);
	glVertex3d(0, 101, gWaterHeight);
	glVertex3d(101, 101, gWaterHeight);
	glVertex3d(101, 0, gWaterHeight);
	glEnd();
	//Draw really big rect
	int z;
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			unsigned char r = (unsigned char)((i * 34243 + j * 45543) % 256);
			unsigned char g = (unsigned char)((i * 97654 + j * 36721) % 256);
			unsigned char b = (unsigned char)((i * 67467 + j * 22345) % 256);
			
			glColor3ub(r, g, b);
			glBegin(GL_QUADS);
			glVertex3d(i, j, getTerrainHeight(i, j));
			glVertex3d(i, j + 1, getTerrainHeight(i, j+1));
			glVertex3d(i + 1, j + 1, getTerrainHeight(i+1, j+1));
			glVertex3d(i+1, j, getTerrainHeight(i+1, j));
			glEnd();
		}
	}
}

double getTerrainHeight(double x, double y) {
	return gTerrainConst1 * sin(x * 0.5) + gTerrainConst2 * sin(y * 0.5) + 4 * sin(y * .2) + 3 * cos(x * .3);
}

void increaseRuggedness() {
	gTerrainConst1 += .1;
	gTerrainConst2 += .2;
}
void decreaseRuggedness() {
	gTerrainConst1 -= .1;
	gTerrainConst2 -= .2;
}
void raiseWaterLevel() {
	gWaterHeight += 0.05;
}
void lowerWaterLevel() {
	gWaterHeight -= 0.05;
}
