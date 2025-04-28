#pragma once

#include "RenderUtils.h"
#include "Constants.h"
#include "PanelUI.h"

#include "Core.h"
#include "Utils.h"

#include "Widgets/LabelUI.h"
#include "Widgets/InputUI.h"
#include "Widgets/ButtonUI.h"
#include "Widgets/RadioButtonUI.h"
#include "Widgets/ScrollViewUI.h"
#include "Widgets/NodeSelectorUI.h"
#include "Widgets/TrainerUI.h"

class Application;
class WindowPanel :public PanelUI
{
public:
	//Solver
	URef<LabelUI> infoLabel;

	URef<LabelUI> boardLabel;
	URef<InputUI> boardInput;

	URef<LabelUI> sbLabel;
	URef<InputUI> sbInput;

	URef<LabelUI> marginLabel;
	URef<InputUI> maginInput;

	URef<LabelUI> bbLabel;
	URef<InputUI> bbInput;

	URef<LabelUI> playersLabel;
	std::array<URef<LabelUI>, 3> totalPlayerLabels;
	std::array<URef<RadioButtonUI>, 3> totalPlayerRadioButtons;

	URef<ButtonUI> solveButton;

	//Options
	URef<LabelUI> numThreadsLabel;
	URef<InputUI> threadInput;

	URef<LabelUI> repeatIterLabel;
	URef<InputUI> repeatIterInput;

	URef<LabelUI> perHandIterLabel;
	URef<InputUI> perHandIterInput;

	URef<LabelUI> chunkSizeLabel;
	URef<InputUI> chunkInput;

	URef<LabelUI> saveDirLabel;
	URef<InputUI> saveInput;

	//Viewer
	std::array<URef<LabelUI>, 3> boardCards;
	URef<ButtonUI> loadSolutionButton;
	URef<ScrollViewUI> scrollView;
	URef<LabelUI> rangeTotalLabel;
	URef<InputUI> filterInput = nullptr;
	URef<LabelUI> m_nodesTitleLabel;
	URef<NodeSelectorUI> nodeSelector;
	URef<TrainerUI> trainer;

	int Render(Application* app)override;
	void RenderSolverPanel(Application* app);
	void RenderOptionsPanel(Application* app);
	void RenderViewerPanel(Application* app);

	// Solver
	int GetPlayerCount()
	{
		if (totalPlayerRadioButtons[0]->IsOn())
			return 2;
		else if (totalPlayerRadioButtons[1]->IsOn())
			return 3;
		return 4;
	}
	bool GetBoard(std::array<uint8_t, 3>& flop)
	{
		std::string board = (boardInput->m_input->value() != nullptr) ? std::string(boardInput->m_input->value()) : std::string();
		if (board.size() != 6)
			return false;
		if (!(
			Constants::rankSet.contains(board[0]) && Constants::rankSet.contains(board[2]) && Constants::rankSet.contains(board[4]) &&
			Constants::suitSet.contains(board[1]) && Constants::suitSet.contains(board[3]) && Constants::suitSet.contains(board[5])
			))
			return false;

		phevaluator::Card c0 = phevaluator::Card(board.substr(0, 2));
		phevaluator::Card c1 = phevaluator::Card(board.substr(2, 2));
		phevaluator::Card c2 = phevaluator::Card(board.substr(4, 2));

		flop[0] = static_cast<uint8_t>((int)c0);
		flop[1] = static_cast<uint8_t>((int)c1);
		flop[2] = static_cast<uint8_t>((int)c2);
		return true;
	}
	bool GetBlinds(float& sb, float& bb)
	{
		return Utils::SafeCharToFloat(sbInput->m_input->value(), sb) && Utils::SafeCharToFloat(bbInput->m_input->value(), bb);
	}

	// Margin
	bool GetMargin(float& margin)
	{
		return Utils::SafeCharToFloat(maginInput->m_input->value(), margin);
	}

	// Options
	bool GetThreads(unsigned int& threadCount)
	{
		return Utils::SafeCharToUint(threadInput->m_input->value(), threadCount);
	}
	bool GetChunkSize(unsigned int& chunkSize)
	{
		return Utils::SafeCharToUint(chunkInput->m_input->value(), chunkSize);
	}
	bool GetRepeatIters(unsigned int& repeatCount)
	{
		return Utils::SafeCharToUint(repeatIterInput->m_input->value(), repeatCount);
	}
	bool GetPerHandIter(unsigned int& perHandIter)
	{
		return Utils::SafeCharToUint(perHandIterInput->m_input->value(), perHandIter);
	}
	bool GetSaveFolder(std::string& folder)
	{
		folder = saveInput->m_input->value();
		return true;
	}

	//callbacks
	static void range_radio_callback(Fl_Widget* widget, void* userData);

	static void solve_button_callback(Fl_Widget* widget, void* userData);

	static void load_button_callback(Fl_Widget*, void* userData);
};