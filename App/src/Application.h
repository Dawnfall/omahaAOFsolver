#pragma once

#include <memory>
#include <string>
#include <chrono>

#include "UI/WindowPanel.h"
#include "Solver/Solver.h"
#include "Data/SolverResult.h"
#include "Event.h"

struct Solution;
class Application
{
public:
	Application()
	{
		m_instance = this;
	}

	WindowPanel window;

	std::unique_ptr<SolverResult> m_solverResult = nullptr;
	std::shared_ptr<Solution> m_solution = nullptr;
	int currentNode = -1;

	Event<> OnSolutionChange;
	Event<> OnRangeChange;
	Event<> OnSolveStart;
	Event<> OnSolveFinish;

	int Run();
	void SaveSolution();
	void LoadSolution(const std::string& filePath);
	void RunSolver();
	void SetSolution(std::shared_ptr<Solution> solution);
	void SetNode(int nodeIndex);

	bool GetSolverParams(SolverParams& solParams);

	static Application* GetInstance() { return m_instance; }
	static inline Application* m_instance = nullptr;

private:
	bool m_isRunning = false;
};