#pragma once
#include "Neuron.h"
#include "Edge.h"
#include <math.h> 
#include <vector>
using namespace std;
class MachineLearning
{
public:
	MachineLearning(int l,...);
	vector<vector<Neuron*>*>* levels;
	double eta;
	double alfa;
	double assert(vector<double> input, vector<double> expected);
	double epsilon(vector<double> output, vector<double> expected);
	vector<double> setInput(vector<double> input);
	double sigma(double val);
	double dsigma(double val);
	vector<double> compute();
	~MachineLearning();
};

