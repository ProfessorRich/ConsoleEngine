#include <windows.h>
#include "WorldGen.h"
#include <iostream>

WorldGen::WorldGen() {																// This constructor should never be called...
	m_newWorldMap = new tile* [g_consoleHeight - 1];						

	for (int h = 0; h < (g_consoleHeight - 1); h++) {
		m_newWorldMap[h] = new tile[g_consoleWidth - 1];
		for (int w = 0; w < (g_consoleWidth - 1); w++) {
			m_newWorldMap[h][w] = g_debugTile;								
		}
	}

	// 	m_console = new ConsoleEngine;							// Something like this should be here, but when it's here it fucks up the window size.
	RandomSeed x;
	m_currentSeed = &x;
	m_statCount = 0;
	m_worldGenStats = new int[m_maxStats];
}

WorldGen::WorldGen(ConsoleEngine* console, RandomSeed* seed) {
	m_console = console;
	m_currentSeed = seed;
	m_newWorldMap = new tile* [g_consoleHeight - 1];								// Theoretically all this bollocks reverses memory for the array.
	
	for (int h = 0; h < (g_consoleHeight - 1); h++) {
		m_newWorldMap[h] = new tile[g_consoleWidth - 1];
		for (int w = 0; w < (g_consoleWidth - 1); w++) {
			m_newWorldMap[h][w] = g_debugTile;										// fill in some initial values for testing. Maybe better to call WipeWorld, or remove the call.
		}
	}
	m_statCount = 0;
	m_worldGenStats = new int[m_maxStats];
}

void WorldGen::PrintWorldGen() {														// Probably this is a retarded duplication doing what is essentially GameProcess::PrintWorld...

	m_console->ResetCursorPosition();


	for (int i = 0; i < (g_consoleHeight - 1); i++) {
		for (int j = 0; j < (g_consoleWidth - 1); j++) {
			m_console->SetManualColour(m_newWorldMap[i][j].colour);
			m_console->WriteChar(&m_newWorldMap[i][j].pattern);
		}
		m_console->WriteString("\n");
	}
	m_console->ResetColours();
}

void WorldGen::WipeWorld() {
	for (int h = 0; h < (g_consoleHeight - 1); h++) {
		for (int w = 0; w < (g_consoleWidth - 1); w++) {
			m_newWorldMap[h][w] = g_clearTile;										// wipe
		}
	}
}

int* WorldGen::GenerateNewWorld() {
	logAllConstants();

	WipeWorld();

	PrintWorldGen();
	GenerateLand();

	PrintWorldGen();
	GenerateSea();

	PrintWorldGen();
	GenerateForests();

	PrintWorldGen();
	GenerateStreams();

	PrintWorldGen();
	return m_worldGenStats;
}

void WorldGen::logAllConstants() {
	logGenStat(123);
	logGenStat(m_seaAmount);
	logGenStat(m_coastVariance);
	logGenStat(m_maxForests);
	logGenStat(m_minForests);
	logGenStat(m_woodlandChance);
	logGenStat(m_initialForestChance);
	logGenStat(m_forestSpreadDecay);		
}

void WorldGen::logGenStat(int toLog) {
	if (m_statCount < m_maxStats) {
		m_worldGenStats[m_statCount++] = toLog;
	}
}


void WorldGen::GenerateLand() {
	for (int h = 0; h < (g_consoleHeight - 1); h++) {
		for (int w = 0; w < (g_consoleWidth - 1); w++) {
			m_newWorldMap[h][w] = g_fieldTile;
		}
	}
}

void WorldGen::GenerateSea() { 
	int nextCoastLine, lastCoastLine = ((g_consoleWidth - 1) / (100 / m_seaAmount));										// starts the coast at m_seaAmount percentage of g_consoleWidth - 1
	
	for (int h = 0; h < (g_consoleHeight - 1); h++) {
		nextCoastLine = lastCoastLine + (m_currentSeed->NextRoll() % m_coastVariance) - (m_coastVariance / 2);				// Randomise 0 to m_coastVariance and place the next water tiles up till there. There should be a + 1 in here but that looks shitter so I took it out.
		if (nextCoastLine <= 0) nextCoastLine = 0;
		for (int w = 0; w < nextCoastLine; w++) {
			m_newWorldMap[h][w] = g_waterTile;
		}
		m_newWorldMap[h][nextCoastLine] = g_coastTile;

		if ((nextCoastLine == 0) && (lastCoastLine == 0)) m_newWorldMap[h][nextCoastLine] = g_fieldTile;					// If the sea hits the wall, stops drawing coast tiles.

		if ((lastCoastLine == 0) && (nextCoastLine > 0)) m_newWorldMap[h - 1][lastCoastLine] = g_coastTile;					// If the sea comes back from the wall, draws a coast tile on previous height. 

		lastCoastLine = nextCoastLine;														
	}
}

