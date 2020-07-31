#include<GL/glut.h>
#include<GL/gl.h>
#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<time.h>

using namespace std;

#define TIME_REDRAW  1

GLint g_maxIter=1;
GLfloat g_angle1 = 0;

GLfloat xComplexMin=-2.0,xComplexMax=0.5;
GLfloat yComplexMin=-1.2,yComplexMax=1.2;

GLfloat complexWidth=xComplexMax-xComplexMin;
GLfloat complexHeight=yComplexMax-yComplexMin;

class complexNum{
public:
	GLfloat x,y;
};
struct color{GLfloat r,g,b;};

void init()
{
	glClearColor(1.0,1.0,1.0,0.0);
}
void plotPoint(complexNum z)
{
	glBegin(GL_POINTS);
		glVertex2f(z.x,z.y);
	glEnd();

}

complexNum complexSquare(complexNum z)
{
	complexNum zSquare;
	zSquare.x=z.x*z.x-z.y*z.y;
	zSquare.y=2*z.x*z.y;
	return zSquare;
}

GLint mandelSqTransf(complexNum z0,GLint maxIter)
{
	complexNum z=z0;
	GLint count=0;
	while((z.x*z.x+z.y*z.y<=4.0)&&(count<maxIter))
	{
		z=complexSquare(z);
		z.x+=z0.x;
		z.y+=z0.y;
		count++;
	}
	return count;
}

void mandelbrot(GLint nx,GLint ny,GLint maxIter)
{
	complexNum z,zIncr;
	color ptColor;
	GLint iterCount;
	zIncr.x=complexWidth/GLfloat(nx);
	zIncr.y=complexHeight/GLfloat(ny);
	for(z.x=xComplexMin;z.x<xComplexMax;z.x+=zIncr.x)
		for(z.y=yComplexMin;z.y<yComplexMax;z.y+=zIncr.y)
		{
			iterCount=mandelSqTransf(z,maxIter);
			if(iterCount>=maxIter)
				ptColor.r=ptColor.g=ptColor.b=0.0;
			else if (iterCount>(maxIter/8))
			{
				ptColor.r=1.0;
				ptColor.g=0.5;
				ptColor.b=0.0;
			}
			else if(iterCount>(maxIter/10))
			{
				ptColor.r=1.0;
				ptColor.g=ptColor.b=0.0;
			}
			else if(iterCount>(maxIter/20))
			{
				ptColor.r=0.5;
				ptColor.g=ptColor.b=0.0;
			}
			else if(iterCount>(maxIter/40))
			{
				ptColor.r=ptColor.g=1.0;
				ptColor.b=0.0;
			}
			else if(iterCount>(maxIter/100))
			{
				ptColor.r=ptColor.b=0.0;
				ptColor.g=0.3;
			}
			else
			{
				ptColor.r=0.0;
				ptColor.g=ptColor.b=1.0;
			}
		glColor3f(ptColor.r,ptColor.g,ptColor.b);
		plotPoint(z);
		}
}

void display()
{
	GLint nx=1000,ny=1000;
	glClear(GL_COLOR_BUFFER_BIT);
	mandelbrot(nx,ny,g_maxIter++);
	glPushMatrix();
	glRotatef(g_angle1, 1.0, 0.0, 0.0);
	glPushMatrix();
	glFlush();

	cout<<"g_maxIter："<<g_maxIter<<endl;
}
void reshape(int w,int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xComplexMin,xComplexMax,yComplexMin,yComplexMax);
	glClear(GL_COLOR_BUFFER_BIT);
}

void on_timer(int value)
{
	g_angle1 = (GLfloat) fmod(g_angle1 + 0.8, 360.0);
	glutPostRedisplay();
	glutTimerFunc(TIME_REDRAW, on_timer, 1);
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(600,400);
	glutInitWindowPosition(100,100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(TIME_REDRAW, on_timer, 1);
	glutMainLoop();
	return 0;

}