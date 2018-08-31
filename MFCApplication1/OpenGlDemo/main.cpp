#define GLM_ENABLE_EXPERIMENTAL
#include<iostream>
#include<GL/glew.h>
#include<GL/gl.h>
#include<GL/GLU.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtx/transform2.hpp>




using glm::vec3;
using glm::mat4;

int main()
{
	//GLenum err=glfwInit();
	//if (GLEW_OK != err)
	//{
	//	fprintf(stderr, "Error ininitial: -%s\n",glewGetErrorString(err));
	//}
	if (!glfwInit()) exit(EXIT_FAILURE);
	if (!GLEW_ARB_vertex_program)
	{
		fprintf(stderr, "ARB_vertex_programming is missing !\n");
	}

	return 0;
}