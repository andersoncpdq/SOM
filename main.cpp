/*
 *	Name: main.cpp
 *	Author: Anderson Couto (andersoncpdq@gmail.com)
 *	Description: Self-Organization Maps (SOM) or Kohonen Maps
 */

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

#include "som/som.h"
//#include "ga/GA.h"

using namespace std;

void fileInput(const char dataset[], std::vector< std::vector<double> >& data, std::vector<int>& target);

int main()
{
	/*********************************************************************
	 	 	 	 	 A L G O R I T M O   G E N É T I C O
	 *********************************************************************/
/*
	// conjunto de treinamento
	std::vector< std::vector<double> > trainingData;
	std::vector<int> trainingTarget;

	// conjunto de validacao
	std::vector< std::vector<double> > validationData;
	std::vector<int> validationTarget;

	fileInput("datasets/iris.data", trainingData, trainingTarget);
	fileInput("datasets/iris.test", validationData, validationTarget);

	GA ga(50);
	ga.config(1, 1, 20, 10);
	ga.run(trainingData, trainingTarget, validationData, validationTarget);
*/
	/*********************************************************************
	  M A P A S   A U T O - O R G A N I Z A V E I S   D E   K O H O N E N
	 *********************************************************************/
	int numEpochs			= 2500;
	double acceptableError 	= 0.001;
	double eta0 			= 0.2;		// valor inicial da taxa de aprendizagem
	double sigma0 			= 0.8;		// valor inicial da largura efetiva da vizinhanca topologica
	double tau 				= 100000;	// constante de tempo

	// conjunto de treinamento
	std::vector< std::vector<double> > trainingData;
	std::vector<int> trainingTarget;

	// conjunto de validacao
	std::vector< std::vector<double> > validationData;
	std::vector<int> validationTarget;

	cout << endl << "Mapas auto-organizaveis de Kohonen" << endl;

	/*********************************************************************
	  	  	  	  F A S E   D E   T R E I N A M E N T O
	 *********************************************************************/
	fileInput("datasets/cefaleia.data", trainingData, trainingTarget);
	//fileInput("datasets/iris.data", trainingData, trainingTarget);
	//fileInput("datasets/wine.data", trainingData, trainingTarget);

	cout << endl << " A G U A R D E   O   P R O C E S S A M E N T O" << endl << endl;

	int xDim = 5;
	int yDim = 6;
	int nInput = trainingData[0].size();
	int dimension;

	dimension = xDim;
	if(yDim > xDim)
		dimension = yDim;

	SOM som(xDim, yDim, nInput, dimension);
	//som.printNeurons();

	som.trainingConfig(numEpochs, acceptableError, eta0, sigma0, tau);
	som.training(trainingData);
	som.clustering(trainingData, trainingTarget);

	/*********************************************************************
	  	  	  	  	  	  F A S E   D E   O P E R A C A O
	 *********************************************************************/
	fileInput("datasets/cefaleia.test", validationData, validationTarget);
	//fileInput("datasets/iris.test", validationData, validationTarget);
	//fileInput("datasets/wine.test", validationData, validationTarget);

	som.operation(validationData, validationTarget);

    cout << endl << "Processamento concluido!" << endl;

	return 0;
}

void fileInput(const char dataset[], std::vector< std::vector<double> >& data, std::vector<int>& target)
{
	int numExamples, numInputs, numTargets;
	const int TITLE_LENGHT = 100;
	char title[TITLE_LENGHT];

	// construtor ifstream abre o arquivo.
	ifstream inFile(dataset, ios::in);

	// encerra o programa se o arquivo nao for aberto.
	if(!inFile)
	{
		cerr << "O arquivo de treinamento ou teste nao pode ser aberto!" << endl;
		exit(1);
	}

	inFile.getline(title, TITLE_LENGHT, '\n');

	inFile >> numExamples;
	inFile >> numInputs;
	inFile >> numTargets;

	inFile.getline(title, TITLE_LENGHT, '\n');
	inFile.getline(title, TITLE_LENGHT, '\n');

	data.resize(numExamples);
	target.resize(numExamples);

	for (int i = 0; i < numExamples; i++)
	{
		data[i].resize(numInputs);
	}

	for (unsigned int i = 0; i < data.size(); i++)
	{
		//cout << i << ": ";
		for (unsigned int j = 0; j < data[i].size(); j++)
		{
			inFile >> data[i][j];
			//cout << data[i][j] << " ";
		}

		inFile >> target[i];
		//cout << "output: " << target[i] << endl;
	}

	inFile.close();
}
