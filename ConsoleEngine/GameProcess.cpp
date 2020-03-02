#include <windows.h>
#include "GameProcess.h"
#include "WorldGen.h"
// #include <stdio.h> // DEPRECIATED.... transfer all calls to std:cin/cout in iostream
#include <iostream> // - Used for debugging my crappy WriteString/WriteChar functions... and for printing numbers since I have no idea how to do that without it. 


GameProcess::GameProcess(ConsoleEngine* console) {
	m_console = console;
	m_playerPos = { 0,0 };
	m_lastKey = 0;
	m_status = 1;

	m_worldGenStats = new int[10];

	WorldGen newWorld;
	m_currentWorld = newWorld.GetWorld();			// mild concern... this newWorld does not have 'console' pointer - but could it ever need it? And why did I even make that decision?
}

void GameProcess::WaitForKey() {
	EndEfficiencyTest();
	m_console->WriteString("\n\nPress any key to continue...");					// Should standardise the position this prints... like the bottom of the screen below the 'drawing area'.
	CheckKey();
	while (m_lastKey == 0) {
		CheckKey();
	}
}

bool GameProcess::WaitForKeyRetry() {
	EndEfficiencyTest();
	m_console->WriteString("\n\nPress any key to continue... OR 1 to retry");			
	CheckKey();
	while (m_lastKey == 0) {
		CheckKey();
	}
	if (m_lastKey == 0x31) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}


int GameProcess::GetStatus() {
	return m_status;
}

void GameProcess::CheckKey(void) {                 // Trying to stop it taking one key input twice... it is retarded and doesn't work anyway.
	m_lastKey = m_console->GetKey();
}

void GameProcess::ProcessMovement() {
	COORD playerMove = { 0,0 };

	switch (m_lastKey) {
	case VK_UP:
		playerMove.Y = -1;
		break;
	case VK_DOWN:
		playerMove.Y = 1;
		break;
	case VK_LEFT:
		playerMove.X = -1;
		break;
	case VK_RIGHT:
		playerMove.X = 1;
		break;
	}

	if ((playerMove.Y != 0) || (playerMove.X != 0)) {
		m_playerPos.Y += playerMove.Y;
		m_playerPos.X += playerMove.X;
	}

	if (m_playerPos.Y < 0) {
		m_playerPos.Y = 0;
	}
	if (m_playerPos.X < 0) {
		m_playerPos.X = 0;
	}
	if (m_playerPos.Y >= g_consoleHeight-1) {
		m_playerPos.Y = g_consoleHeight-1;
	}
	if (m_playerPos.X >= g_consoleWidth-1) {
		m_playerPos.X = g_consoleWidth-1;
	}
}

void GameProcess::DrawPlayer() {
	m_console->SetCursorPosition(m_playerPos);
	m_console->SetTextColour(0);
	m_console->SetBackgroundColour(ColourWhite);
	m_console->WriteString("P");
}

void GameProcess::CentralProcess(void) {
	switch (m_status) {
	case 1:
		MenuProcess();
		break;
	case 2:
		MapGenProcess();
		break;
	case 3:
		GameRunProcess();
		break;
	case 4:
		RandomSeedTesting();
		break;
	case 5:
		ConsoleOutputTesting();
		break;
	}
}

void GameProcess::ConsoleOutputTesting() {
	m_console->Clear();
	m_console->ResetCursorPosition();
	m_console->WriteString("Welcome to console output testing\n");
	m_console->WriteString("This uses the phenomenally well-programmed ConsoleEngine (tm).\n");
	m_console->WriteString("1. Print a rainbow of shit.\n");
	m_console->WriteString("2. Set colour manually.\n");
	m_console->WriteString("3. Reset colours.\n");
	m_console->WriteString("4. Type some shit to print - for whatever reason you might want to do that.\n");
	m_console->WriteString("ESC = quit out\n\n\n");
	while (m_status == 5) {
		CheckKey();
		if (m_lastKey == 0x31) {
			StartEfficiencyTest();

			for (int i = 0; i < 256; i++) {
				m_console->SetManualColour(i);
				std::cout << i;
				m_console->ResetColours();
				std::cout << " ";
			}
			std::cout << "\n\n";
			WaitForKey();
			ConsoleOutputTesting();
		}
		if (m_lastKey == 0x32) {
			unsigned int s;
			m_console->WriteString("Input colour code:");
			std::cin >> s;
			m_console->SetManualColour(s);
			ConsoleOutputTesting();
		}
		if (m_lastKey == 0x33) {
			m_console->ResetColours();
			ConsoleOutputTesting();
		}
		if (m_lastKey == 0x34) {
			StartEfficiencyTest();

			char s[255];
			m_console->WriteString("Input string:");
			std::cin >> s;
			m_console->WriteString("\n\n\n");
			std::cout << s;
			WaitForKey();
			ConsoleOutputTesting();
		}
		if (m_lastKey == VK_ESCAPE) {
			m_status = 1;
			CentralProcess();
		}
	}
}

