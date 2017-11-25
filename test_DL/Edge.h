#pragma once
#include "Neuron.h"
#include <random>
using namespace std;

class Edge
{
public:
	Edge(Neuron* left, Neuron* right);
	double weight;
	double d_weight;
	Neuron* left;
	Neuron* right;
	~Edge();
};

