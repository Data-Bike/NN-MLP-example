#include "stdafx.h"
#include "Neuron.h"
#include <chrono>
#include "Random_double.h"

Neuron::Neuron()
{

	

	this->managers = new vector<Edge*>;
	this->subordinates = new vector<Edge*>;
	this->bias = Random_double::get();
	this->state = 0.0;
	this->output = 0.0;
	this->derivative = 0.0;
	this->delta = 0.0;
}

void Neuron::addManager(Edge* manager) {
	this->managers->insert(this->managers->end(), manager);
}
void Neuron::addSubordinate(Edge* subordinate) {
	this->subordinates->insert(this->subordinates->end(), subordinate);
}

Neuron::~Neuron()
{
}
