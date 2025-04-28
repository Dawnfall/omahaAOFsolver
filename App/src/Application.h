#pragma once

#include <memory>
#include <string>
#include <chrono>

#include "UI/WindowPanel.h"
#include "Solver/Solver.h"
#include "Event.h"

class Solution;
class Node;

class Application
{
public:
	Application()
	{
		m_instance = this;
	}

	WindowPanel window;

	std::shared_ptr<Solution> m_solution = nullptr;
	std::vector<WeightedHand>* m_currentRange = nullptr;

	Event<Solution*> OnSolutionChange;
	Event<const std::vector<WeightedHand>*> OnRangeChange;
	Event<> OnSolveStart;
	Event<Solution*> OnSolveFinish;

	int Run();
	void SaveSolution();
	void LoadSolution(const std::string& filePath);
	void RunSolver();
	void SetSolution(std::shared_ptr<Solution> solution);
	void SetRange(const std::string& action);

	bool GetSolverParams(SolverParams& solParams);

	static Application* GetInstance() { return m_instance; }
	static inline Application* m_instance = nullptr;

	std::chrono::duration<double> duration;
private:
	bool m_isRunning = false;
	std::chrono::steady_clock::time_point m_solverStart;
};