#pragma once

#include "RenderUtils.h"
#include "Constants.h"
#include "PanelUI.h"

#include "Core.h"
#include "Utils.h"
#include "Data/SolverResult.h"

#include "Widgets/LabelUI.h"
#include "Widgets/InputUI.h"
#include "Widgets/ButtonUI.h"
#include "Widgets/RadioButtonUI.h"
#include "Widgets/ScrollViewUI.h"
#include "Widgets/NodeSelectorUI.h"
#include "Widgets/TrainerUI.h"

#include "Tabs/OptionsTabUI.h"
#include "Tabs/SolverTabUI.h"
#include "Tabs/ViewerTabUI.h"

class Application;
class WindowPanel :public PanelUI
{
public:
	URef<OptionsTabUI> optionsTab;
	URef<SolverTabUI> solverTab;
	URef<ViewerTabUI> viewerTab;

	int Render(Application* app)override;
};