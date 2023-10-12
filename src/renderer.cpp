#include <renderer.h>
Renderer::Renderer(int screenWidth, int screenHeight) : screenWidth{ screenWidth }, screenHeight{ screenHeight } {
	this->init();
}

void Renderer::renderPlanets(Planet* p, std::vector<Planet*> suns, Camera& cam) {
	Shader* temp;
	if (!p->getStationary()) {
		temp = shader;
		temp->use();
		temp->setInt("numLights", suns.size());
		temp->setVec3("material.ambient", 0.5f, 0.5f, 0.5f);
		temp->setVec3("material.diffuse", 0.5f, 0.5f, 0.5f);
		temp->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		temp->setFloat("material.shininess", 64.0f);
		//lighting
		for (int i = 0; i < suns.size(); i++) {
			std::string num = std::to_string(i);

			temp->setVec3("light[" + num + "].position", suns[i]->getPosition());
			temp->setVec3("light[" + num + "].ambient", 0.05f, 0.05f, 0.05f);
			temp->setVec3("light[" + num + "].diffuse", 0.8f, 0.8f, 0.8f);
			temp->setVec3("light[" + num + "].specular", 1.0f, 1.0f, 1.0f);
			temp->setFloat("light[" + num + "].constant", 1.0f);
			temp->setFloat("light[" + num + "].linear", 0.0014f);
			temp->setFloat("light[" + num + "].quadratic", 0.000007f);
		}
	}
	else {
		temp = shaderSun;
		temp->use();
		temp->setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
		temp->setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
		temp->setVec3("material.specular", 1.0f, 1.0f, 1.0f);
		temp->setFloat("material.shininess", 64.0f);
	}
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, p->getPosition());
	temp->setMat4("model", model);
	glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 10000.0f);
	glm::mat4 view = cam.GetViewMatrix();
	temp->setMat4("projection", projection);
	temp->setMat4("view", view);
	temp->setVec3("viewPos", cam.Position);

	glBindVertexArray(VAO);
	glBufferData(GL_ARRAY_BUFFER, p->getVerticesSize() * sizeof(float), &(p->getVertices())[0], GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, p->getVerticesSize() / 8);
}

void Renderer::renderUI(Camera& cam) {
	
	glDepthFunc(GL_LEQUAL);
	skyboxShader->use();
	glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 10000.0f);
	glm::mat4 view = glm::mat4(glm::mat3(cam.GetViewMatrix())); // remove translation from the view matrix
	skyboxShader->setMat4("view", view);
	skyboxShader->setMat4("projection", projection);
	// skybox cube
	glBindVertexArray(UIVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

}

void Renderer::frameBufferInit() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::frameBufferFin() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	frameBufferShader->use();
	glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, textureFBO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

Renderer::~Renderer() {
	delete shader;
	delete shaderSun;
	delete skyboxShader;
	delete bloomBlur;
	delete bloomFinal;
}

void Renderer::init() {
	float quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};


	unsigned int quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	unsigned int UIVBO;
	glGenVertexArrays(1, &UIVAO);
	glGenBuffers(1, &UIVBO);
	glBindVertexArray(UIVAO);
	glBindBuffer(GL_ARRAY_BUFFER, UIVBO);
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), textureLoader.getSkyboxVertices(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	//only position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//texture
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	//framebuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenTextures(1, &textureFBO);
	glBindTexture(GL_TEXTURE_2D, textureFBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureFBO, 0);

	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Framebuffer is not complete" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	std::vector<std::string> faces
	{
		"assets/resources/right.jpg",
		"assets/resources/left.jpg",
		"assets/resources/top.jpg",
		"assets/resources/bottom.jpg",
		"assets/resources/front.jpg",
		"assets/resources/back.jpg",
	};
	cubemapTexture = textureLoader.loadCubemap(faces);

	shader = new Shader("assets/shaders/shader.vs", "assets/shaders/shader.fs");
	shaderSun = new Shader("assets/shaders/shader.vs", "assets/shaders/shaderSun.fs");
	skyboxShader = new Shader("assets/shaders/skyboxShader.vs", "assets/shaders/skyboxShader.fs");
	bloomBlur = new Shader("assets/shaders/bloom.vs", "assets/shaders/bloomBlur.fs");
	bloomFinal = new Shader("assets/shaders/bloom.vs", "assets/shaders/bloomFinal.fs");
	frameBufferShader = new Shader("assets/shaders/shaderFBO.vs", "assets/shaders/shaderFBO.fs");
}