void WorldGen::GenerateForests() {
	int numberOfForests = 0;
	do {
		numberOfForests = (m_currentSeed->NextRoll() % m_maxForests) + 1;
	} while (numberOfForests < m_minForests);

	logGenStat(numberOfForests);				// Logs the total number of forests to be started (unless for one or more, StartNewForest fails to find a suitable spot in 20 tries...)

	for (int i = 0; i < numberOfForests; i++) {
		StartNewForest(0);
	}
	PrintWorldGen();
}
void WorldGen::StartNewForest(short infinityProtection) {
	COORD forestCentre;

	forestCentre.Y = (m_currentSeed->NextRoll() % (g_consoleHeight - 1));								// Again... no + 1 here else I get crashes... cba working it out.
	forestCentre.X = (m_currentSeed->NextRoll() % (g_consoleWidth - 1));
	if (CheckIsField(forestCentre)) {												// If a fieldTile is randomly found then it's forest time. 
		m_newWorldMap[forestCentre.Y][forestCentre.X] = g_forestTile;
		WoodlandSpread(forestCentre);
		ForestSpread(forestCentre, m_initialForestChance);
	}
	else if (infinityProtection < 20) {
		StartNewForest(++infinityProtection);					// hmmmm
	}
}

void WorldGen::WoodlandSpread(COORD woodlandCentre) {
	woodlandCentre.Y++;
	WoodlandSpreadCheck(woodlandCentre);
	woodlandCentre.X++;
	WoodlandSpreadCheck(woodlandCentre);
	woodlandCentre.Y--;
	WoodlandSpreadCheck(woodlandCentre);
	woodlandCentre.Y--;
	WoodlandSpreadCheck(woodlandCentre);
	woodlandCentre.X--;
	WoodlandSpreadCheck(woodlandCentre);
	woodlandCentre.X--;
	WoodlandSpreadCheck(woodlandCentre);
	woodlandCentre.Y++;
	WoodlandSpreadCheck(woodlandCentre);
	woodlandCentre.Y++;
	WoodlandSpreadCheck(woodlandCentre);
}

bool WorldGen::CheckBounds(COORD checkCoord) {
	if ((checkCoord.Y < 0) || (checkCoord.X < 0) || (checkCoord.Y >= (g_consoleHeight - 1)) || (checkCoord.X >= (g_consoleWidth - 1))) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}


void WorldGen::WoodlandSpreadCheck(COORD woodlandCentre) {
	if (((m_currentSeed->NextRoll() % 100) + 1) < m_woodlandChance) {
		if (CheckIsField(woodlandCentre)) m_newWorldMap[woodlandCentre.Y][woodlandCentre.X] = g_woodlandTile;
	}
}

bool WorldGen::CheckIsField(COORD fieldCoord) {
	if (CheckBounds(fieldCoord)) {
		return (m_newWorldMap[fieldCoord.Y][fieldCoord.X].pattern == g_fieldTile.pattern);
	}
	else {
		return false;
	}
}

bool WorldGen::CheckIsNotCoast(COORD notCoastCoord) {
	if (CheckBounds(notCoastCoord)) {
		return ((m_newWorldMap[notCoastCoord.Y][notCoastCoord.X].pattern != g_waterTile.pattern) && (m_newWorldMap[notCoastCoord.Y][notCoastCoord.X].pattern != g_coastTile.pattern));
	}
	else {
		return false;
	} 
}

bool WorldGen::CheckIsWoodland(COORD woodlandCoord) {
	if (CheckBounds(woodlandCoord)) {
		return (m_newWorldMap[woodlandCoord.Y][woodlandCoord.X].pattern == g_woodlandTile.pattern);
	}
	else {
		return false;
	}
}

void WorldGen::ForestSpreadCheck(COORD forestCentre, int spreadChance) {
	if (CheckIsWoodland(forestCentre)) {
		if (((m_currentSeed->NextRoll() % 100) + 1) < spreadChance) {
			m_newWorldMap[forestCentre.Y][forestCentre.X] = g_forestTile;
			WoodlandSpread(forestCentre);
			if ((spreadChance < 10) && (m_currentSeed->NextRoll() % 1000) < 20) {
				ForestSpread(forestCentre, (spreadChance = m_initialForestChance));  // Chance of revitalisation when forest dying out.
			}
			else {
				ForestSpread(forestCentre, (spreadChance - m_forestSpreadDecay));
			}
		}
	}
}

