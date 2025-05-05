#include "Application.h"
#include "Solver/PokerUtils.h"

int main()
{
	PokerUtils::Init();

	Application app;
	return app.Run();
}

