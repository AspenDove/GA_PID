//#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include "CartPole.h"
#include "GA.h"
#include <iostream>
#include "motor.h"

//CartPole env;

/*
static void init(void)
{
	glShadeModel(GL_FLAT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static bool rendering;
	if (!rendering)
	{
		polulation.evolution(env);
		std::cout << polulation.evalue(polulation.father) << std::endl;
		rendering = true;
		env.reset();
	}
	else
	{	
		if (env.update(polulation.adjust({ env.x,env.v,env.theta,env.omiga }, polulation.father)))
		{
			glColor4f(1.0, 0.0, 0.0, 0.0);
			DrawCart(env.x, -0.5);

			glColor4f(0.0, 1.0, 0.0, 0.0);
			DrawPole(env.x, -0.5, env.theta);
			glFlush();
			rendering = true;
		}
		else
		{
			rendering = false;
			
		}
	}
	//env.update(0);
	//glColor4f(1.0, 0.0, 0.0, 0.0);
	//DrawCart(env.x, -0.5);

	//glColor4f(0.0, 1.0, 0.0, 0.0);
	//DrawPole(env.x, -0.5, env.theta);
	glFlush();


}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLint)w, (GLint)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/*if (w <= h)
		glOrtho(-1., 1., -1.*(GLfloat)h / (GLfloat)w,
			1.*(GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-1.*(GLfloat)w / (GLfloat)h,
			1.*(GLfloat)w / (GLfloat)h, -1., 1., -10.0, 10.0);
	glOrtho(-2, 2, -2., 2., -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display();
}
*/
void keyboard(unsigned char key, int x, int y)
{
}

int main(int argc, char** argv)
{
	/*	pid[i].Kp = 2000.f / 1.e2f;
		pid[i].Ti = 655.f / 1.e4f;
		pid[i].Td = 349.f / 1.e6;
	*/
	motor motor;
	GA polulation(motor, 3, 5,0.5);
	while (true)
	{
		polulation.evolution(motor);
		//motor.reset();
		std::cout << "father:" << polulation.evalue(motor, polulation.father) << std::endl;
		cout << "Kp:" << polulation.father[0] << "\nTi:" << polulation.father[1] << "\nTd:" << polulation.father[2] << "\n";
		//motor.reset();
	}

	return 0;
}