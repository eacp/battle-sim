//
// Created by eduardo on 18/11/20.
//

#ifndef BATTLE_FACTION_H
#define BATTLE_FACTION_H

#include <vector>
#include <ostream>

// From the specification
#define MAX_FACTIONS 25

class Faction {
	// The CDF (sums)
	float cdf[MAX_FACTIONS];

	// A markiv reference
	const float* markovMatrixRow;

	// The vector with the generated random numbers
	std::vector<float> numbers;

	int killCount[MAX_FACTIONS] = {0};

public:

	// Default constructor with initialization
	Faction();

	// How many soldiers does this one have
	int warriors;

	// The factions
	inline static int factions;

	// Pick an enemy
	int getIndex();

	// Make initial config
	void config(const float* markovRow);

	// Func to receive damage
	bool damage();

	// Total KC
	int totalKillCount() const;

	// Export the data on each stage
	void exportStage(const int stage, std::ostream& out) const;

	// Export the generated numbers
	void exportNumbers(std::ostream& out) const;

};

#endif //BATTLE_FACTION_H
