#pragma once

#include "Core.h"
#include "UI/Widgets/ButtonUI.h"
#include "UI/Widgets/InputUI.h"
#include "UI/Widgets/NodeSelectorUI.h"
#include "UI/Widgets/TrainerUI.h"
#include "UI/Widgets/ScrollViewUI.h"
#include "UI/Special/HandEvItemUI.h"

class ViewerTabUI
{
public:
	ViewerTabUI();

	std::array<URef<LabelUI>, 3> boardCards;
	URef<ButtonUI> loadSolutionButton;
	URef<ScrollViewUI> scrollView;
	URef<LabelUI> m_nodesTitleLabel;
	URef<NodeSelectorUI> nodeSelector;
	URef<TrainerUI> trainer;
	URef<LabelUI> rangeTotalLabel;
	URef<InputUI> filterInput;

	void RefreshRange()const;
};