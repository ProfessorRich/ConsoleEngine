#include <windows.h>
#include "RandomSeed.h"
#include <stdio.h> // for debugging
#include <time.h> // for system clock access

RandomSeed::RandomSeed() {
	m_rollPosition = 0;
	m_theSeed = 1;
	srand(m_theSeed);
	ProcessSeed();
}

short RandomSeed::GetRollNumber() {
	return m_rollPosition;
}

void RandomSeed::ProcessSeed() {
	int i;
	for (i = 0; i < g_rollTotal; i++) {
		m_storedRolls[i] = rand();				// No longer rand()%100+1 as I think it works better like this... maybe?
	}
}

void RandomSeed::PrintAllRolls() {
	int i;
	for (i = 0; i < g_rollTotal; i++) {
		printf("%d ", m_storedRolls[i]);
	}
}

void RandomSeed::InputSeed(int newSeed) {			// uses an inputted seed to generate the random number array AND resets the m_rollPosition
	m_rollPosition = 0;
	m_theSeed = newSeed;
	srand(m_theSeed);
	ProcessSeed();
}


int RandomSeed::GetSeed() {
	return m_theSeed;
}
void RandomSeed::GenSeed() {						// Generates a seed from the system clock AND resets the m_rollPosition - uses some jiggery pokery to make the seed seem cooler.
	m_rollPosition = 0;
	while ((rand() * rand() / rand()) == m_theSeed) {			//This shit is designed to ensure that a GenSeed call will return a NEW seed even if called at the same system clock time.
		srand((unsigned int)time(NULL));	
	}
	m_theSeed = (rand() * rand() / rand());

	srand(m_theSeed);
	ProcessSeed();
}

int RandomSeed::NextRoll() {
	if (m_rollPosition >= g_rollTotal) {			// if we get to the end of the stored dice rolls then go back to the start
		m_rollPosition = 0;
	}
	return m_storedRolls[m_rollPosition++];			// There was a lovely bug here where this statement came before the above check and so was reaching values above 1000+ and giving all sorts of weird negative numbers and access violations. It was great fun.
}