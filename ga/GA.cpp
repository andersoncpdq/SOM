/*
 * GA.cpp
 *
 *  Created on: 18/03/2014
 *      Author: anderson
 */
#include <iomanip>
#include <algorithm>
#include <sstream>
#include "GA.h"
#include "/home/anderson/workspace/SOM/som/som.h"

GA::GA(int numCandidates)
{
	numCrossing = 0;
	numMutation = 0;
	numBestCandidates = 0;
	epochs = 0;
	p1 = p2 = 0;

	candidates.resize(numCandidates);

	for(int i = 0; i < numCandidates; i++)
	{
		candidates[i].create();
	}
}

void GA::config(double _p1, double _p2, int _numCrossing, int _numMutation)
{
	p1 = _p1;
	p2 = _p2;
	numCrossing = _numCrossing;
	numMutation = _numMutation;
	numBestCandidates = numCrossing;

	parents.resize(numCrossing);
	bestCandidate.resize(numBestCandidates);
	bestFitness.resize(numBestCandidates);
}

double GA::objectiveFunction(double trainError, double validError)
{
	return( (p1 * trainError + p2 * validError) / (p1 + p2) );
}

void GA::crossing(Chromosome& s1, Chromosome& s2, Chromosome& child)
{
	child.eta0			= s1.eta0;
	child.sigma0 		= s2.sigma0;
	child.xDimension 	= s2.xDimension;
	child.yDimension 	= s1.yDimension;
}

void GA::mutation(Chromosome& s)
{
	int temp;
	std::vector<int> arrayTemp;

	for(unsigned int i = 0; i < s.sigma0.size(); i++)
	{
		temp = s.eta0[i];
		s.eta0[i] = s.sigma0[i];
		s.sigma0[i] = temp;
	}

	arrayTemp.resize(5);
	arrayTemp = s.xDimension;
	s.xDimension = s.yDimension;
	s.yDimension = arrayTemp;
}