void GameProcess::MenuProcess() {
	m_console->Clear();
	m_console->ResetCursorPosition();
	m_console->ResetColours();										// incase some dickhead has been pissing about in colour testing and messed up the colours.
	m_console->WriteString("Welcome to the cutting edge of procedural game design.\n");
	m_console->WriteString("Please choose from the following options...\n\n");
	m_console->WriteString("1. Generate a New World (beta)\n");
	m_console->WriteString("2. Play the Game (currently in pre-alpha)\n");
	m_console->WriteString("3. Random seed testing\n");
	m_console->WriteString("4. Console output colour testing\n\n");
	m_console->WriteString("ESC = quit out\n\n\n");
	while (m_status == 1) {
		CheckKey();
		if (m_lastKey == 0x31) {
			m_status = 2;
			CentralProcess();
		}
		if (m_lastKey == 0x32) {
			m_status = 3;
			CentralProcess();
		}
		if (m_lastKey == 0x33) {
			m_status = 4;
			CentralProcess();
		}
		if (m_lastKey == 0x34) {
			m_status = 5;
			CentralProcess();
		}
		if (m_lastKey == VK_ESCAPE) {
			exit(0);
		}
	}
}

void GameProcess::RandomSeedTesting() {														// Under construction for testing and debugging RandomSeed(tm)
	m_console->Clear();
	m_console->ResetCursorPosition();

	m_console->WriteString("Welcome to random seed testing.\n");
	m_console->WriteString("This is cutting edge randomisation technology. Proceed with caution.\n");
	m_console->WriteString("1. Print all rolls\n");
	m_console->WriteString("2. Generate a new seed using system clock\n");
	m_console->WriteString("3. Enter a seed\n");
	m_console->WriteString("4. Show next roll\n");
	m_console->WriteString("ESC = quit out\n\n\n");
	m_console->WriteString("Current seed: ");
	std::cout << m_currentSeed.GetSeed();
	m_console->WriteString("\nCurrent roll number: ");
	std::cout << m_currentSeed.GetRollNumber();


	m_console->WriteString("\n\n\n");

	while (m_status == 4) {
		CheckKey();
		if (m_lastKey == 0x31) {
			StartEfficiencyTest();
			m_currentSeed.PrintAllRolls();
			std::cout << "\n\n";
			WaitForKey();
			RandomSeedTesting();
		}
		if (m_lastKey == 0x32) {
			m_currentSeed.GenSeed();
			RandomSeedTesting();
		}
		if (m_lastKey == 0x33) {
			unsigned int s;
			m_console->WriteString("Input seed:");
			std::cin >> s;
			m_currentSeed.InputSeed(s);
			RandomSeedTesting();
		}
		if (m_lastKey == 0x34) {
			StartEfficiencyTest();
			std::cout << "Dice rolled and got ... " << m_currentSeed.NextRoll();
			std::cout << "\n\n";
			WaitForKey();
			RandomSeedTesting();
		}
		if (m_lastKey == VK_ESCAPE) {
			m_status = 1;
			CentralProcess();
		}
	}
	
}

void GameProcess::MapGenProcess() {
	m_console->Clear();
	m_console->ResetCursorPosition();
	m_console->WriteString("Welcome to the best things you've ever seen in your life.\n");
	m_console->WriteString("1. Generate a new world.\n");
	m_console->WriteString("2. Display the current world.\n");
	m_console->WriteString("3. Display the current world with WHACKED OUT COLOURS!.\n");
	m_console->WriteString("4. Display the current world in MONOCHROME!!!!\n");
	m_console->WriteString("5. Print an efficient world\n");
	m_console->WriteString("6. Wipe world.\n");
	m_console->WriteString("ESC = quit out\n\n\n");
	m_console->WriteString("Current seed: ");
	std::cout << m_currentSeed.GetSeed();
	m_console->WriteString("\nCurrent roll number: ");
	std::cout << m_currentSeed.GetRollNumber();
	if (m_worldGenStats[0] == 123) {
		m_console->WriteString("\n(FIXED) Sea amount: ");
		std::cout << m_worldGenStats[1];
		m_console->WriteString("\n(FIXED) Coast variance: ");
		std::cout << m_worldGenStats[2];
		m_console->WriteString("\n(FIXED) Max forests: ");
		std::cout << m_worldGenStats[3];
		m_console->WriteString("\n(FIXED) Min forests: ");
		std::cout << m_worldGenStats[4];
		m_console->WriteString("\n(FIXED) Woodland border chance: ");
		std::cout << m_worldGenStats[5];
		m_console->WriteString("\n(FIXED) Starting forest spread chance: ");
		std::cout << m_worldGenStats[6];
		m_console->WriteString("\n(FIXED) Forest spread decay per iteration: ");
		std::cout << m_worldGenStats[7];
		m_console->WriteString("\n(GENERATED) Number of forests: ");
		std::cout << m_worldGenStats[8];
	}

	while (m_status == 2) {
		CheckKey();
		if (m_lastKey == 0x31) {

			WorldGen newWorld(m_console, &m_currentSeed);		// newWorld is going to take over m_console and print shit itself to make the world gen 'cinematically cool'.
			do {
				StartEfficiencyTest();
				m_worldGenStats = newWorld.GenerateNewWorld();		// And it's going to return some awesome stats in a meticulously organised array.
				m_currentWorld = newWorld.GetWorld();				// And then pass it to m_currentWorld so we can use it in GameProcess / draw it without the 'cool cinematics'.
			} while (WaitForKeyRetry());
			MapGenProcess();
		}
		if (m_lastKey == 0x32) {
			StartEfficiencyTest();
			PrintWorld();
			MapGenProcess();
		}
		if (m_lastKey == 0x33) {
			int whackFactor;
			printf("\n\nEnter a whack factor: ");
			scanf_s("%d", &whackFactor);
			StartEfficiencyTest();
			PrintWorldWhack(whackFactor);
			MapGenProcess();
		}
		if (m_lastKey == 0x34) {
			StartEfficiencyTest();
			PrintWorldMono();
			MapGenProcess();
		}
		if (m_lastKey == 0x35) {
			StartEfficiencyTest();
			PrintWorldEfficient();
			MapGenProcess();
		}
		if (m_lastKey == 0x36) {	
			StartEfficiencyTest();
			WorldGen newWorld(m_console, &m_currentSeed);			// This seems a totally spasticated way of doing this
			newWorld.WipeWorld();
			m_currentWorld = newWorld.GetWorld();
			WaitForKey();
			MapGenProcess();
		}
		if (m_lastKey == VK_ESCAPE) {
			m_status = 1;
			CentralProcess();
		}
	}

}

