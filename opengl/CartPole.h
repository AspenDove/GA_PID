#pragma once
#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>

#define PI 3.1415926

float sgn(float x)
{
	return (x > 0) - (x < 0);
}

class CartPole
{
public:
	float x, v, a;
	float theta, omiga, alpha;
	float miuc, miup;
	float mc, mp;
	float l;
	float g;
	float t, T;

	std::uniform_real_distribution<float> x0, theta0;

	CartPole(void) :x0(std::uniform_real_distribution<float>(-1.f, 1.f)),
		theta0(std::uniform_real_distribution<float>(-0.18f, 0.18f))
	{

		//std::uniform_real_distribution<float> x0(-0.5f, 0.5f);
		//std::uniform_real_distribution<float> theta0(-0.4f, 0.4f);

		x = x0(engine);
		theta = theta0(engine);
		reset();
	}
	void reset(void)
	{
		std::default_random_engine engine(time(0));

		x = x0(engine);
		theta = theta0(engine);

		v = 0.0, a = 0.0;
		omiga = 0.0, alpha = 0.0;

		miuc = 5e-4, miup = 2e-6;
		mc = 1.0, mp = 0.1;
		l = 0.5;
		g = 9.8;
		t = 0.02;
		T = 0.02;
	}
	bool update(double F)
	{
		//F = std::max(-10.f, std::min(10.f, F));
		//t += T;
	//	double temp = (F + mp*l * omiga * omiga  *sin(theta)) / (mc + mp);
	//	alpha = (g * sin(theta) - cos(theta)* temp) / (l * (4.0 / 3.0 - mp * cos(theta) * cos(theta) / (mc + mp)));
	//	a = temp - mp*l* alpha * cos(theta) / (mc + mp);

		alpha = (g*sin(theta) + cos(theta)*(-F - mp*l*omiga*omiga*sin(theta) + miuc*sgn(v)) / (mc + mp) - miup*omiga / (mp*l))
			/ (l*(4. / 3. - (mp*cos(theta)*cos(theta) / (mc + mp))));

		a = (F + mp*l*(omiga*omiga*sin(theta) - alpha*cos(theta)) - miuc*sgn(x)) / (mc + mp);
		
		x += v*t;
		v += a*t;
		theta += omiga*t;
		omiga += alpha*t;

		return (abs(x) <= 2.4) && (abs(theta) <= (15 * PI / 180));
	}
	std::default_random_engine engine;
};