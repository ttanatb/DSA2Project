#ifndef __WALL_H_
#define __WALL_H_

#include "MyEntity.h"

namespace Simplex
{
	class Wall : public MyEntity
	{
	public:
		Wall();
		void Initialize(vector3 position, bool isLeft);
	private:
		const String FILE_PATH = "Wall.obj";
		String defaultID = "Wall";
	};
}

#endif