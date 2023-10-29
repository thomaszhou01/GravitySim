#include <renderer.h>
Renderer::Renderer(int screenWidth, int screenHeight) : screenWidth{ screenWidth }, screenHeight{ screenHeight } {
	this->init();
}

void Renderer::renderSuns(Planet* p, Camera& cam) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, p->getPosition());
	glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 10000.0f);
	glm::mat4 view = cam.GetViewMatrix();

	shaderSun->use();
	shaderSun->setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
	shaderSun->setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
	shaderSun->setVec3("material.specular", 1.0f, 1.0f, 1.0f);
	shaderSun->setFloat("material.shininess", 64.0f);
	shaderSun->setMat4("model", model);
	shaderSun->setMat4("projection", projection);
	shaderSun->setMat4("view", view);
	shaderSun->setVec3("viewPos", cam.Position);

	glBindVertexArray(VAO);
	glBufferData(GL_ARRAY_BUFFER, p->getVerticesSize() * sizeof(float), &(p->getVertices())[0], GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, p->getVerticesSize() / 8);
}

void Renderer::renderPlanetsDeferred(Planet* p, Camera& cam) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, p->getPosition());
	glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 10000.0f);
	glm::mat4 view = cam.GetViewMatrix();
	
	float speed = glm::length(p->direction);
	glm::vec3 colour = glm::vec3(0.1) * speed;

	deferredGeo->use();
	deferredGeo->setVec3("colour", colour);
	deferredGeo->setFloat("albedo", 1.0f);
	deferredGeo->setMat4("model", model);
	deferredGeo->setMat4("projection", projection);
	deferredGeo->setMat4("view", view);

	glBindVertexArray(VAO);
	glBufferData(GL_ARRAY_BUFFER, p->getVerticesSize() * sizeof(float), &(p->getVertices())[0], GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, p->getVerticesSize() / 8);
}

void Renderer::renderUI(Camera& cam) {
	glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 10000.0f);
	glm::mat4 view = glm::mat4(glm::mat3(cam.GetViewMatrix())); // remove translation from the view matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.8f, 0.8f, 0.0f));
	model = glm::scale(model, glm::vec3(0.1f));
	model = model * glm::lookAt(glm::vec3(0), glm::vec3(0) + cam.Front, glm::vec3(0.0f, 1.0f, 0.0f));
	
	skyboxShader->use();
	skyboxShader->setMat4("view", view);
	skyboxShader->setMat4("projection", projection);
	// skybox cube
	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(UIVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	orientationBoxShader->use();
	orientationBoxShader->setMat4("view", view);
	orientationBoxShader->setMat4("projection", projection);
	orientationBoxShader->setMat4("model", model);
	glBindVertexArray(UIVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, orientationCubeTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void Renderer::renderLighting(std::vector<Planet*> suns, Camera& cam) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

	deferredLighting->use();
	deferredLighting->setInt("gPosition", 0);
	deferredLighting->setInt("gNormal", 1);
	deferredLighting->setInt("gAlbedoSpec", 2);
	deferredLighting->setInt("numLights", suns.size());

	for (unsigned int i = 0; i < suns.size(); i++)
	{
		const float linear = 0.0014f;
		const float quadratic = 0.000007f;
		deferredLighting->setVec3("lights[" + std::to_string(i) + "].Position", suns[i]->getPosition());
		deferredLighting->setVec3("lights[" + std::to_string(i) + "].Color", glm::vec3(0.5f, 0.5f, 0.5f));
		deferredLighting->setFloat("lights[" + std::to_string(i) + "].Linear", linear);
		deferredLighting->setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
	}
	deferredLighting->setVec3("viewPos", cam.Position);
}

void Renderer::frameBufferInit() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::frameBufferFin() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::frameBufferRender() {
	//render framebuffer image to screen
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	//allow depth information for forward rendered components
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderer::~Renderer() {
	delete shader;
	delete shaderSun;
	delete skyboxShader;
	delete bloomBlur;
	delete bloomFinal;
	delete frameBufferShader;
	delete deferredGeo;
	delete deferredLighting;
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
	//position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	//texture
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

	// position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// color + specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	//depth buffer
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Framebuffer is not complete" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//load assets + shaders
	std::vector<std::string> faces
	{
		"assets/resources/right.jpg",
		"assets/resources/left.jpg",
		"assets/resources/top.jpg",
		"assets/resources/bottom.jpg",
		"assets/resources/front.jpg",
		"assets/resources/back.jpg",
	};

	std::vector<std::string> orientationFaces
	{
		"assets/resources/orientationCubeRight.jpg",
		"assets/resources/orientationCubeLeft.jpg",
		"assets/resources/orientationCubeTop.jpg",
		"assets/resources/orientationCubeBottom.jpg",
		"assets/resources/orientationCubeBack.jpg",
		"assets/resources/orientationCubeFront.jpg",
	};

	cubemapTexture = textureLoader.loadCubemap(faces);
	orientationCubeTexture = textureLoader.loadCubemap(orientationFaces);

	shader = new Shader("assets/shaders/shader.vs", "assets/shaders/shader.fs");
	shaderSun = new Shader("assets/shaders/shader.vs", "assets/shaders/shaderSun.fs");
	skyboxShader = new Shader("assets/shaders/skyboxShader.vs", "assets/shaders/skyboxShader.fs");
	orientationBoxShader = new Shader("assets/shaders/orientationBox.vs", "assets/shaders/skyboxShader.fs");
	bloomBlur = new Shader("assets/shaders/bloom.vs", "assets/shaders/bloomBlur.fs");
	bloomFinal = new Shader("assets/shaders/bloom.vs", "assets/shaders/bloomFinal.fs");
	frameBufferShader = new Shader("assets/shaders/shaderFBO.vs", "assets/shaders/shaderFBO.fs");

	deferredGeo = new Shader("assets/shaders/shader.vs", "assets/shaders/shaderGeometry.fs");
	deferredLighting = new Shader("assets/shaders/shaderDeferredLighting.vs", "assets/shaders/shaderDeferredLighting.fs");
}