#include <simulator.h>
Camera* Simulator::camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool Simulator::firstMouse = false;
float Simulator::lastX = 0;
float Simulator::lastY = 0;

Simulator::Simulator(int width, int height) : screenW(width), screenH(height) {
	deltaTime = 0.0f;
	lastFrame = 0.0f;
	lastSpawnTime = 0.0f;
	initOpenGL();
	renderer = new Renderer();
	lastX = width / 2.0f;
	height = height / 2.0f;
}

void Simulator::runSim() {
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// renderING
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		processInput(window, currentFrame);
		//std::cout << camera->Front.x << " " << camera->Front.y << " " << camera->Front.z << std::endl;

		updateAndRender();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Simulator::setupSim() {
	/*Planet* temp = new Planet{};
	planets.push_back(temp);*/
}

void Simulator::endSim() {
	glfwTerminate();
}

Simulator::~Simulator() {
	delete renderer;
	delete camera;
}

void Simulator::updateAndRender() {
	//apply new direction to all new objects in simulation
	for (int i = 0; i < planets.size(); i++) {
		if (!planets[i]->getStationary()) planets[i]->applyPhysics(planets);
	}

	//render
	for (int i = 0; i < planets.size(); i++) {
		renderer->render(planets[i], *camera);
	}
}

void Simulator::spawnPlanet() {

}

void Simulator::initOpenGL() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(screenW, screenH, "Gravity Simulation", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Simulator::processInput(GLFWwindow* window, float curFrame) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera->ProcessKeyboard(DOWN, deltaTime);

	if (((curFrame - lastSpawnTime) > 1 || lastSpawnTime == 0.0) && (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)) {
		lastSpawnTime = curFrame;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			Planet* temp = new Planet{ false, camera->Position, camera->Front, 10000 };
			planets.push_back(temp);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			Planet* temp = new Planet{ true, camera->Position + camera->Front*3.0f, camera->Front, 1000000 };
			planets.push_back(temp);
		}
	}
}

void Simulator::frameSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Simulator::mouseCallback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera->ProcessMouseMovement(xoffset, yoffset);
}

void Simulator::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(static_cast<float>(yoffset));
}