/*
 * chromosome.cpp
 *
 *  Created on: 17/03/2014
 *      Author: anderson
 */

#include "chromosome.h"

Chromosome::Chromosome()
{
	eta0.resize(6);
	sigma0.resize(5);
	xDimension.resize(5);
	yDimension.resize(5);

	trainError = 0.0;
	validError = 0.0;
	fitness = 0.0;
}

int Chromosome::randomize()
{
	float value;

	mt_seed();

	value = mt_drand();

	if(value >= 0.5)
		return 1;
	else
		return 0;
}

double Chromosome::toInt(std::vector<int>& num)
{
	double result = 0.0;
	for(unsigned int i = 0; i < num.size(); i++)
	{
		result += num[i] * pow(2, i);
	}
	return result;
}

double Chromosome::toFloat(std::vector<int>& num)
{
	double result = 0.0;
	for(unsigned int i = 0; i < num.size(); i++)
	{
		result += num[i] / pow(2, (num.size()-i) );
	}
	return result;
}

void Chromosome::create()
{
	do {
		for(unsigned int i = 0; i < eta0.size(); i++)
		{
			eta0[i] = randomize();
		}
	} while( toFloat(eta0) <= 0.09 );

	do {
		for(unsigned int i = 0; i < sigma0.size(); i++)
		{
			sigma0[i] = randomize();
		}
	} while( toFloat(sigma0) <= 0.09 );

	do {
		for(unsigned int i = 0; i < xDimension.size(); i++)
		{
			xDimension[i] = randomize();
		}
	} while( toInt(xDimension) <= 0 );

	do {
		for(unsigned int i = 0; i < yDimension.size(); i++)
		{
			yDimension[i] = randomize();
		}
	} while( toInt(yDimension) <= 0 );
}

double Chromosome::getEta()
{
	return toFloat(eta0);
}

double Chromosome::getSigma()
{
	return toFloat(sigma0);
}

int Chromosome::getXdimension()
{
	return toInt(xDimension);
}

int Chromosome::getYdimension()
{
	return toInt(yDimension);
}
