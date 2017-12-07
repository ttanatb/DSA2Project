#ifndef __ZOMBIE_H_
#define __ZOMBIE_H_

#include "BouncyBall.h"
#include "Wall.h"
#include "ScoreManager.h"

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

		void Update() override;
		bool m_bIsColliding = false;

		void SetRBMatrix(matrix4 a_m4ToWorld);
    ScoreManager* m_pScoreManager = nullptr; //Score Manager

	private:
		const String FILE_PATH = "Minecraft\\Zombie.obj";
		String defaultID = "Zombie";

		MyRigidBody* m_pHeadRB  =  nullptr;
		MyRigidBody* m_pLArmRB  =  nullptr;
		MyRigidBody* m_pRArmRB  =  nullptr;
		MyRigidBody* m_pTorsoRB =  nullptr;
		MyRigidBody* m_pLegsRB  =  nullptr;
	};
}
#endif