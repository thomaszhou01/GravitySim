#include <renderer.h>
Renderer::Renderer() {
	this->init();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::render(Planet* p, std::vector<Planet*> suns, Camera& cam) {
	Shader* temp;
	if (!p->getStationary()) {
		temp = shader;
		temp->use();
		temp->setInt("numLights", suns.size());
		temp->setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		temp->setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		temp->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		temp->setFloat("material.shininess", 64.0f);
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
	glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)800 / (float)800, 0.1f, 10000.0f);
	glm::mat4 view = cam.GetViewMatrix();
	temp->setMat4("projection", projection);
	temp->setMat4("view", view);
	temp->setVec3("viewPos", cam.Position);





	glBindVertexArray(VAO);
	glBufferData(GL_ARRAY_BUFFER, p->getVerticesSize() * sizeof(float), &(p->getVertices())[0], GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, p->getVerticesSize() / 8);
}

Renderer::~Renderer() {
	delete shader;
	delete shaderSun;
}

void Renderer::init() {
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBindVertexArray(VAO);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	shader = new Shader("assets/shader.vs", "assets/shader.fs");
	shaderSun = new Shader("assets/shader.vs", "assets/shaderSun.fs");
}