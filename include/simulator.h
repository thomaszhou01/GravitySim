#pragma once
#include <iostream>

#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <camera.h>
#include <planet.h>
#include <renderer.h>

//simulator itself, handles inputs, physics of objects
class Simulator {
public:
	Simulator(int width, int height);
	void setupSim();
	void runSim();
	void endSim();
	~Simulator();
private:
	int screenW;
	int screenH;
	bool paused;
	static bool firstMouse;
	static float lastX;
	static float lastY;
	float deltaTime;
	float lastFrame;
	float lastSpawnTime;
	std::vector<Planet*> planets;
	std::vector<Planet*> suns;
	GLFWwindow* window;
	static Camera* camera;
	Renderer* renderer;
	void updateAndRender();
	void initOpenGL();
	void spawnPlanet();
	void processInput(GLFWwindow* window, float curFrame);
	static void frameSizeCallback(GLFWwindow* window, int width, int height);
	static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};