void GA::run(std::vector< std::vector<double> > trainingData, std::vector<int> trainingTarget,
		 	 std::vector< std::vector<double> > validationData, std::vector<int> validationTarget)
{
	int numEpochs			= 100;
	double acceptableError 	= 0.01;
	double tau 				= 100000;	// constante de tempo
	int numInputs 			= trainingData[0].size();

	double eta0 	= 0.0;		// taxa de aprendizagem inicial
	double sigma0 	= 0.0;		// largura efetiva da vizinhanca topologica inicial
	int xDim 		= 0;
	int yDim 		= 0;
	int dimension 	= 0;		// maior dimensao entre x e y

	cout << "Algoritmo Genetico para Otimizacao da Rede de Kohonen" << endl << endl;
	cout << "A G U A R D E   O   P R O C E S S A M E N T O" << endl << endl;

	/*******************************************************
	  	  	  	P O P U L A C A O   I N I C I A L
	 *******************************************************/
	for(unsigned int i = 0; i < candidates.size(); i++)
	{
		xDim = candidates[i].getXdimension();
		yDim = candidates[i].getYdimension();

		dimension = xDim;
		if(yDim > xDim)
			dimension = yDim;

		eta0 = candidates[i].getEta();
		sigma0 = candidates[i].getSigma();

		SOM som(xDim, yDim, numInputs, dimension);
		som.trainingConfig(numEpochs, acceptableError, eta0, sigma0, tau);
		som.training(trainingData);
		som.clustering(trainingData, trainingTarget);
		candidates[i].trainError = som.distance;
		epochs = som.lastSeason;

		som.operation(validationData, validationTarget);
		candidates[i].validError = som.distance;

		candidates[i].fitness = objectiveFunction(candidates[i].trainError, candidates[i].validError);

		if(i == numBestCandidates)
			sortFitness(bestCandidate, bestFitness);

		if(i < numBestCandidates)
		{
			bestCandidate[i] = i;
			bestFitness[i] = candidates[i].fitness;
		}
		else if(bestFitness[numBestCandidates - 1] > candidates[i].fitness)
		{
			bestCandidate[numBestCandidates - 1] = i;
			bestFitness[numBestCandidates - 1] = candidates[i].fitness;
			sortFitness(bestCandidate, bestFitness);
		}
	}

	int generation = 0;

	while(generation <= 4)
	{
		generation++;

		selectParents();
		Chromosome child;

		/*************************************************
		  	  	  	   C R U Z A M E N T O S
		 *************************************************/
		for(unsigned int i = 0; i < numCrossing; i++)
		{
			crossing(candidates[ bestCandidate[i] ], candidates[ parents[i] ], child);

			xDim = child.getXdimension();
			yDim = child.getYdimension();

			dimension = xDim;
			if(yDim > xDim)
				dimension = yDim;

			eta0 = child.getEta();
			sigma0 = child.getSigma();

			SOM som(xDim, yDim, numInputs, dimension);
			som.trainingConfig(numEpochs, acceptableError, eta0, sigma0, tau);
			som.training(trainingData);
			som.clustering(trainingData, trainingTarget);
			child.trainError = som.distance;
			epochs = som.lastSeason;

			som.operation(validationData, validationTarget);
			child.validError = som.distance;

			child.fitness = objectiveFunction(child.trainError, child.validError);

			if(candidates[ bestFitness[i] ].fitness > candidates[ parents[i] ].fitness)
			{
				if(child.fitness < candidates[ bestCandidate[i] ].fitness)
				{
					candidates[ bestCandidate[i] ].eta0 		= child.eta0;
					candidates[ bestCandidate[i] ].sigma0 		= child.sigma0;
					candidates[ bestCandidate[i] ].xDimension 	= child.xDimension;
					candidates[ bestCandidate[i] ].yDimension 	= child.yDimension;
					candidates[ bestCandidate[i] ].trainError	= child.trainError;
					candidates[ bestCandidate[i] ].validError	= child.validError;
					candidates[ bestCandidate[i] ].fitness		= child.fitness;
				}
			} else {
				if(child.fitness < candidates[ parents[i] ].fitness)
				{
					candidates[ parents[i] ].eta0 			= child.eta0;
					candidates[ parents[i] ].sigma0 		= child.sigma0;
					candidates[ parents[i] ].xDimension 	= child.xDimension;
					candidates[ parents[i] ].yDimension 	= child.yDimension;
					candidates[ parents[i] ].trainError		= child.trainError;
					candidates[ parents[i] ].validError		= child.validError;
					candidates[ parents[i] ].fitness		= child.fitness;
				}
			}
		}

		/*************************************************
		  	  	  	   	  M U T A C O E S
		 *************************************************/
		for(unsigned int i = 0; i < numMutation; i++)
		{
			mt_seed();

			int mutant = mt_lrand() % candidates.size();
			mutation(candidates[mutant]);

			xDim = candidates[mutant].getXdimension();
			yDim = candidates[mutant].getYdimension();

			dimension = xDim;
			if(yDim > xDim)
				dimension = yDim;

			eta0 = candidates[mutant].getEta();
			sigma0 = candidates[mutant].getSigma();

			SOM som(xDim, yDim, numInputs, dimension);
			som.trainingConfig(numEpochs, acceptableError, eta0, sigma0, tau);
			som.training(trainingData);
			som.clustering(trainingData, trainingTarget);
			candidates[mutant].trainError = som.distance;
			epochs = som.lastSeason;

			som.operation(validationData, validationTarget);
			candidates[mutant].validError = som.distance;

			candidates[mutant].fitness = objectiveFunction(candidates[mutant].trainError, candidates[mutant].validError);

			if(candidates[ bestFitness[i] ].fitness > candidates[ parents[i] ].fitness)
			{
				if(candidates[mutant].fitness < candidates[ bestCandidate[i] ].fitness)
				{
					candidates[ bestCandidate[i] ].eta0 		= candidates[mutant].eta0;
					candidates[ bestCandidate[i] ].sigma0 		= candidates[mutant].sigma0;
					candidates[ bestCandidate[i] ].xDimension 	= candidates[mutant].xDimension;
					candidates[ bestCandidate[i] ].yDimension 	= candidates[mutant].yDimension;
					candidates[ bestCandidate[i] ].trainError	= candidates[mutant].trainError;
					candidates[ bestCandidate[i] ].validError	= candidates[mutant].validError;
					candidates[ bestCandidate[i] ].fitness		= candidates[mutant].fitness;
				}
			} else {
				if(candidates[mutant].fitness < candidates[ parents[i] ].fitness)
				{
					candidates[ parents[i] ].eta0 			= candidates[mutant].eta0;
					candidates[ parents[i] ].sigma0 		= candidates[mutant].sigma0;
					candidates[ parents[i] ].xDimension 	= candidates[mutant].xDimension;
					candidates[ parents[i] ].yDimension 	= candidates[mutant].yDimension;
					candidates[ parents[i] ].trainError		= candidates[mutant].trainError;
					candidates[ parents[i] ].validError		= candidates[mutant].validError;
					candidates[ parents[i] ].fitness		= candidates[mutant].fitness;
				}
			}
		}

		cout << " #----------# GERACAO - " << generation << " #----------# " << endl;
		for(unsigned int i = 0; i < candidates.size(); i++)
		{
			print(i);
		}
	}

	std::vector<int> top10solutions;
	std::vector<double> top10fitness;
	top10solutions.resize(numCrossing);
	top10fitness.resize(numCrossing);

	for(unsigned int i = 0; i < candidates.size(); i++)
	{
		if(i == numCrossing)
			sortFitness(top10solutions, top10fitness);
		if(i < numCrossing)
		{
			top10solutions[i] = i;
			top10fitness[i] = candidates[i].fitness;
		}
		else if(top10fitness[numCrossing-1] > candidates[i].fitness)
		{
			top10solutions[numCrossing-1] = i;
			top10fitness[numCrossing-1] = candidates[i].fitness;
			sortFitness(top10solutions, top10fitness);
		}
	}

	cout << endl << " #################### MELHORES SOLUCOES #################### " << endl;
	for(unsigned int i = 0; i < numCrossing; i++)
	{
		print(top10solutions[i]);
	}
}

