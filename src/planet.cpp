#include <planet.h>

Planet::Planet(bool stationary, glm::vec3 pos, glm::vec3 dir, int mass, int radius) {
	std::vector<float> verticesTemp = generateSphere(20, 20, radius);
	vertices = new float[verticesTemp.size()];
	verticesSize = verticesTemp.size();

	for (int i = 0; i < verticesTemp.size(); i++) {
		vertices[i] = verticesTemp[i];
	}

	this->position = pos;
	this->direction = glm::normalize(dir) * 5.0f;
	this->stationary = stationary;
	this->mass = mass;
}

float* Planet::getVertices() {
	return vertices;
}

int Planet::getVerticesSize() {
	return verticesSize;
}

glm::vec3 Planet::getPosition() {
	return position;
}

bool Planet::getStationary() {
	return stationary;
}

int Planet::getMass() {
	return mass;
}

void Planet::applyPhysics(std::vector<Planet*>& otherPlanets, std::vector<Planet*>& otherSuns) {
	calcPhysics(otherPlanets);
	calcPhysics(otherSuns);
	position += direction * 0.2f;
}

Planet::~Planet() {
	delete[] vertices;
}

void Planet::calcPhysics(std::vector<Planet*>& other) {
	float gravitationalConstant = 6.67430e-10;
	for (int i = 0; i < other.size(); i++) {
		if (other[i] != this) {
			glm::vec3 dist = other[i]->getPosition() - position;
			glm::vec3 attractionDir = glm::normalize(dist);
			float magnitude = (gravitationalConstant * other[i]->getMass() * mass) / (glm::length(dist) * glm::length(dist));
			attractionDir *= magnitude;
			direction += attractionDir;
		}
	}
}

std::vector<float> Planet::generateSphere(int latLines, int longLines, float radius) {
	int totalVertices = latLines * (longLines + 1) + 2;
	glm::vec3* vecPositions = new glm::vec3[totalVertices];
	glm::vec3* vecNormals = new glm::vec3[totalVertices];
	glm::vec2* textureCoords = new glm::vec2[totalVertices];

	vecPositions[0] = glm::vec3(0, radius, 0);
	vecNormals[0] = glm::vec3(0, 1, 0);
	textureCoords[0] = glm::vec2(0, 1);

	vecPositions[totalVertices - 1] = glm::vec3(0, -radius, 0);
	vecNormals[totalVertices - 1] = glm::vec3(0, -1, 0);
	textureCoords[totalVertices - 1] = glm::vec2(0, 0);

	float latSpacing = 1.0 / (latLines + 1);
	float longSpacing = 1.0 / (longLines);

	int i = 1;
	for (int lati = 0; lati < latLines; lati++) {
		for (int longi = 0; longi <= longLines; longi++) {
			textureCoords[i] = glm::vec2(longi * longSpacing, 1.0f - ((lati + 1) * latSpacing));
			float theta = (textureCoords[i].x * 2.0f * glm::pi<float>());
			float phi = ((textureCoords[i].y - 0.5f) * glm::pi<float>());
			float c = cos(phi);
			vecPositions[i] = glm::vec3((c * cos(theta)), sin(phi), (c * sin(theta))) * radius;
			vecNormals[i] = glm::normalize(vecPositions[i]);
			textureCoords[i] = glm::vec2(0, 0);
			i++;
		}
	}

	int numTriangles = latLines * longLines * 2;
	std::vector<float> meshData;


	for (int j = 0; j < longLines; j++) {
		for (int k = 0; k < 3; k++) {
			int temp = j + k;
			if (k == 0) {
				temp = 0;
			}
			meshData.push_back(vecPositions[temp].x);
			meshData.push_back(vecPositions[temp].y);
			meshData.push_back(vecPositions[temp].z);
			meshData.push_back(vecNormals[temp].x);
			meshData.push_back(vecNormals[temp].y);
			meshData.push_back(vecNormals[temp].z);
			meshData.push_back(textureCoords[temp].x);
			meshData.push_back(textureCoords[temp].y);
		}
	}
	int rowLength = longLines + 1;
	for (int lati = 0; lati < latLines - 1; lati++) {

		int rowStart = (lati * rowLength) + 1;

		for (int longi = 0; longi < longLines; longi++) {
			int firstCorner = rowStart + longi;
			for (int k = 0; k < 6; k++) {
				int temp = firstCorner;
				if (k == 1 || k == 5) {
					temp += rowLength + 1;
				}
				else if (k == 2) {
					temp += rowLength;
				}
				else if (k == 4) {
					temp += 1;
				}
				meshData.push_back(vecPositions[temp].x);
				meshData.push_back(vecPositions[temp].y);
				meshData.push_back(vecPositions[temp].z);
				meshData.push_back(vecNormals[temp].x);
				meshData.push_back(vecNormals[temp].y);
				meshData.push_back(vecNormals[temp].z);
				meshData.push_back(textureCoords[temp].x);
				meshData.push_back(textureCoords[temp].y);
			}
		}
	}

	int pole = totalVertices - 1;
	int bottomRow = ((latLines - 1) * rowLength) + 1;

	for (int j = 0; j < longLines; j++)
	{
		for (int k = 0; k < 3; k++) {
			int temp = bottomRow + j;
			if (k == 0) {
				temp = pole;
			}
			else if(k == 2) {
				temp += 1;
			}
			meshData.push_back(vecPositions[temp].x);
			meshData.push_back(vecPositions[temp].y);
			meshData.push_back(vecPositions[temp].z);
			meshData.push_back(vecNormals[temp].x);
			meshData.push_back(vecNormals[temp].y);
			meshData.push_back(vecNormals[temp].z);
			meshData.push_back(textureCoords[temp].x);
			meshData.push_back(textureCoords[temp].y);
		}
	}

	return meshData;
}