//
// Created by eduardo on 19/11/20.
//

#include "Faction.h"

void Faction::exportStage(const int stage, std::ostream& out) const {
	// Output everything
	out << "* Stage " << stage + 1
		<< "\nRemaining warriors: " << warriors
		<< "\nProbability of attack:\n";

	// Output the probability of factions
	for(int i = 0; i < factions; i++) {
		out << "Faction " << i+1 << '\t' << markovMatrixRow[i]*100.0f << "%\n";
	}

	// The kill count
	out << "\nKill Count:\n";
	for(int i = 0; i < factions; i++) {
		out << "Faction " << i+1 << '\t' << killCount[i] << '\n';
	}

	out << "*************************\n";
}

void Faction::exportNumbers(std::ostream& out) const {
	// Print all numbers
	out << "Generated numbers for the simulation:\n";
	for(const float n : numbers) out << n << '\n';
}