void WorldGen::ForestSpread(COORD forestCentre, int spreadChance) {
	forestCentre.Y++;
	ForestSpreadCheck(forestCentre, spreadChance);

	forestCentre.Y--;
	forestCentre.Y--;
	ForestSpreadCheck(forestCentre, spreadChance);

	forestCentre.Y++;
	forestCentre.X++;
	ForestSpreadCheck(forestCentre, spreadChance);

	forestCentre.X--;
	forestCentre.X--;
	ForestSpreadCheck(forestCentre, spreadChance);

}

void WorldGen::GenerateStreams() {
	for (int i = 0; i < (m_numberOfStreams); i++) {
		GenerateOneStream();
	}
}

void WorldGen::GenerateOneStream() {
	COORD streamPlacer;
	streamPlacer.Y = 0;
	streamPlacer.X = 0;

	do {
		streamPlacer.X = m_currentSeed->NextRoll() % (g_consoleWidth - 1);
	} while (!CheckIsField(streamPlacer));

	m_newWorldMap[streamPlacer.Y][streamPlacer.X] = g_streamTile;

	GrowStream(streamPlacer, m_streamForkChance);
}

void WorldGen::GrowStream(COORD streamPlacer, int forkChance) {
	if (forkChance < 0) {
		forkChance = 0;
	}

	if (streamPlacer.Y >= (g_consoleHeight - 1)) {
		return;
	}
	else if (streamPlacer.X >= (g_consoleWidth - 1)) {
		return;
	}

	if ((m_currentSeed->NextRoll() % 100) < forkChance) {
		COORD streamForker = streamPlacer;
		streamForker.X--;
		GrowStream(streamForker, (forkChance-3));
	}

	bool streampath = FALSE;
	bool placed = FALSE;
	int rand = (((m_currentSeed->NextRoll() % 3) + (m_currentSeed->NextRoll() % 3) + 0) / 3);

	streamPlacer.Y++;
	streamPlacer.X--;

	streamPlacer.X += rand;

	if (CheckIsNotCoast(streamPlacer)) {
		if (CheckIsField(streamPlacer)) {
			m_newWorldMap[streamPlacer.Y][streamPlacer.X] = g_streamTile;
			placed = TRUE;
		}
		streamPlacer.X++;
		if (CheckIsNotCoast(streamPlacer) && (rand > 0)) {
			if (CheckIsField(streamPlacer)) {
				m_newWorldMap[streamPlacer.Y][streamPlacer.X] = g_diagStreamTile;
				placed = TRUE;
			}
			else {
				placed = FALSE;
			}
			streamPlacer.X++;
			if (CheckIsNotCoast(streamPlacer) && (rand > 1)) {
				if (CheckIsField(streamPlacer)) {
					m_newWorldMap[streamPlacer.Y][streamPlacer.X] = g_sideStreamTile;
					placed = TRUE;
				}
				else {
					placed = FALSE;
				}
			}

		}
		streampath = TRUE;
	}
	else {
		streamPlacer.X -= rand;
	}

	int i = 0;
	while (!streampath && i < 3) {
		if (CheckIsNotCoast(streamPlacer)) {
			if (CheckIsField(streamPlacer)) {
				m_newWorldMap[streamPlacer.Y][streamPlacer.X] = g_streamTile;
				placed = TRUE;
			}
			streampath = TRUE;
		}
		else {
			i++;
			streamPlacer.X++;
		}
	}
	if (streampath) {
		if (!placed) {
			streamPlacer.X -= 2;
			streamPlacer.Y--;
			if (CheckIsField(streamPlacer)) {
				m_newWorldMap[streamPlacer.Y][streamPlacer.X] = g_brookTile;
			}
			streamPlacer.X += 2;
			streamPlacer.Y++;
		}
		GrowStream(streamPlacer, --forkChance);
	}
	else {
		streamPlacer.X -= 2;
		streamPlacer.Y--;
		if (CheckIsField(streamPlacer)) {
			m_newWorldMap[streamPlacer.Y][streamPlacer.X] = g_brookTile;
		}
	}
}


tile** WorldGen::GetWorld() {
	return m_newWorldMap;
}