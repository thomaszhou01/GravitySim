#include <renderer.h>
Renderer::Renderer() {
	this->init();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::render(Planet &p, Camera& cam) {
	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	shader->setMat4("model", model);
	glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom), (float)800 / (float)800, 0.1f, 100.0f);
	glm::mat4 view = cam.GetViewMatrix();
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);

	glBindVertexArray(VAO);
	glBufferData(GL_ARRAY_BUFFER, p.getVerticesSize() * sizeof(float), &(p.getVertices())[0], GL_STATIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, p.getVerticesSize()/8);
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
}