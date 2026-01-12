#include "OptionsTabUI.h"
#include "UI/RenderCursor.h"
#include "Constants.h"
#include "Utils.h"

#include "FL/Fl_Group.H"
#include "UI/Widgets/LabelUI.h"
#include "UI/Widgets/InputUI.h"

OptionsTabUI::OptionsTabUI()
{
	RenderCursor cursor;
	cursor.spacingX = 10;
	cursor.spacingY = 10;
	cursor.SetStartPos(0, Constants::tabsHeight);
	cursor.Reset();
	cursor.NextRow();

	Fl_Group* optionsTab = new Fl_Group(0, 25, Constants::windowWidth, Constants::windowHeight - 20, "Options");
	cursor.SetStartPos(0, 25);
	cursor.Reset();
	optionsTab->hide();

	//***********
	// Save

	saveDirLabel = std::make_unique <LabelUI>(cursor, 50, Constants::el_height, Constants::saveDir_label);
	saveInput = std::make_unique<InputUI>(cursor, 150, Constants::el_height, Utils::GetExecutableDir().string());
	cursor.NextRow();

	//***********
	// Threads and chunks

	numThreadsLabel = std::make_unique <LabelUI>(cursor, 50, Constants::el_height, Constants::numThreads_label);
	threadInput = std::make_unique<InputUI>(cursor, 40, Constants::el_height, Constants::threadCount_default);

	chunkSizeLabel = std::make_unique <LabelUI>(cursor, 50, Constants::el_height, Constants::chunkSize_label);
	chunkInput = std::make_unique<InputUI>(cursor, 40, Constants::el_height, Constants::chunkSize_default);
	cursor.NextRow();

	optionsTab->end();

}
