#include "stdafx.h"
#include "MachineLearning.h"
#include <iostream>

MachineLearning::MachineLearning(int levels_count,...)
{
	va_list args;
	va_start(args, levels_count);
	this->alfa = 0.0;
	this->eta = 1.0;
	this->levels = new vector<vector<Neuron*>*>();
	vector<Neuron*>* current_level = NULL;
	vector<Neuron*>* before_level = NULL;

	for (int level_num = 0; level_num < levels_count; level_num++) {

		int neurons_count = va_arg(args, int);
		vector<Neuron*>* before_level = current_level;
		current_level = new vector<Neuron*>;

		this->levels->insert(this->levels->end(), current_level);

		for (int neuron_num = 0; neuron_num < neurons_count; neuron_num++) {

			Neuron* current_neuron = new Neuron();

			current_level->insert(current_level->end(), current_neuron);

			if (level_num > 0) {
				for (Neuron* before_level_neuron : *before_level) {

					Edge* current_edge = new Edge(before_level_neuron, current_neuron);
				}
			}

		}
	}
}
double MachineLearning::sigma(double val) {
	double s = 1.0 / (1.0 + exp(- 1* val));
	//double s = tanh(val);
	//double s = max(0,val);
	//double s = val;
	
	return s;
}
double MachineLearning::dsigma(double val) {
	double s = 1.0 * this->sigma(val)*(1 - this->sigma(val));
	//double s = (1-pow(this->sigma(val),2));
	//double s = 1*((val>=0)?1:0);
	//double s = 1;
	//s = (val == 0) ? 0 : s;

	return s;
}

double MachineLearning::epsilon(vector<double> output, vector<double> expected) {
	double eps = 0;
	vector<double>::iterator t_i = expected.begin();
	for (double st : output) {
		double t = *t_i;
		eps += pow((st-t),2);
		t_i++;
	}

	return eps;
}

double MachineLearning::assert(vector<double> input, vector<double> expected) {
	vector<double> output = this->setInput(input);

	vector<Neuron*> output_neuron = *this->levels->back();
	vector<double>::iterator t_it = expected.begin();
	//set delta for output level 
	for (Neuron* neuron : output_neuron) {
		double t = *t_it;
		neuron->delta = neuron->derivative*(t - neuron->output);
		t_it++;
	}

	double eps = this->epsilon(output, expected);


	
	//set delta for hidden levels
	double a = this->alfa;
	double n = this->eta;
	for (auto level_rit = next(this->levels->rbegin()); level_rit != prev(this->levels->rend()); level_rit++) {
		vector<Neuron*>* level = *level_rit;
		for (Neuron* neuron_current : *level) {
			neuron_current->delta = 0.0;
			for (Edge* edge_current : *neuron_current->subordinates) {
				neuron_current->delta += edge_current->right->delta*edge_current->weight;
			}
			neuron_current->delta *= neuron_current->derivative;
		}
	}

	//set weights&biases
	for (auto level_rit = this->levels->rbegin(); level_rit != prev(this->levels->rend()); level_rit++) {
		vector<Neuron*>* level = *level_rit;
		for (Neuron* neuron_current : *level) {
			double d = neuron_current->delta;
			for (Edge* edge_current : *neuron_current->managers) {
				double dw_before_current = edge_current->d_weight;
				double o = edge_current->left->output;
				double dw_current = a*dw_before_current + (1 - a)*n*d*o;
				edge_current->d_weight = dw_current;
				edge_current->weight += dw_current;
			}
			neuron_current->bias += n*d;
		}
	}

	return eps;
}
	
vector<double> MachineLearning::setInput(vector<double> input) {
	vector<Neuron*> input_level = *this->levels->front();
	vector<double>::iterator input_it = input.begin();
	for (Neuron* neuron_current : input_level) {
		double input_current = *input_it;
		neuron_current->state = input_current;
		neuron_current->bias = 0;
		neuron_current->output = neuron_current->state;
		input_it++;
	}
	return this->compute();
}

vector<double> MachineLearning::compute() {
	for (auto level_it = next(this->levels->begin()); level_it != this->levels->end();level_it++) {
		vector<Neuron*>* level = *level_it;
		for (Neuron* current_neuron : *level) {
			double s = 0;
			for (Edge * current_manager : *current_neuron->managers) {
				double x = current_manager->left->output;
				double w = current_manager->weight;
				s += x*w;
			}
			current_neuron->state = s + current_neuron->bias;
			current_neuron->output = this->sigma(current_neuron->state);
			current_neuron->derivative = this->dsigma(current_neuron->state);
		}
	}
	vector<double> output;
	for (Neuron* neuron_output: *this->levels->back()) {
		output.insert(output.end(), neuron_output->output);
	}
	return output;
}

MachineLearning::~MachineLearning()
{
}
