#include "Agent.hpp"

#include <chrono>

std::uniform_real_distribution<double> Agent::coefficientRange = std::uniform_real_distribution<double>(-1.0f, 1.0f);
std::default_random_engine Agent::generator = std::default_random_engine(std::random_device{}());

Agent::Agent(unsigned int polynomialDegree) :
	normalizationFactor(0.0f)
{
	for (size_t k = 0; k < polynomialDegree; k++)
	{
		double coefficient = coefficientRange(generator) / (k + 1);

		coefficients.push_back(coefficient);
		normalizationFactor += coefficient;
	}

	normalizationFactor = 1.0f / normalizationFactor;
}

Agent::~Agent()
{
}

double Agent::operator()(const Piece& piece)
{
	double value = 0.0f;
	for(const Interval& interval : piece.intervals)
		value += Eval(interval.max) - Eval(interval.min);

	return value;
}

double Agent::Eval(double x)
{
	double value = 0.0f;
	for (size_t k = 0; k < coefficients.size(); k++)
		value += coefficients[k] * pow(x, k + 1);

	return normalizationFactor * value;
}
