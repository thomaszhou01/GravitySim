#pragma once
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RenderedObject {
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 direction;
	float* vertices;
	int verticesSize;

	RenderedObject();
	float* getVertices();
	int getVerticesSize();
	glm::vec3 getPosition();
	~RenderedObject();
};