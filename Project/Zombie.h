#ifndef __ZOMBIE_H_
#define __ZOMBIE_H_

#include "MyEntity.h"

namespace Simplex 
{
	class Zombie : public MyEntity
	{
	public :
		Zombie();
		void Initialize(vector3 position);
	private:
		const String FILE_PATH = "Minecraft\\Zombie.obj";
		String defaultID = "Zombie";
	};
}

#endif