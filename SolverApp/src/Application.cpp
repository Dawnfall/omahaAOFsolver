#include "Application.h"

#include "UI/WindowPanel.h"
#include "RenderUtils.h"

#include "Core.h"
#include "Solver.h"
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
	currentRange = 0;
	m_filter = HandFilter::ParseFilter("");
	FilterHands();

	OnSolutionChange.Trigger();
}

void Application::SetRange(int rangeIndex)
{
	if (m_solution == nullptr || currentRange == rangeIndex)
		return;

	currentRange = rangeIndex;
	m_filter = HandFilter::ParseFilter("");
	FilterHands();

	OnRangeChange.Trigger();
}

void Application::SetFilter(URef<FilterNode> filter)
{
	if (filter == m_filter || !filter)
		return;

	m_filter = std::move(filter);
	FilterHands();
	OnFilterChange.Trigger();
}

bool Application::GetSolverParams(SolverParams& solParams)
{
	if (!window.solverTab->GetBlinds(solParams.sb, solParams.bb))
	{
		RenderUtils::CreateAlert("Invalid blinds");
		return false;
	}
	if (!window.solverTab->GetMargin(solParams.margin))
	{
		RenderUtils::CreateAlert("Invalid margin");
		return false;
	}
	if (!window.solverTab->GetBoard(solParams.flop))
	{
		RenderUtils::CreateAlert("Invalid board");
		return false;
	}
	solParams.totalPlayers = window.solverTab->GetPlayerCount();

	if (!window.optionsTab->GetThreads(solParams.threadCount))
	{
		RenderUtils::CreateAlert("Invalid threads");
		return false;
	}
	if (!window.optionsTab->GetChunkSize(solParams.chunkSize))
	{
		RenderUtils::CreateAlert("Invalid Chunk Size");
		return false;
	}
	if (!window.optionsTab->GetSaveFolder(solParams.folder, sizeof(solParams.folder)))
	{
		RenderUtils::CreateAlert("Invalid save folder");
		return false;
	}
	return true;
}

void Application::FilterHands()
{
	filteredHands.clear();
	filteredEvs.clear();

	if (m_solution && currentRange >= 0)
	{
		for (int i = 0; i < m_solution->Hands.size() / 4; i++)
		{
			const auto& [hand, ev] = m_solution->GetHandAndEv(currentRange, i);
			if (m_filter->Validate(hand))
			{
				filteredHands.emplace_back(hand);
				filteredEvs.emplace_back(ev);
			}
		}
	}
}
