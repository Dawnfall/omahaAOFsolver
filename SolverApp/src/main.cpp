#include "Application.h"
#include "Solver.h"

int main()
{
	SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);

	Solver::Init();

	Application app;
	int result = app.Run();

	SetThreadExecutionState(ES_CONTINUOUS);
	return result;
}

