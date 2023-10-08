#pragma once
#include <iostream>
#include <thread>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <camera.h>
#include <planet.h>
#include <renderer.h>
#include <threadPool/threadPool.h>

//simulator itself, handles inputs, physics of objects
class Simulator {
public:
	Simulator(int width, int height, int threads);
	void setupSim();
	void runSim();
	void endSim();
	~Simulator();
private:
	int screenW;
	int screenH;
	unsigned int fpsCounter;
	unsigned int totalObjects;
	bool paused;
	static bool firstMouse;
	static float lastX;
	static float lastY;
	float deltaTime;
	float lastFrame;
	float lastSpawnTime;
	float fpsTimeDiff;
	float fpsPrevTime;
	std::vector<Planet*> planets;
	std::vector<Planet*> suns;
	GLFWwindow* window;
	static Camera* camera;
	Renderer* renderer;
	ThreadPool threadPool;
	void updateAndRender();
	void initOpenGL();
	void spawnPlanet();
	void processInput(GLFWwindow* window, float curFrame);
	static void frameSizeCallback(GLFWwindow* window, int width, int height);
	static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};