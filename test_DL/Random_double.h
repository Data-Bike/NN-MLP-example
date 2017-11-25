#pragma once
#include <random>
using namespace std;
class Random_double
{
public:
	Random_double();
	static random_device rd;
	static mt19937 gen;
	static uniform_real_distribution<> dis;
	static double get();
	static bool setuped;
	~Random_double();
};

