#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "RubikCube.h"
#include "Color.h"
#include "Cube.h"
#include "Model.h"
#include "CubeSolver.h"

#define SIZE 100
int TIME = 4;

const int width = 600;
const int height = 600;
const int depth = 300;

float xrot = 0;
float yrot = 0;
float xdiff = 0;
float ydiff = 0;

bool mouseDown = false;

unsigned int color_brik[9] = { WHITE, YELLOW, BLUE, GREEN, PINK, ORANGE };

int timerOn = 0;
int automat = 0;

RubikCube cube;

void display()
{
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gluLookAt(
		0.0f, 0.0f, 3.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);

	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);

	glColor3f(1, 0, 0);

	glTranslatef(SIZE / -2.0, SIZE / -2.0, SIZE / -2.0);

	cube.draw();
	glPopMatrix();
	glutSwapBuffers();
}

void init()
{
	glClearColor(0.70, 0.70, 0.70, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(-width / 2, width / 2, -height / 2, height / 2, -depth / 2, depth / 2);
	glEnable(GL_DEPTH_TEST);
}

char getColor(int color)
{
	switch (color)
	{
	case YELLOW:
		return 'y';
	case WHITE:
		return 'w';
	case BLUE:
		return 'b';
	case GREEN:
		return 'g';
	case PINK:
		return 'r';
	case ORANGE:
		return 'o';
		break;
	default:
		std::cout << "Color: " << color << std::endl;
		break;
	}
}

void keyboard(unsigned char key, int, int)
{
	if (cube.getBrinkAnimation() == -1 && key >= '0' && key <= '6')
	{
		cube.rotation(key - '0');
		display();
	}
	else if (key == 'r')
	{
		/*system("cls");
		Cube test;
		test = test.U();
		test.printCube(test.theCubeData);

		std::cout << "------------------------------\n";

		Cube machine;
		machine = cube;*/

	

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				cout << getColor(cube.getColorDetails(i, 2, j)[3]) << " ";
			}
			cout << endl;
		}
		cout << endl;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				cout << getColor(cube.getColorDetails(j, 0, i)[2]) << " ";
			}
			cout << endl;
		}
		cout << endl;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				cout << getColor(cube.getColorDetails(i, j, 0)[1]) << " ";
			}
			cout << endl;
		}
		cout << endl;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				cout << getColor(cube.getColorDetails(i, j, 2)[0]) << " ";
			}
			cout << endl;
		}
		cout << endl;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				cout << getColor(cube.getColorDetails(0, i, j)[4]) << " ";
			}
			cout << endl;
		}
		cout << endl;

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				cout << getColor(cube.getColorDetails(2, i, j)[5]) << " ";
			}
			cout << endl;
		}
		cout << endl;

		/*CubeSolver solver;

		Cube answer = solver.solveTheCube(machine);

		vector<int> buffer = answer.getHistoryForCubikRubics();

		for (int i = 0; i < buffer.size(); i++)
			cube.pushMove(buffer[i]);
		automat = 1 - automat;*/
	}
	else if (key == 's')
	{
		std::ofstream output_file("output.txt");

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					for (int c = 0; c < 6; c++)
						output_file << cube.getColorDetails(i, j, k)[c] << std::endl;
	}
}

void timer(int = 0)
{
	glutTimerFunc(TIME, timer, 0);
	if (timerOn)
	{
		if (cube.getBrinkAnimation() == -1)
			keyboard(rand() % 6 + '0', 0, 0);
		else
			cube.rotation(cube.getBrinkAnimation());
	}
	else
	{
		if (cube.getBrinkAnimation() != -1)
			cube.rotation(cube.getBrinkAnimation());
	}

	if (automat)
	{
		if (cube.getBrinkAnimation() == -1)
		{
			cube.rotation();
		}
		else if (cube.emptyQueue())
			automat = 1 - automat;
	}

	display();
}

void mouseMotion(int x, int y)
{
	if (mouseDown)
	{
		yrot = x - xdiff;
		xrot = y + ydiff;

		glutPostRedisplay();
	}
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;

		xdiff = x - yrot;
		ydiff = -y + xrot;
	}
	else
		mouseDown = false;

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		timerOn = 1 - timerOn;

}

void graphics(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("lab8");
	init();
	glutDisplayFunc(display);
	glutTimerFunc(TIME, timer, 10);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouse);
}

void initCube(int argc, char** argv)
{
	cube.init(SIZE, color_brik);
	graphics(argc, argv);
}

void initCube(int argc, char** argv, unsigned int* color_position)
{
	cube.init(SIZE, color_brik, color_position);
	graphics(argc, argv);
}