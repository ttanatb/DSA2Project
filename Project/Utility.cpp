#include "Utility.h"
using namespace Simplex;
Random* Random::m_pInstance;

Simplex::Random::Random()
{
	srand(static_cast <unsigned> (time(0)));
}

Simplex::Random::~Random()
{
	ReleaseInstance();
}

vector3 Simplex::Random::Next(vector3 min, vector3 max)
{
	return vector3(Next(min.x, max.x), Next(min.y, max.y), Next(min.z, max.z));
}

float Simplex::Random::Next(float min, float max)
{
	return min + static_cast<float>(std::rand() / (static_cast<float>(RAND_MAX / (max - min))));
}

float Simplex::Random::Next(float max)
{
	return static_cast <float> (std::rand() / (static_cast<float>(RAND_MAX / max)));
}

float Simplex::Random::Next()
{
	return static_cast<float> (std::rand()) / static_cast <float> (RAND_MAX);
}

Random * Simplex::Random::GetInstancce(void)
{
	if (m_pInstance == nullptr)
		m_pInstance = new Random();

	return m_pInstance;
}

void Simplex::Random::ReleaseInstance(void)
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
