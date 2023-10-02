#include <simulator.h>

int main()
{
	Simulator gravitySim{ 1600, 1600 };
	gravitySim.setupSim();
	gravitySim.runSim();
	gravitySim.endSim();
	return 0;
}