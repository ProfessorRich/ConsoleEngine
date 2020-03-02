#pragma once

#include "ConsoleEngine.h"
#include "GlobalConstants.h"
#include "RandomSeed.h"
#include <chrono>	// - for efficiency testing

class GameProcess {
private:
	int m_status;
	int m_lastKey;
	tile** m_currentWorld;
	COORD m_playerPos;
	ConsoleEngine* m_console;
	RandomSeed m_currentSeed;

	int* m_worldGenStats; // mmm tasty


	std::chrono::time_point<std::chrono::steady_clock> m_testStartTime;
	std::chrono::time_point<std::chrono::steady_clock> m_testEndTime;
public:
	GameProcess(ConsoleEngine*);

	int GetStatus();
	void CheckKey();
	void ProcessMovement();
	void CentralProcess();
	void DrawPlayer();
	void MenuProcess();
	void ConsoleOutputTesting();
	void RandomSeedTesting();
	void MapGenProcess();
	void GameRunProcess();
	void PrintWorld();
	void PrintWorldMono();
	void PrintWorldEfficient();
	void PrintWorldWhack(int);
	void WaitForKey();
	bool WaitForKeyRetry();
	void StartEfficiencyTest();
	void EndEfficiencyTest(); 
};