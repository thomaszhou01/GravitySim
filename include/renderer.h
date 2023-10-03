#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <planet.h>
#include <shader.h>
#include <camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//does everything with rendering
class Renderer {
public:
	Renderer();
	void render(Planet* p, std::vector<Planet*> suns, Camera& cam);
	~Renderer();
private:
	void init();
	void addLight();
	unsigned int VAO;
	Shader* shader;
	Shader* shaderSun;
};