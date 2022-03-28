#pragma once

#include <vector>
#include <random>

struct Interval
{
	double min, max;
};

struct Piece
{
	Piece() {}
	Piece(double min, double max) :
		intervals({ {min, max} })
	{}

	std::vector<Interval> intervals;
};

class Agent
{
public:
	static std::uniform_real_distribution<double> coefficientRange;
	static std::default_random_engine generator;

public:
	Agent(unsigned int polynomialDegree);
	~Agent();

	double operator()(const Piece& piece);
	double Eval(double x);

private:
	std::vector<double> coefficients;
	double normalizationFactor;
};
