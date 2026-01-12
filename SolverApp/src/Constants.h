#pragma once
#include <string>
#include <unordered_set>
#include <filesystem>


namespace Constants
{
	//************
	// Images
	const std::unordered_set<char> rankSet = { '2','3','4','5','6','7','8','9','T','J','Q','K','A' };
	const std::unordered_set<char> suitSet = { 's','c','d','h' };

	const int el_height = 20;
	const int input_height = 25;

	//************
	// viewer board
	const int boardCardX = 30;
	const int boardCardY = 44;
	const int loadButtonX = 60;
	const int loadButtonY = 25;

	//************
	// Scroll
	const int scrollitemHeight = 28;
	const int scollItemEvLen = 80;
	const int scrollCardLen = 20;
	const int scrollViewX = 300;
	const int scrollViewY = 600;
	const int rangeTotalLabelY = 30;

	//************
	// Window

	const std::string window_title = "AOF Solver";
	const int windowWidth = 1200;
	const int windowHeight = 800;
	const int tabsHeight = 25;
	//************
	// Widget names

	const std::string board_label = "Board:";
	const std::string players_label = "Players";
	const std::string saveDir_label = "Save dir:";
	const std::string saveButton_label = "Save";
	const std::string sb_label = "sb";
	const std::string bb_label = "bb";
	const std::string margin_label = "Margin";
	const std::string player2_label = "2";
	const std::string player3_label = "3";
	const std::string player4_label = "4";
	const std::string solveButton_label = "Solve";
	const std::string chunkSize_label = "Chunk:";
	const std::string numThreads_label = "Threads:";
	const std::string iterCount_label = "Repeats:";
	const std::string perHandIter_label = "Per Hand:";

	//**************
	//default values
	const std::string board_default = "7s7d7h";
	const std::string sb_default = "0.5";
	const std::string bb_default = "1.0";
	const std::string margin_default = "0.1";
	const std::string saveFolder_default = "";// "F:/Programiranje/Test Data/Solver";
	const std::string repeatIter_default = "1";
	const std::string threadCount_default = "16";
	const std::string chunkSize_default = "250";
	const std::string perHandIter_default = "1";
	const bool saveAutomatic_default = true;

}