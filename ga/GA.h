/*
 * GA.h
 *
 *  Created on: 18/03/2014
 *      Author: anderson
 */

#ifndef GA_H_
#define GA_H_

#include "chromosome.h"

class GA
{

public:
	std::vector<Chromosome> candidates;
	std::vector<int> parents;
	std::vector<int> bestCandidate;
	std::vector<double> bestFitness;
	int epochs;
	unsigned int numCrossing;
	unsigned int numMutation;
	unsigned int numBestCandidates;
	double p1, p2;

	GA(int numCandidates);
	void config(double _p1, double _p2, int _numCrossing, int _numMutation);
	void run(std::vector< std::vector<double> > trainingData, std::vector<int> trainingTarget,
			 std::vector< std::vector<double> > validationData, std::vector<int> validationTarget);
	double objectiveFunction(double trainError, double validError);
	void sortFitness(std::vector<int>& indexCandidate, std::vector<double>& fitness);
	void selectParents();
	void crossing(Chromosome& s1, Chromosome& s2, Chromosome& child);
	void mutation(Chromosome& s);

	void print(int index);
};

#endif /* GA_H_ */
