#include "Application.h"

#include "UI/WindowPanel.h"
#include "RenderUtils.h"

#include "Core.h"
#include "Solver/Solver.h"
#include "Data/Solution.h"
#include "Solver/Node.h"

#include "Data/SolverParams.h"

int Application::Run()
{
	window.Render(this);
	SetSolution(nullptr);
	return Fl::run();
}

void Application::RunSolver()
{
	if (m_isRunning)
		return;

	m_isRunning = true;
	SolverParams solverParams;
	GetSolverParams(solverParams);

	m_solverStart = std::chrono::high_resolution_clock::now();
	OnSolveStart.Trigger();
	Solver::SolveAsync(solverParams, [this](const SolverParams& solverParams, std::shared_ptr<Solution> solution) {
		
		Solution::Serialize(std::string(solverParams.folderPath), *solution);

		duration = std::chrono::high_resolution_clock::now() - m_solverStart;
		m_isRunning = false;
		OnSolveFinish.Trigger(solution.get());
		});
}


void Application::SaveSolution()
{
	if (m_solution == nullptr)
		return;

	//Solution::Serialize(window.GetSaveFolder(), *m_solution);
}

void Application::LoadSolution(const std::string& filePath)
{
	auto sol = Solution::Deserialize(filePath);
	if (!sol)
	{
		RenderUtils::CreateAlert("Solution load failed!");
		return;
	}

	SetSolution(sol);
}

void Application::SetSolution(std::shared_ptr<Solution> solution)
{
	if (solution == m_solution)
		return;
	m_solution = solution;

	if (m_solution)
	{
		OnSolutionChange.Trigger(m_solution.get());
		SetRange("");
	}
	else
		OnSolutionChange.Trigger(nullptr);
}

void Application::SetRange(const std::string& action)
{
	if (m_solution == nullptr)
		return;

	m_currentRange = m_solution->GetRange(action);
	OnRangeChange.Trigger(m_currentRange);
}

bool Application::GetSolverParams(SolverParams& solParams)
{
	if (!window.GetBlinds(solParams.sb, solParams.bb))
	{
		RenderUtils::CreateAlert("Invalid blinds");
		return false;
	}
	if (!window.GetMargin(solParams.margin))
	{
		RenderUtils::CreateAlert("Invalid margin");
		return false;
	}
	if (!window.GetBoard(solParams.flop))
	{
		RenderUtils::CreateAlert("Invalid board");
		return false;
	}
	solParams.totalPlayers = window.GetPlayerCount();

	if (!window.GetThreads(solParams.threadCount))
	{
		RenderUtils::CreateAlert("Invalid threads");
		return false;
	}
	if (!window.GetChunkSize(solParams.chunkSize))
	{
		RenderUtils::CreateAlert("Invalid Chunk Size");
		return false;
	}
	if (!window.GetPerHandIter(solParams.perHandIterCount))
	{
		RenderUtils::CreateAlert("Invalid per hand iters");
		return false;
	}
	if (!window.GetRepeatIters(solParams.repeatIterCount))
	{
		RenderUtils::CreateAlert("Invalid repeat iterations");
		return false;
	}
	std::string folderPath;
	if (!window.GetSaveFolder(folderPath))
	{
		RenderUtils::CreateAlert("Invalid save folder");
		return false;
	}
	solParams.folderPath = folderPath.c_str();
	return true;
}