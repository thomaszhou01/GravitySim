#include <vector>
#include <planet.h>

class OctTree {
public:
	OctTree(int maxW, int maxH);
	void addNode(Planet* planet);
private:
	int maxWidth;
	int maxHeight;
};