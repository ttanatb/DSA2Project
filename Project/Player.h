#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "MyEntity.h"

namespace Simplex
{
	class Player 
	{
	private:
		const String CROSSHAIR_FILE_PATH = "crosshair.obj";
		MyEntity* m_pCrosshair = nullptr;
	public:
		Player();
		~Player();

		void Update(vector3 camPos, vector3 camForward);
		void Render();

		//To-do
		//crosshair
		//get a model, add it to the renderlist
		//render function passes camera quaternion as argument to position crosshair correctly

		//gun model
		//add it to render list similarly to crosshair (position it bottom-right)

		//recharge bar model (a rectangle with the pivot to the left-side of the bar)
		//add it to renderlist and place it to bottom-left
		//scale the bar relatively to energy/maxEnergy
		//render another bar that's slightly larger and behind as the background of the recharge bar

		//health, score, energy, maxEnergy
		//function to reduce health
		//function to increment score
		//function to reduce energy if it is above the threshold
	};
}

#endif