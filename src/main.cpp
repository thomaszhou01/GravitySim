#include <simulator.h>

int main()
{
	Simulator gravitySim{ 800, 800, 4 };
	gravitySim.setupSim();
	gravitySim.runSim();
	gravitySim.endSim();
	return 0;
}