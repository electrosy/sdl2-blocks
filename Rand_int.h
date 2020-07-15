#ifndef RAND_INT_H
#define RAND_INT_H

#include <iostream>
#include <iomanip>
#include <random>

// type of distribution
namespace ley{

	class Rand_int
	{
	
	private:
		std::random_device rd;
		std::mt19937 gen;
		std::uniform_int_distribution<> dis;
		int last;
	public:
		Rand_int(int low, int high);
		~Rand_int();

	    int operator()();
	};

/* Operators */
/*
std::ostream& operator<<(std::ostream& os, const Rand_int& ri);
*/
}
#endif