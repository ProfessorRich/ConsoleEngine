#pragma once

#include "GlobalConstants.h"

class RandomSeed {
private:
	int m_storedRolls[g_rollTotal];
	short m_rollPosition;
	unsigned int m_theSeed;
	

public:
	RandomSeed();

	void ProcessSeed();			// PROCESS THE SEED!!! - in other words... generate a list of 1000 numbers between 1-100 in an array using the current srand(m_theSeed);
	void InputSeed(int);		// input a new seed manually
	int GetSeed();				// get the current seed.
	void GenSeed();				// generated a 'random' seed using system clock.
	void PrintAllRolls();       // for debugging
	int NextRoll();				// next roll of dice
	short GetRollNumber();		// return m_rollPosition;
};