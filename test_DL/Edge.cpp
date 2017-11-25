#include "stdafx.h"
#include "Edge.h"
#include <random>
#include <time.h>
#include <chrono>
#include "Random_double.h"
Edge::Edge(Neuron* left, Neuron* right)
{
	
	this->weight = Random_double::get();
	this->d_weight = 0.0;
	this->left = left;
	this->right = right;
	right->addManager(this);
	left->addSubordinate(this);
}


Edge::~Edge()
{
}
