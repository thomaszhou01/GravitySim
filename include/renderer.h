#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <planet.h>
#include <resourceLoader.h>
#include <camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//does everything with rendering
class Renderer {
public:
	Renderer();
	void renderPlanets(Planet* p, std::vector<Planet*> suns, Camera& cam);
	void renderUI(Camera& cam);
	~Renderer();
private:
	void init();
	unsigned int VAO;
	unsigned int UIVAO;
	unsigned int cubemapTexture;
	TextureLoader textureLoader;
	Shader* shader;
	Shader* shaderSun;
	Shader* skyboxShader;
	Shader* bloomBlur;
	Shader* bloomFinal;
};