#include "WindowPanel.h"
#include "RenderUtils.h"

#include "Constants.h"
#include "Application.h"
#include "PokerUtils.h"

int WindowPanel::Render(Application* app)
{
	Fl_Window* window = new Fl_Window(Constants::windowWidth, Constants::windowHeight, Constants::window_title.c_str());

	Fl_Tabs* tabs = new Fl_Tabs(0, 0, Constants::windowWidth, Constants::windowHeight);
	tabs->box(FL_BORDER_BOX);
	solverTab = std::make_unique<SolverTabUI>();
	viewerTab = std::make_unique<ViewerTabUI>();
	optionsTab = std::make_unique<OptionsTabUI>();
	tabs->end();

	window->end();
	window->show();

	return 0;
}





