#include <renderedObject.h>

RenderedObject::RenderedObject() {

}

float* RenderedObject::getVertices() {
	return vertices;
}

int RenderedObject::getVerticesSize() {
	return verticesSize;
}

glm::vec3 RenderedObject::getPosition() {
	return position;
}

RenderedObject::~RenderedObject() {
	delete[] vertices;
}