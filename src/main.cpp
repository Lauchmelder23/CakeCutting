#include <iostream>
#include <iomanip>
#include <Eigen/Dense>

#include "Agent.hpp"


int main(int argc, char** argv)
{
	constexpr size_t numAgents = 5;
	std::vector<Agent> agents;
	std::vector<Piece> initialPartition(numAgents);

	for (size_t k = 0; k < numAgents; k++)
	{
		agents.emplace_back(5);
		initialPartition[k] = Piece((double)k / numAgents, (double)(k + 1) / numAgents);
	}

	std::cout << "Initialized " << numAgents << " random agents and created the following partition: " << std::endl;
	for (const Piece piece : initialPartition)
		std::cout << "   [" << piece.intervals[0].min << ", " << piece.intervals[0].max << "]" << std::endl;
	std::cout << std::endl;

	Eigen::MatrixXd M(numAgents, numAgents);
	for (size_t i = 0; i < numAgents; i++)
		for (size_t j = 0; j < numAgents; j++)
			M(i, j) = agents[i](initialPartition[j]);

	std::cout << "M = " << std::endl << M << std::endl;

	M = M.inverse();
	std::cout << "M^-1 = " << std::endl << M << std::endl;

	double t = M.minCoeff();

	double delta = (double)(numAgents - 1) / (numAgents * (1.0 - t * numAgents));
	std::cout << "delta = " << delta << std::endl;

	Eigen::MatrixXd N(numAgents, numAgents);
	for (size_t i = 0; i < numAgents; i++)
		for (size_t j = 0; j < numAgents; j++)
			N(i, j) = (i == j) ?
				1.0 / (numAgents + delta) :
				1.0 / numAgents - delta / (numAgents - 1);

	std::cout << "N = " << std::endl << N << std::endl;

	Eigen::MatrixXd R = M * N;
	std::cout << "R = " << std::endl << R << std::endl;

	std::vector<Piece> intermediatePartition(numAgents);

	double epsilon = delta / (double)(numAgents * numAgents);
	for (size_t j = 0; j < numAgents; j++)
	{
		double divisor = R.row(j).sum();
		double min = initialPartition[j].intervals[0].min;
		double len = initialPartition[j].intervals[0].max - initialPartition[j].intervals[0].min;

		for (size_t n = 0; n < numAgents; n++)
		{
			double ratio = R(j, n) / divisor;
			intermediatePartition[j].intervals.push_back({ min, min + len * ratio });
			min += len * ratio;
		}

	}

	std::vector<Piece> finalPartition(numAgents);
	for (size_t i = 0; i < numAgents; i++)
		for (size_t n = 0; n < numAgents; n++)
			finalPartition[i].intervals.push_back(intermediatePartition[n].intervals[i]);

	std::cout << "FINAL PARTITION" << std::endl;
	std::cout << "====================================================" << std::endl;
	for (size_t n = 0; n < numAgents; n++)
	{
		std::cout << "Agent " << n << ": {" << std::endl;
		for (size_t k = 0; k < numAgents; k++)
			std::cout << "    [" << std::setprecision(6) << std::fixed << finalPartition[n].intervals[k].min << ", " << std::setprecision(6) << std::fixed << finalPartition[n].intervals[k].max << "]" << std::endl;

		std::cout << "}, mu = " << agents[n](finalPartition[n]);

		std::cout << " (";
		for (size_t k = 0; k < numAgents; k++)
			if (k != n)
				std::cout << agents[n](finalPartition[k]) << ", ";
		
		std::cout << ")" << std::endl;
	}

	return 0;
}
