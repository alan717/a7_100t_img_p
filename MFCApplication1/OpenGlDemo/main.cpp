#define GLM_ENABLE_EXPERIMENTAL
#include<iostream>
//#include<GL/glew.h>
//#include<GL/gl.h>
//#include<GL/GLU.h>
//#include<GLFW/glfw3.h>
//#include<glm/glm.hpp>
//#include<glm/gtc/matrix_transform.hpp>
//#include<glm/gtx/transform2.hpp>
#include"scenerunner.h"
#include"scenebasic.h"



#include<memory>
using glm::vec3;
using glm::mat4;

static std::map< std::string, std::string > sceneData = {
	{ "basic", "Basic scene." },
{ "basic-attrib", "Prints active attributes." },
{ "basic-debug", "Display debug messages." },
{ "basic-uniform", "Basic scene with a uniform variable." },
{ "basic-uniform-block", "Scene with a uniform block variable." }
};
int main(int argc,char *argv[])
{
	//GLenum err=glfwInit();
	//if (GLEW_OK != err)
	//{
	//	fprintf(stderr, "Error ininitial: -%s\n",glewGetErrorString(err));
	//}
	/*if (!glfwInit()) exit(EXIT_FAILURE);
	if (!GLEW_ARB_vertex_program)
	{
		fprintf(stderr, "ARB_vertex_programming is missing !\n");
	}*/
	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneData);
	SceneRunner runner("Chapter 1 - " + recipe, 500, 500);
	std::unique_ptr<Scene> scene;
	if (recipe == "basic") {
		scene = std::unique_ptr<Scene>(new SceneBasic());
	}
	//else if (recipe == "basic-attrib") {
	//	scene = std::unique_ptr<Scene>(new SceneBasic_Attrib());
	//}
	//else if (recipe == "basic-uniform") {
	//	scene = std::unique_ptr<Scene>(new SceneBasic_Uniform());
	//}
	//else if (recipe == "basic-uniform-block") {
	//	scene = std::unique_ptr<Scene>(new SceneBasic_UniformBlock());
	//}
	//else if (recipe == "basic-debug") {
	//	scene = std::unique_ptr<Scene>(new SceneBasic_Debug());
	//}
	return runner.run(*scene);
}