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
	Planet(bool stationary = true, glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 dir = glm::vec3(0, 0, 0), int mass = 100, int radius = 1);
	float* getVertices();
	int getVerticesSize();
	glm::vec3 getPosition();
	void applyPhysics(std::vector<Planet*>& otherPlanets, std::vector<Planet*>& otherSuns);
	bool getStationary();
	int getMass();
	~Planet();
private:
	int mass;
	bool stationary;
	std::vector<float> mesh;
	std::vector<float> meshIndices;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 direction;
	float* vertices;
	int verticesSize;
	void calcPhysics(std::vector<Planet*>& other);
	std::vector<float> generateSphere(int latLines, int longLines, float radius);
};