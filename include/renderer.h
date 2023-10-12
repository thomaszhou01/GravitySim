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
	Renderer(int screenWidth, int screenHeight);
	void renderPlanets(Planet* p, std::vector<Planet*> suns, Camera& cam);
	void renderUI(Camera& cam);
	void frameBufferInit();
	void frameBufferFin();
	void frameBufferRender();
	~Renderer();
private:
	void init();
	int screenWidth;
	int screenHeight;
	unsigned int VAO;
	unsigned int UIVAO;
	unsigned int FBO;
	unsigned int quadVAO;
	unsigned int cubemapTexture;
	unsigned int textureFBO;
	TextureLoader textureLoader;
	Shader* shader;
	Shader* shaderSun;
	Shader* skyboxShader;
	Shader* bloomBlur;
	Shader* bloomFinal;
	Shader* frameBufferShader;
};