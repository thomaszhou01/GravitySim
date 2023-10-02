#pragma once
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//contains mesh data, norms, location in space, speed, direction
class Planet {
public:
	Planet();
	float* getVertices();
	int getVerticesSize();
	std::vector<float> mesh;
	std::vector<float> meshIndices;
private:
	float* vertices;
	int verticesSize;
	std::vector<float> generateSphere(int latLines, int longLines, float radius);
};