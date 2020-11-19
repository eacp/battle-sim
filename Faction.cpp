//
// Created by eduardo on 18/11/20.
//

#include "Faction.h"
#include <random>
#include <algorithm>
#include <ctime>

// Utility function to make an uniform
// random number between 0 and 1
float random01(){
	static std::default_random_engine e(time(NULL));
	static std::uniform_real_distribution d(0.0f, 1.0f);

	return d(e);
}

int Faction::getIndex() {
	// Get a random uniform float
	const float r = random01();
	const float adjusted = r*cdf[factions-1]; // Avoid large values after some start becoming 0

	// Register this number for later purposes
	numbers.emplace_back(r);

	// Compute the lower bound of said number and return the index
	float* i = std::lower_bound(cdf, cdf+(factions-1), adjusted);

	const auto index = std::distance(cdf, i);

	// Increase the kc
	killCount[index]++;

	// This should return the index
	return index;
}

void Faction::config(const float* markovRow) {
	// Store the ref
	markovMatrixRow = markovRow;

	// Compute the cdf
	float acum = 0.0f;

	for(int i = 0; i < factions; i++) {
		acum += markovRow[i];
		cdf[i] = acum;
	}
}

bool Faction::damage() {
	if(warriors <= 0) return true;
	warriors--;
	return warriors <= 0;
}

Faction::Faction() {
	// Reserve space to list the random numbers later
	numbers.reserve(2*MAX_FACTIONS);
}

int Faction::totalKillCount() const {
	int total = 0;
	for(int i = 0; i < factions; i++) total+=killCount[i];

	return total;
}