// OpenGL画三维分形
#include <GL/glut.h>
#pragma comment(lib, "glut32.lib")
 
#define MAXCOLOR 7
float colortab[MAXCOLOR][4]=
{
	0.2,        0,            0.4,    0.0,    //紫
	0.2,        0,            0.5,    0.0,    //紫
	0.2,        0,            0.6,    0.0,    //紫
	0.3,        0,            1.0,    0.0,    //紫
	0.196,      0.3039,       0.296,  0.0,    //深绿
	0.2784,     1.0,          0.1843, 0.0,    //浅绿
	1.0,        0.3068,       0.1,    0.0     //橙
};
 
 
//画正方体
void Square(float center[3], float size, float color[4])
{
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);  // 将背景颜色和散射颜色设置成同一颜色
	glTranslatef(center[0], center[1], center[2]);        //平衡坐标系
	glutSolidCube(size);//利用库函数绘制一个半径为1的球体。
	glPopMatrix();
}
 
void init(void)
{
	glClearColor(0.5, 0.5, 0.5, 0.0);    //清理颜色，为黑色，（也可认为是背景颜色）
 
	float light_diffuse[]= { 1.0, 1.0, 1.0, 1.0};       //有灯光才能体现材质的效果，他的灯光的强度是与的关系。貌似是两个相乘什么的。(0-1)范围。
	float light_position[] = { 0, 3, 2.0, 0.0 };        //设置点光源的矩阵，这个向量也忒奇怪了，1不跟着变，0跟着变，设置为透视之后又是1跟着变，0不跟着变。
	float light_specular[] = { 1.0, 1.0, 0.0, 1.0 };    //反射光
	float light_ambient[] = {0.5, 0.5, 0.5, 1.0};
 
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);        //，漫射光会产生漫射的效果和高光的效果
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);    //点光源没有漫射光的效果，会一直是黑色的。但会有高光反射的效果。
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);        //反射光基本没有效果。可能是我不知道吧
 
	GLfloat no_mat[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat mat_specular[] =  {1.0, 1.0, 1.0, 1.0};
	GLfloat hig_shininess[] = {100.0};
	glMaterialfv(GL_FRONT,GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS, hig_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
 
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    //如果什么都不设置，GL_LIGHT有默认的值。
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);    
	glEnable(GL_DEPTH_TEST);
}
 
//在给定中心点周围画6个正方体
void SurroundSquare(float center[3], float size, float color[4])
{
	//周围6个正方体的中心
	float halfcenter[6][3] = 
	{
		center[0] + size * 3/4.0, center[1], center[2],
		center[0] - size * 3/4.0, center[1], center[2],
		center[0] , center[1] + size * 3/4.0, center[2],
		center[0] , center[1] - size * 3/4.0, center[2],
		center[0] , center[1], center[2] + size * 3/4.0,
		center[0] , center[1], center[2] - size * 3/4.0,
	};
 
	for (int i=0; i<6; i++)
	{
		Square(halfcenter[i], size/2, color);
	}
}
 
void Iteration(float center[3], float size, int nIter)
{
	if(nIter<0)return;
 
	SurroundSquare(center, size, colortab[nIter]);
 
	float halfcenter[6][3] = 
	{
		center[0] + size * 3/4.0, center[1], center[2],
		center[0] - size * 3/4.0, center[1], center[2],
		center[0] , center[1] + size * 3/4.0, center[2],
		center[0] , center[1] - size * 3/4.0, center[2],
		center[0] , center[1], center[2] + size * 3/4.0,
		center[0] , center[1], center[2] - size * 3/4.0,
	};
 
	for (int i=0; i<6; i++)
	{
		Iteration(halfcenter[i], size/2, nIter-1);
	}
}
 
void display(void)
{
	//清除颜色缓存和深度缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	glLoadIdentity();
 
	gluLookAt(1, -1, 3,   0, 0, 0,   0.4, -3, 0.5);        //视点转换
	float center[] = {0.0f, 0.0f, 0.0f};
	float radius = 2;
	float color[] = {1.0, 0.1, 0.0, 0};
	Square(center, radius, color);
	Iteration(center, radius, 6);
	glutSwapBuffers();      //交换双缓存
}
 
 
void reshape(int width,int height)
{
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-3.0, 3.0, -3.0 * height / width, 3.0* height / width, -5, 5);    //为了不变形，则要长和宽成比例
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
 
 
int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);    //使用双缓存模式和深度缓存
	glutInitWindowSize(600,600);
	glutInitWindowPosition(200,200);
	glutCreateWindow("三维分形");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);   //设置空闲时用的函数
	glutMainLoop();
	return 0;
}
