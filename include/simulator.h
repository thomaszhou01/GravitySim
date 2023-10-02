#pragma once
#include <iostream>

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <camera.h>
#include <buffer.h>
#include <planet.h>
#include <renderer.h>

//simulator itself, handles inputs, physics of objects
class Simulator {
public:
	Simulator(int width, int height);
	void setupSim();
	void runSim();
	void endSim();

private:
	int screenW;
	int screenH;
	float deltaTime;
	float lastFrame;
	std::vector<Planet> planets;
	GLFWwindow* window;
	Camera* camera;
	Renderer* renderer;
	void update();
	void initOpenGL();
	void processInput(GLFWwindow* window);
	static void frameSizeCallback(GLFWwindow* window, int width, int height);
};