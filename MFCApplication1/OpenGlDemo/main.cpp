#define GLM_ENABLE_EXPERIMENTAL

#include<iostream>
//#include<GL/glew.h>
//#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
//#include<glm/gtc/matrix_transform.hpp>
//#include<glm/gtx/transform2.hpp>
#include"scenerunner.h"
#include"scenebasic.h"
#include"scenebasic_attrib.h"
#include "scenebasic_uniform.h"
#include "scenebasic_uniformblock.h"

//#include<GL/gl.h>
//#include<GL/GLU.h>

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
	//GLenum err = glewInit();
	//if (GLEW_OK != err)
	//{
	//	fprintf(stderr, "Error initializing GLEW: %s\n",
	//	glewGetErrorString(err));
	//}
	
	/*GLenum glew_ok =glfwInit();
	if (glew_ok != err)
	{
		fprintf(stderr, "error ininitial: -%s\n", glfwGetErrorString(err));
	}
	*///if (!glfwinit()) exit(exit_failure);
	//if (!glew_arb_vertex_program)
	//{
	//	fprintf(stderr, "arb_vertex_programming is missing !\n");
	//}
	const GLubyte *renderer =glGetString(GL_RENDERER);
	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneData);
	SceneRunner runner("Chapter 1 - " + recipe, 500, 500);
	std::unique_ptr<Scene> scene;
	if (recipe == "basic") {
		scene = std::unique_ptr<Scene>(new SceneBasic());
	}
	else if (recipe == "basic-attrib") {
		scene = std::unique_ptr<Scene>(new SceneBasic_Attrib());
	}
	else if (recipe == "basic-uniform") {
		scene = std::unique_ptr<Scene>(new SceneBasic_Uniform());
	}
	else if (recipe == "basic-uniform-block") {
		scene = std::unique_ptr<Scene>(new SceneBasic_UniformBlock());
	}
	//else if (recipe == "basic-debug") {
	//	scene = std::unique_ptr<Scene>(new SceneBasic_Debug());
	//}
	return runner.run(*scene);
}