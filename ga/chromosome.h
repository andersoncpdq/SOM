/*
 * chromosome.h
 *
 *  Created on: 17/03/2014
 *      Author: anderson
 */

#ifndef CHROMOSOME_H_
#define CHROMOSOME_H_

#include <vector>
#include <cmath>
#include </home/anderson/workspace/SOM/mersenne/mtwist.h>

using namespace std;

class Chromosome
{

public:
	std::vector<int> eta0;
	std::vector<int> sigma0;
	std::vector<int> xDimension;
	std::vector<int> yDimension;
	double trainError;
	double validError;
	double fitness;

	Chromosome();
	void create();
	int randomize();
	double toInt(std::vector<int>& num);
	double toFloat(std::vector<int>& num);
	double getEta();
	double getSigma();
	int getXdimension();
	int getYdimension();
};

#endif /* CHROMOSOME_H_ */
