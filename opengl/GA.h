#pragma once

#include <algorithm>
#include <random>
#include <numeric>
#include <vector>
#include "motor.h"
using namespace std;
/*
void DrawCart(const float x, const float y)
{
	float height = 0.3, width = 0.3;
	glRectf(x - width / 2, y + height / 2, x + width / 2, y - height / 2);
}
void DrawPole(const float x, const float y, const float theta)
{
	float length = 0.5;
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x + sin(theta)*length, y + cos(theta)*length);         //直线结束坐标
	glEnd();
}
*/
template <typename T>
vector<size_t> sort_indexes_e(vector<T> &v)
{
	vector<size_t> idx(v.size());
	iota(idx.begin(), idx.end(), 0);
	sort(idx.begin(), idx.end(),
		[&v](size_t i1, size_t i2) {return v[i1] > v[i2]; });
	return idx;
}

class GA
{
public:
	GA(motor& motor, const size_t nDNA, const size_t nPopulation,
		const double mutation = 0.2):nPopulation(nPopulation), nDNA(nDNA), mutation(mutation)
	{
		std::uniform_real_distribution<double> u(-100.0, 100.0);

		mother.resize(nDNA);
		father.resize(nDNA);
		son.resize(nPopulation);

		for (auto &individual : son)
		{
			individual.resize(nDNA);
				
			individual[0] = std::uniform_real_distribution<double>(0, 1)(engine);//2
			individual[1] = std::uniform_real_distribution<double>(0, 0.0001)(engine);//0.0001
			individual[2] = std::uniform_real_distribution<double>(0, 1)(engine);//5
		}

		std::vector<double> reward(nPopulation);
		for (size_t i = 0; i != nPopulation; ++i)
		{
			reward[i] = evalue(motor, son[i]);
		}
		std::vector<size_t> index = sort_indexes_e(reward);
		father = son[index[0]];
		mother = son[index[1]];
	}
	void multiply(void)
	{
		std::uniform_real_distribution<double> u(0, 1.0);
		std::uniform_int_distribution<size_t> ui(0, nDNA - 1);
		//std::default_random_engine engine(time(0));

		son[0] = father;
		son[1] = mother;
		for (size_t i = 2; i != nPopulation; ++i)
		{
			if (u(engine) > 0.5)
				son[i] = father;
			else
				son[i] = mother;
			double pos = ui(engine);
			son[i][pos] = std::uniform_real_distribution<double>(min(father[pos],mother[pos]),
																 max(father[pos],mother[pos]))(engine);
		}
		for (auto &individual : son)
		{
			if (u(engine) < mutation)
			{//for (auto &gene : individual)gene += std::uniform_real_distribution<double>(-0.1, 0.1)(engine);
				individual[0] += std::uniform_real_distribution<double>(-0.001, 0.001)(engine);
				individual[1] += std::uniform_real_distribution<double>(-0.0001, 0.0001)(engine);
				individual[1] += std::uniform_real_distribution<double>(-0.001, 0.001)(engine);
			}
		}
	}
	//double adjust(std::vector<double> observation, std::vector<double> individual)
	//{
	//	double sum = 0.0;
	//	for (size_t i = 0; i != observation.size(); ++i)
	//	{
	//		sum += observation[i] * individual[i];
	//	}
	//	return sum;
	//}
	double evalue(motor& motor,std::vector<double> &individual)
	{
		//CartPole env;
		motor.reset();
		double reward = 0.0;
		while (motor.update(individual[0],individual[1],individual[2]))
		{
			reward += 1;
			if(reward >= 10000) break;
		}
		cout << "evalue:" << reward << "\n";
		return reward;
	}
	void evolution(motor& motor)
	{
		multiply();
		std::vector<double> reward(nPopulation);
		for (size_t i = 0; i != nPopulation; ++i)
		{
			reward[i] = evalue(motor,son[i]);
		}
		std::vector<size_t> index = sort_indexes_e(reward);
		father = son[index[0]];
		mother = son[index[1]];
	}

	std::default_random_engine engine;
	size_t nDNA, nPopulation;
	double mutation;
	std::vector<double> mother, father;
	std::vector<std::vector<double>> son;
};