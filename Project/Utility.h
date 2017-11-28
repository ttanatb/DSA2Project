#ifndef __UTILITY_H_
#define __UTILITY_H_

#include <cstdlib>
#include <ctime>

#include "Simplex\System\Definitions.h"

namespace Simplex
{
	class Random
	{
	private:
		static Random* m_pInstance;

		Random();
		~Random();
	public:
		vector3 Next(vector3 min, vector3 max);
		float Next(float min, float max);
		float Next(float max);
		float Next();

		static Random* GetInstancce(void);
		static void ReleaseInstance(void);
	};
}

#endif