#include <iostream>
#include "Faction.h"
#include <random>
#include <ctime>
#include <string> // To String

// Test with strings first
#include <fstream>

#define SQLIN(row,col,size) size*row + col

template<typename T>
void printMat(const T* matLin, const int rows, const int cols) {
	int index;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			index = SQLIN(i,j,cols);
			std::cout << matLin[index] << ' ';
		}
		std::cout << '\n';
	}
}



int main() {
	// Fast IO?
	std::ios_base::sync_with_stdio(false);
	// Allocate space for the factions
	float markov[MAX_FACTIONS*MAX_FACTIONS];
	Faction factions[MAX_FACTIONS];


	// Get the number of factions from the input
	std::cout << "Please enter the number of factions\n";
	std::cin >> Faction::factions;
	const int f = Faction::factions;

	// One stream per faction
	std::ofstream files[MAX_FACTIONS];


	// Open files
	for(int i = 0; i < f; i++) {
		files[i].open("faction" + std::to_string(i+1) + ".txt");
	}


	std::cout << "Please enter the markov chain here."
			  "It should be a valid transition matrix of warriors " << f;

	// Get the attack markov chain
	for(int i = 0; i < f; i++) {
		for (int j = 0; j < f; j++) {
			// Linearize coordinate
			const int lin = SQLIN(i, j, f);
			// Get the data
			std::cin >> markov[lin];

			// Set it to zero if i and j are the same
			// We still get it so the input is good
			if(i == j) markov[lin] = 0;
		}

		// Config this faction
		const float* row = markov+(i*f);
		factions[i].config(row);
	}

	// Get the warriors of each faction
	for(int i = 0; i < f; i++) {
		std::cout << "\nInsert the number warriors (life) of Faction " << i+1;
		std::cin >> factions[i].warriors;
	}

	// Execute the turn based simulation
	int turns;
	std::cout << "\nWrite the number of turns in the simulation: ";
	std::cin >> turns;



	// Randomly and uniformly select random elements
	std::default_random_engine e(time(NULL));
	std::uniform_int_distribution<int> random(0,f-1);

	for(int t = 0; t < turns; t++) {
		// Log the state of ALL the actions
		int survivors = 0;
		for(int i = 0; i < f; i++) {
			factions[i].exportStage(t, files[i]);
			// Check if at least there is one alive
			if(factions[i].warriors > 0) survivors++;
		}

		if(survivors <= 1) {
			std::cout << "There is onlye one remaining\n";
			break; // Only one: we have a winner
		}


		// Execute turn
		const int currentTurn = random(e);

		std::cout << "\nTurn " << t+1 << "\nRandom Turn: Faction " << currentTurn+1;

		// Export the data of this turn for this faction
		//factions[currentTurn].exportStage(t, files[currentTurn]);

		// Check we can still attack
		if(factions[currentTurn].warriors <= 0) {
			// We cannot attack
			std::cout << "\n\tThis faction can no longer attack, it has already lost";
			continue;
		}

		// Get a random index to attack
		int toAttack = factions[currentTurn].getIndex();

		std::cout << "\n\tAttacking faction " << toAttack+1;

		if(factions[toAttack].warriors <= 0)
			std::cout << "\n\tCannot attack this faction\n";

		// Damage the attacked faction
		const bool finished = factions[toAttack].damage();

		if(finished) {
			// Recompute the matrix
			std::cout << "\n\tThe faction " << toAttack+1
				<< " is out of warriors.\n\tRecalculating matrix\n";

			// Set this one to zero in all the markov chain. It will stop being valid
			for(int r = 0; r < f; r++) {
				const int lin = SQLIN(r, toAttack, f);
				markov[lin] = 0.0f;
			}

			// Print the new matrix
			printMat(markov, f,f);

			// Now we have to recalculate ALL CDFs
			for(int fact = 0; fact < f; fact++) {
				const int lin = SQLIN(fact, 0, f);
				factions[fact].config(markov+lin);
			}
		}
	}

	// Show remaining factions
	std::cout << "\nRemaining warriors per faction:";

	int maxWarriors = factions[0].warriors;
	int winner = 0;


	for(int i = 0; i < f; i++) {
		std::cout << "\n\tFaction " << i+1 << ": " << factions[i].warriors;

		if(factions[i].warriors > maxWarriors) {
			// OMG a better one
			maxWarriors = factions[i].warriors;
			winner = i;
		}
	}

	std::cout << "\nThe winner faction is: Faction " << winner+1;

	int maxKc = 0;
	int maxKcIndex = 0;

	// Export the generated numbers
	for(int i = 0; i < f; i++) {
		// Log
		factions[i].exportNumbers(files[i]);
		// Calc the biggest kill count

		const int kc = factions[i].totalKillCount();

		std::cout << "\nTotal Kill Count of Faction " << i+1 << ": " << kc;

		if(kc > maxKc) {
			maxKc = kc;
			maxKcIndex = i;
		}
	}

	std::cout << "\nThe faction with the biggest Kill Count is Faction " << maxKcIndex+1
		<< " with " << maxKc << " hits.\n";


	return 0;
}

