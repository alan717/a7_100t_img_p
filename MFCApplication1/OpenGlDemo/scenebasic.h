#ifndef SCENEBASIC_H
#define SCENEBASIC_H

#include "scene.h"
#include "glad.h"

class SceneBasic : public Scene
{
private:
	GLuint vaoHandle;

	void linkMe(GLint vertShader, GLint fragShader);

public:
	SceneBasic();

	void initScene();
	void update(float t);
	void render();
	void resize(int, int);
};

#endif // SCENEBASIC_H