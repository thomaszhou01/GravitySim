#pragma once
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <renderedObject.h>

//contains mesh data, norms, location in space, speed, direction
class Planet : public RenderedObject{
public:
	Planet(bool stationary = true, glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 dir = glm::vec3(0, 0, 0), int mass = 100, int radius = 1);
	void applyPhysics(std::vector<Planet*>& otherPlanets, std::vector<Planet*>& otherSuns);
	void test(int i);
	bool getStationary();
	int getMass();
	~Planet();
private:
	int mass;
	bool stationary;
	void calcPhysics(std::vector<Planet*>& other);
	std::vector<float> generateSphere(int latLines, int longLines, float radius);
};