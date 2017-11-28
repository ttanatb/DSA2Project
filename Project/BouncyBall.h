#ifndef __BOUNCYBALL_H_
#define __BOUNCYBALL_H_

#include "MyEntity.h"

namespace Simplex
{
	class BouncyBall : public MyEntity
	{
	public:
		BouncyBall();
		void Initialize(vector3 position, vector3 forward);
	private:
		const String FILE_PATH = "Planets\\09_Pluto.obj";
		String defaultID = "Ball";
	};
}

#endif