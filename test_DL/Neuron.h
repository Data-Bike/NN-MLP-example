#pragma once
#include <vector>
#include <random>
using namespace std;
class Edge;
class Neuron
{
public:
	Neuron();
	void addManager(Edge* manager);
	void addSubordinate(Edge* subordinate);
	double state;
	double output;
	double derivative;
	double bias;
	double delta;
	double X;
	double Y;
	vector<Edge*>* managers;
	vector<Edge*>* subordinates;
	~Neuron();
};

