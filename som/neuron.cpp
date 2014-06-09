#include "neuron.h"

Neuron::Neuron()
{
	id = 0;
	label = 0;
	numSynapses = 0;
}

void Neuron::init(int _numSynapses, int x, int y, int dimension)
{
	numSynapses = _numSynapses;
	id = (x * dimension) + y;
	weights.resize(numSynapses);

	weightsInit();
}

void Neuron::weightsInit()
{
	// semente
	mt_seed();

	for (int i = 0; i < numSynapses; i++)
	{
		weights[i] = mt_ldrand(); // gera numeros pseudo-aleatorios entre 0.0 - 1.0
	}
}

double Neuron::euclidianDistance(std::vector<double>& input)
{
	double distance = 0.0;

	for (int i = 0; i < numSynapses; i++)
	{
		distance += pow(input[i] - weights[i], 2);
	}

	return distance;
}
