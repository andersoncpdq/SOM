/*
 * Name: som.h
 * Author: Anderson Couto (andersoncpdq@gmail.com)
 * Description: Self-Organization Maps (SOM) or Kohonen Maps
 */

#include "neuron.h"
using namespace std;

#ifndef SOM_H_
#define SOM_H_

class SOM
{

public:
	std::vector< std::vector<Neuron> > neurons;
	int winnerNeuron;
	int numEpochs, numInputs;
	int lastSeason;		// armazena a qtd de epocas executadas
	int dimension;
	double acceptableError;
	double eta0;	// valor inicial da taxa de aprendizagem
	double sigma0;	// valor inicial da largura efetiva da vizinhanca topologica
	double tau;		// constante de tempo
	long double distance;

	SOM(int xDim, int yDim, int _numInputs, int _dimension);
	void trainingConfig(int _numEpochs, double _acceptableError, double _eta0, double _sigma0, double _tau);
	void shortestDistance(std::vector<double>& data);
	double euclidianDistance(int xWinner, int yWinner, int xNeighbor, int yNeighbor);
	void adjustWeights(int index, std::vector<double>& inputData);
	void training(std::vector< std::vector<double> >& inputData);
	void clustering(std::vector< std::vector<double> >& inputData, std::vector<int>& target);
	void operation(std::vector< std::vector<double> >& inputData, std::vector<int>& target);

	void printNeurons();
	void printWeights();
};

#endif /* SOM_H_ */