void GA::sortFitness(std::vector<int>& indexCandidate, std::vector<double>& fitness)
{
	int threshold = indexCandidate.size() - 1;
	int lastSwap;
	int tempIndex;
	double tempFitness;

	while(threshold > 0)
	{
		lastSwap = 0;
		for(int i = 0; i < threshold; i++)
		{
			if(fitness[i] > fitness[i+1])
			{
				tempIndex = indexCandidate[i];
				tempFitness = fitness[i];

				indexCandidate[i] = indexCandidate[i+1];
				fitness[i] = fitness[i+1];

				indexCandidate[i+1] = tempIndex;
				fitness[i+1] = tempFitness;

				lastSwap = i;
			}
		}
		threshold = lastSwap;
	}
}

void GA::selectParents()
{
	// semente
	mt_seed();

	for(unsigned int i = 0; i < numCrossing; i++)
	{
		do
		{
			parents[i] = mt_lrand() % candidates.size();

		} while(parents[i] == bestCandidate[i]);
	}
}

void GA::print(int index)
{
		cout << "======================================" << endl;
		cout << "Candidato " << index << ": " << endl;
		cout << "	Taxa de Aprendizagem: " << candidates[index].getEta() << endl;
		cout << "	Largura Efetiva da Vizinhanca: "<< candidates[index].getSigma() << endl;
		cout << "	Dimensao X: " << candidates[index].getXdimension() << endl;
		cout << "	Dimensao Y: " << candidates[index].getYdimension() << endl;
		cout << "--------------------------------------" << endl;
		cout << "	Qtd. de Epocas executadas: " << epochs << endl;
		cout << "	Erro de treinamento: " << candidates[index].trainError << endl;
		cout << "	Erro de validacao: " << candidates[index].validError << endl;
		cout << "	Fitness: " << candidates[index].fitness << endl;
}
