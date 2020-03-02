#pragma once

#include "GlobalConstants.h"
#include "ConsoleEngine.h"
#include "RandomSeed.h"

// World Gen Constants

const int m_seaAmount = 30;				// Amount of sea as approx percentage.
const int m_coastVariance = 8;			// Maximum difference in distance between adjacent coast tiles.
const int m_maxForests = 10;			// Maximum number of forests.
const int m_minForests = 5;				// Minimum ...
const int m_woodlandChance = 60;		// Chance of woodland spreading in all 8 directions from a forest tile.
const int m_initialForestChance = 90;	// Chance of forest spreading from an initial (or, 'mother') forest tile on each woodland tile in 4 directions (N E S W)
const int m_forestSpreadDecay = 3;		// How much the forest spread chance decays from the mother to child forest tiles.
const int m_streamForkChance = 15;		// Chance that a river will fork.
const int m_numberOfStreams = 2;		// Number of streams to start.

const int m_maxStats = 10;

class WorldGen {
private:
	tile** m_newWorldMap;
	ConsoleEngine* m_console;
	RandomSeed* m_currentSeed;

	int* m_worldGenStats;
	int m_statCount;
		
public:
	WorldGen();
	WorldGen(ConsoleEngine*, RandomSeed*);

	bool CheckBounds(COORD);
	bool CheckIsField(COORD);
	bool CheckIsWoodland(COORD);
	bool CheckIsNotCoast(COORD);

	void logGenStat(int);
	void logAllConstants();

	void WipeWorld();
	int* GenerateNewWorld();
	void GenerateLand();
	void GenerateSea();
	void GenerateForests();
	void GenerateStreams();
	void GenerateOneStream();
	void GrowStream(COORD, int);
	void StartNewForest(short);
	void WoodlandSpread(COORD);
	void WoodlandSpreadCheck(COORD);
	void ForestSpreadCheck(COORD, int);
	void ForestSpread(COORD, int);
	tile** GetWorld();
	void PrintWorldGen();
	// void CinematicWorldGen();
};