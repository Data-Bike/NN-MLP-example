#include "stdafx.h"
#include "Random_double.h"

bool Random_double::setuped = false;
random_device Random_double::rd;
mt19937 Random_double::gen = mt19937(Random_double::rd());
uniform_real_distribution<> Random_double::dis = uniform_real_distribution<>(-1, 1);

Random_double::Random_double()
{
	if (!Random_double::setuped) {
		setuped = true;
	}
}

double Random_double::get() {
	return Random_double::dis(Random_double::gen);
}

Random_double::~Random_double()
{
}
