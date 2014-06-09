#include "som.h"

SOM::SOM(int xDim, int yDim, int _numInputs, int _dimension)
{
	winnerNeuron 		= 0;
	numEpochs 			= 0;
	acceptableError 	= 0.0;
	eta0 = sigma0 = tau = 0.0;
	distance 			= 0.0;
	lastSeason			= 0;

	dimension = _dimension;
	numInputs = _numInputs;
	neurons.resize(xDim);

	for (int x = 0; x < xDim; x++)
	{
		neurons[x].resize(yDim);
	}

	for (unsigned int x = 0; x < neurons.size(); x++)
	{
		for (unsigned int y = 0; y < neurons[x].size(); y++)
		{
			neurons[x][y].init(numInputs, x, y, dimension);
		}
	}
}

void SOM::trainingConfig(int _numEpochs, double _acceptableError, double _eta0, double _sigma0, double _tau)
{
	numEpochs 		= _numEpochs;
	acceptableError = _acceptableError;
	eta0 			= _eta0;
	sigma0 			= _sigma0;
	tau 			= _tau;
}

void SOM::shortestDistance(std::vector<double>& data)
{
	distance = neurons[0][0].euclidianDistance(data);
	winnerNeuron = neurons[0][0].id;

	for (unsigned int x = 0; x < neurons.size(); x++)
	{
		for (unsigned int y = 0; y < neurons[x].size(); y++)
		{
			if(distance > neurons[x][y].euclidianDistance(data))
			{
				distance = neurons[x][y].euclidianDistance(data);
				winnerNeuron = neurons[x][y].id;
			}
		}
	}
}

double SOM::euclidianDistance(int xWinner, int yWinner, int xNeighbor, int yNeighbor)
{
	return( pow(xWinner - xNeighbor, 2) + pow(yWinner - yNeighbor, 2) );
}

void SOM::adjustWeights(int index, std::vector<double>& inputData)
{
	long double sigma;				// largura efetiva da vizinhanca topologica
	long double lateralDistance;	// distancia euclidiana entre dois neuronios vizinhos
	long double h;					// grau de interacao ou cooperacao entre dois neuronios vizinhos
	long double eta;				// taxa de aprendizagem

	int xWinner = winnerNeuron / dimension;
	int yWinner = winnerNeuron % dimension;

	for (unsigned int x = 0; x < neurons.size(); x++)
	{
		for (unsigned int y = 0; y < neurons[x].size(); y++)
		{
			sigma = sigma0 * exp(-index / tau);	// Diminui com o tempo, o que implica em valores de h menores

			lateralDistance = euclidianDistance(xWinner, yWinner, x, y);

			/* parametro simetrico em relacao ao neuronio vencedor e decresce monotonamente com
			 * o aumento distancia lateral. A funcao gaussiana abaixo atende essas condicoes */
			h = exp( -lateralDistance / (2 * pow(sigma, 2)) );

			/* a taxa de aprendizagem decresce gradualmente ao longo do tempo. A finalidade é evitar que dados novos,
			 * apresentados após um longo treinamento, venham a comprometer seriamente o conhecimento que já está sedimentado.
			 * O mesmo ocorre com o cérebro humano: quanto mais idosa a pessoa se torna, menor é a capacidade de aprender.*/
			eta = eta0 * exp( -index /tau );

			// Ver Simon Haykin
			if(eta <= 0.01)
			{
				eta = 0.02;
			}

			// Atualizacao dos pesos sinapticos
			for (int w = 0; w < neurons[x][y].numSynapses; w++)
			{
				neurons[x][y].weights[w] += eta * h * (inputData[w] - neurons[x][y].weights[w]);
			}
		}
	}
}

void SOM::training(std::vector< std::vector<double> >& inputData)
{
	for (int epoch = 0; epoch < numEpochs; epoch++)
	{
		for (unsigned int example = 0; example < inputData.size(); example++)
		{
			// procura o neuronio que tem os pesos sinapticos mais
			// proximos do exemplo corrente. Este neuronio sera o vencedor
			shortestDistance(inputData[example]);

			int index = ( epoch * inputData.size() ) + example; // iteracao
			// Ajusta os pesos do neuronio vencedor, aproximando-o ainda mais do padrão apresentado
			adjustWeights(index, inputData[example]);
		}

		lastSeason++;

		if(distance < acceptableError)
		{
			cout << "Treinamento completado por Erro Aceitavel" << endl << endl;
			break;
		}
	}
}

void SOM::clustering(std::vector< std::vector<double> >& inputData, std::vector<int>& target)
{
	int xWinner, yWinner;

	for (unsigned int example = 0; example < inputData.size(); example++)
	{
		shortestDistance(inputData[example]);

		xWinner = winnerNeuron / dimension;
		yWinner = winnerNeuron % dimension;

		neurons[xWinner][yWinner].label = target[example];
	}

	for (unsigned int x = 0; x < neurons.size(); x++)
	{
		for (unsigned int y = 0; y < neurons[x].size(); y++)
		{
			cout << neurons[x][y].label << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void SOM::operation(std::vector< std::vector<double> >& inputData, std::vector<int>& target)
{
	int xWinner, yWinner;
	int hits, errors;

	hits = 0;
	errors = 0;

	for (unsigned int example = 0; example < inputData.size(); example++)
	{
		shortestDistance(inputData[example]);

		xWinner = winnerNeuron / dimension;
		yWinner = winnerNeuron % dimension;

		if(neurons[xWinner][yWinner].label == target[example])
			hits++;
		else
			errors++;

		cout << "Teste " << example << ": " << "(classe associada --> " << neurons[xWinner][yWinner].label << ") | (classe esperada --> " << target[example] << ")" << endl;
	}
	cout << endl << "Acertos: " << hits << endl;
	cout << "Erros: " << errors << endl;
}

void SOM::printNeurons()
{
	for (unsigned int x = 0; x < neurons.size(); x++)
	{
		for (unsigned int y = 0; y < neurons[x].size(); y++)
		{
			cout << neurons[x][y].id << " ";
		}
		cout << endl;
	}
}

void SOM::printWeights()
{
	for (unsigned int x = 0; x < neurons.size(); x++)
	{
		for (unsigned int y = 0; y < neurons[x].size(); y++)
		{
			cout << "Neuronio " << neurons[x][y].id << ":" << endl;

			for (int z = 0; z < neurons[x][y].numSynapses; z++)
			{
				cout << "	(Peso " << z << ") --> " << neurons[x][y].weights[z] << endl;
			}
		}
	}
}
