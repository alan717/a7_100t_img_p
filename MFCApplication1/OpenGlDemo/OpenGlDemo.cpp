// OpenGlDemo.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include <gl/glut.h>
//
//void renderWindow(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT);                           //当前背景色填充窗口
//	glColor3f(1, 0, 0);                                       //颜色:RGB.此处R:1,为红色.
//	glBegin(GL_TRIANGLES);                                  //开始:三角形
//	glVertex2f(-0.5f, -0.5f);                               //三个顶点坐标:
//	glVertex2d(0.5f, -0.5f);
//	glVertex2d(-0.0f, 0.5f);
//	glEnd();                                                //结束
//	glFlush();                                              //输出缓冲区 
//}
//int main(int argc, char *argv[])                            //主函数: 参数数量&参数值
//{
//	glutInit(&argc, argv);                                  //初始化glut: 接收主函数的参数
//	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);            //显示模式：颜色&缓冲
//	glutInitWindowPosition(0, 0);                           //窗口相对屏幕位置
//	glutInitWindowSize(720, 720);                           //窗口大小
//	glutCreateWindow("Hello, OpenGL!");                     //创建窗口: 标题
//	glutDisplayFunc(&renderWindow);                         //显示函数
//	glutMainLoop();                                         //循环
//	return 0;
//}
///////////////////////////////////
// square.cpp
//
// OpenGL program to draw a square.
// 
// Sumanta Guha.
///////////////////////////////////

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
//#  include <GL/glext.h>
//#pragma comment(lib, "glew32.lib") 
#endif

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 0.0, 0.0);

	// Draw a polygon with specified vertices.
	glBegin(GL_POLYGON);
	glVertex3f(20.0, 20.0, 0.0);
	glVertex3f(80.0, 20.0, 0.0);
	glVertex3f(80.0, 80.0, 0.0);
	glVertex3f(20.0, 80.0, 0.0);
	glEnd();

	glFlush();
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Main routine.
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("square.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
