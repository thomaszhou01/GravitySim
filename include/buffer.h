#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


//contains buffer code for 
class Buffer {
public:
	Buffer();
	void addVertexData(float* vertices);
private:
	unsigned int VAO;

};