#ifndef __BOUNCYBALL_H_
#define __BOUNCYBALL_H_

#include "Wall.h"

namespace Simplex
{
	class BouncyBall : public MyEntity
	{
	public:
		BouncyBall();
		void Initialize(vector3 position, vector3 forward, float speed);

		void Resolve(BouncyBall* other);
		void Resolve(Wall* other);
	
		void Update() override;
	private:
		const String FILE_PATH = "BouncyBall.obj";
		String defaultID = "Ball";

		const float FLOOR_POS = 0.0f;

		float radius;
	};
}

#endif