void GameProcess::PrintWorldMono() {
	m_console->Clear();
	m_console->ResetCursorPosition();
	m_console->ResetColours();


	for (int i = 0; i < (g_consoleHeight - 1); i++) {
		for (int j = 0; j < (g_consoleWidth - 1); j++) {
			m_console->WriteChar(&m_currentWorld[i][j].pattern);
		}
		m_console->WriteString("\n");
	}
	WaitForKey();
}

void GameProcess::PrintWorldEfficient() {
	m_console->Clear();
	m_console->ResetCursorPosition();
	m_console->ResetColours();

	char** efficientWorld;
	efficientWorld = new char*[g_consoleHeight - 1];

	for (int i = 0; i < (g_consoleHeight - 1); i++) {
		efficientWorld[i] = new char[g_consoleWidth - 1];			//without this you get nasty access violations in the next line
		for (int j = 0; j < (g_consoleWidth - 1); j++) {
			efficientWorld[i][j] = m_currentWorld[i][j].pattern;
		}
	}

	for (int i = 0; i < (g_consoleHeight - 1); i++) {
		m_console->WriteRow(efficientWorld[i]);
		m_console->WriteString("\n");
	}

	WaitForKey();
}

void GameProcess::PrintWorldWhack(int whackFact) {			// Prints m_currentWorld with a WHACK FACTOR (adds or removes an inputted number to/from the colour code for each tile)
	m_console->Clear();
	m_console->ResetCursorPosition();

	for (int i = 0; i < (g_consoleHeight - 1); i++) {
		for (int j = 0; j < (g_consoleWidth - 1); j++) {
			m_console->SetManualColour(m_currentWorld[i][j].colour + whackFact);
			m_console->WriteChar(&m_currentWorld[i][j].pattern);
		}
		m_console->WriteString("\n");
	}
	m_console->ResetColours();
	WaitForKey();
}

void GameProcess::PrintWorld() {			// Prints m_currentWorld ... Hopefully.	
	m_console->Clear();
	m_console->ResetCursorPosition();

	
	for (int i = 0; i < (g_consoleHeight - 1); i++) {
		for (int j = 0; j < (g_consoleWidth - 1); j++) {
			m_console->SetManualColour(m_currentWorld[i][j].colour);
			m_console->WriteChar(&m_currentWorld[i][j].pattern);
		}
		m_console->WriteString("\n");
	}
	m_console->ResetColours();
	WaitForKey();
}

void GameProcess::GameRunProcess() {
	while (m_status == 3) {
		CheckKey();
		ProcessMovement();

		m_console->SetBackgroundColour(ColourGreen);
		m_console->Clear();
		m_console->ResetCursorPosition();

		DrawPlayer();

		m_console->SetCursorPosition({ 0, 60 });
		m_console->ResetColours();
		m_console->WriteString("NUMPAD CONTROLS - 4, 8, 2, 6. CHECK KEYLOCK IF PROBLEM. ESC TO QUIT.");

		if (m_lastKey == VK_ESCAPE) {
			m_status = 1;
			CentralProcess();
		}
	}
}

void GameProcess::StartEfficiencyTest() {
	m_testStartTime = std::chrono::steady_clock::now();
}

void GameProcess::EndEfficiencyTest() {
	m_testEndTime = std::chrono::steady_clock::now();

	int duration;
	duration = (unsigned int) std::chrono::duration_cast<std::chrono::milliseconds>(m_testEndTime - m_testStartTime).count();

	std::cout << "EXECUTED in " <<  duration << " ms.\n";
	std::cout << "That's an AMAZING " << ((float) 1000 / duration) << " fps!!!";
}