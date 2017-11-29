#ifndef __ZOMBIE_H_
#define __ZOMBIE_H_

#include "BouncyBall.h"
#include "Wall.h"

namespace Simplex 
{
	class Zombie : public MyEntity
	{
	public :
		Zombie();
		void Initialize(vector3 position);

		void Resolve(BouncyBall* other);
		void Resolve(Wall* other);
		void Resolve(Zombie* other);
	private:
		const String FILE_PATH = "Minecraft\\Zombie.obj";
		String defaultID = "Zombie";
	};
}
#endif