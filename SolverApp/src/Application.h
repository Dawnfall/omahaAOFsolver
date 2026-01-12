#pragma once

#include <memory>
#include <string>
#include <chrono>

#include "UI/WindowPanel.h"
#include "Solver.h"
#include "Data/SolverResult.h"
#include "Event.h"
#include "Filter/HandFilter.h"

struct Solution;
class Application
{
public:
	Application()
	{
		m_instance = this;

		m_filter = HandFilter::ParseFilter("");
	}

	WindowPanel window;

	URef<SolverResult> m_solverResult = nullptr;
	SRef<Solution> m_solution = nullptr;
	int currentRange = -1;

	URef<FilterNode> m_filter = nullptr;
	std::vector<Hand> filteredHands;
	std::vector<float> filteredEvs;

	Event<> OnSolutionChange;
	Event<> OnRangeChange;
	Event<> OnFilterChange;
	Event<> OnSolveStart;
	Event<> OnSolveFinish;

	int Run();
	void LoadSolution(const std::string& filePath);
	void RunSolver();
	void SetSolution(std::shared_ptr<Solution> solution);
	void SetRange(int nodeIndex);
	void SetFilter(URef<FilterNode> filter);
	bool GetSolverParams(SolverParams& solParams);

	static Application* GetInstance() { return m_instance; }
	static inline Application* m_instance = nullptr;

private:
	void FilterHands();

	bool m_isRunning = false;
};