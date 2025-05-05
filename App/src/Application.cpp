#include "Application.h"

#include "UI/WindowPanel.h"
#include "RenderUtils.h"

#include "Core.h"
#include "Solver/Solver.h"
#include "Data/SolverParams.h"
#include "Data/SolverResult.h"
#include "Data/Solution.h"

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

	OnSolveStart.Trigger();
	Solver::SolveAsync(solverParams, [this](const SolverParams& solverParams, SolverResult result) {
		m_solverResult = std::make_unique<SolverResult>(result);
		if (m_solverResult->Solution)
		{
			Solution::Serialize(std::string(solverParams.folder), *m_solverResult->Solution);
		}

		m_isRunning = false;
		OnSolveFinish.Trigger();
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
		OnSolutionChange.Trigger();
		SetNode(0);
	}
	else
		OnSolutionChange.Trigger();
}

void Application::SetNode(int nodeIndex)
{
	if (m_solution == nullptr || currentNode == nodeIndex)
		return;

	currentNode = nodeIndex;
	OnRangeChange.Trigger();
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
	if (!window.GetSaveFolder(solParams.folder,sizeof(solParams.folder)))
	{
		RenderUtils::CreateAlert("Invalid save folder");
		return false;
	}
	return true;
}