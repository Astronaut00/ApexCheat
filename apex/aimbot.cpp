#include "cheat.h"
#include "utils.h"
//TODO: Condense debug message comments. Preferably needs some actual debug Api instead of this scuffed shit (that can print vectors etc.)
static std::vector<BasePlayer> playerListCopy;
static BasePlayer lastPlayer; //the last player targetted by the aimbot
static std::deque<Vector> positions; //previous records of the player


//std::vector<BasePlayer> Cheat::ClosestPlayer(const Vector& localPlayerCameraPos, const Vector & localPlayerAngles)
//{
//	//printf("BasePlayer Cheat::ClosestPlayer\n");
//	float closestFov = -FLT_MAX;
//	std::vector<BasePlayer> closestPlayers;
//
//	//printf("\tBasePlayer& player : playerListCopy\n");
//	for (BasePlayer& player : playerListCopy)
//	{
//		if (!player.IsValid()) continue;
//		//if (!player.IsVisible()) continue;
//		//("\t\tplayer: %d\n", player.m_iName().get());
//		Vector3 aimAngle(0,0,0);
//		//Vector3 playerPos = player.m_vecAbsOrigin().get();
//		Vector3 position = player.m_vecAbsOrigin()->get();
//
//		Vector headPos;
//		if (!player.GetEntityBonePosition(BoneId::neck, &headPos))
//			continue;
//
//		float distance = headPos.Distance(localPlayerCameraPos);
//		//printf("\t\Player Pos: (%.2f, %.2f, %.2f)\n", playerPos.x, playerPos.y, playerPos.z);
//		Math::CalcAngle(localPlayerCameraPos, headPos, &aimAngle); //Calc angle to player
//		//printf("\t\tAim Angle: (%.2f, %.2f, %.2f)\n", aimAngle.x, aimAngle.y, aimAngle.z);
//
//		float fovToPlayer = Math::FovToAngle(localPlayerAngles /*regate recoil*/, aimAngle);
//		//printf("\t\tfovToPlayer: %.2f\n", fovToPlayer);
//
//		//float scaledFov = settings.fov / (distance / 10000);
//
//		player.distanceFromLocal = distance;
//		if (closestFov < fovToPlayer && fovToPlayer < settings.fov /*make sure they are in our fov*/) {		
//			//sorting player objects
//			closestPlayers.push_back(player); //reference the player inside of the vector
//			closestFov = fovToPlayer;
//		}
//	}
//
//	if (closestPlayers.size() > 0)
//		std::reverse(closestPlayers.begin(), closestPlayers.end());
//
//		//sort(closestPlayers.begin(), closestPlayers.end(), [](BasePlayer& lhs, BasePlayer& rhs)
//		//{
//		//		return lhs.m_vecAbsOrigin().get() < rhs.m_vecAbsOrigin().get() ;
//		//});
//
//		//if (closestPlayers[0].m_vecAbsOrigin().get().Distance(closestPlayers[1].m_vecAbsOrigin().get()) > 10000) {
//
//		//	sort(closestPlayers.begin(), closestPlayers.end(), [](BasePlayer& lhs, BasePlayer& rhs)
//		//	{
//		//		return lhs.m_vecAbsOrigin().get() < rhs.m_vecAbsOrigin().get() ;
//		//	});
//		//}
//
//	//printf("\tclosestFov: %.2f\n", closestFov);
//	return closestPlayers;
//}

bool Cheat::bestBone(int* bestBone, BasePlayer& player)
{
	int currentBest = -1;
	float lastFovToAngle = FLT_MAX;
	for (int i = 0; i < 39; i++)
	{
		if (i != BoneId::head && 
			i != BoneId::chest &&
			i != BoneId::Spine &&
			i != BoneId::neck &&
			i != BoneId::Right_foot &&
			i != BoneId::Left_foot &&
			i != BoneId::Right_knee &&
			i != BoneId::Left_knee)
			continue;
		Vector bonePos(0, 0, 0);
		if (!player.GetEntityBonePosition(i, &bonePos))
			continue;

		Vector boneAngle(0, 0, 0);
		Math::CalcAngle(localPlayer.m_vecCameraPos()->get(), bonePos, &boneAngle); //Calc angle to player

		if (boneAngle == Vector3(0, 0, 0))
			continue;

		float fovToAngle = Math::FovToAngle(localPlayer.m_vecViewAngles()->get(), boneAngle);

		if (fovToAngle < settings.fov && lastFovToAngle > fovToAngle)
		{
			currentBest = i;
			lastFovToAngle = fovToAngle;
		}
	}

	*bestBone = currentBest;
	return currentBest >= 0;
}

//sorts the entities by distance and then cuts off the largest delta from the search; after it then finds the closest enemy to the FOV
static std::map<uint64_t, float> visible_times;
bool Cheat::bestPlayer(BasePlayer * bestplayer)
{

	std::vector<BasePlayer> closestPlayers = playerListCopy;

	//printf("\tBasePlayer& player : playerListCopy\n");

	//store fovs to each player and also cut out players that are not in my fov
	float closestFov = -FLT_MAX;
	std::vector<BasePlayer> playersInsideFov; //culling the players outside of the fov
	for (BasePlayer& player : closestPlayers) {
		if (!player.IsValid()) 
			continue;


		Vector3 aimAngle(0, 0, 0);
		Vector3 position = player.m_vecAbsOrigin()->fromCache();

		Vector headPos;
		if (!player.GetEntityBonePosition(BoneId::head, &headPos))
			continue;

		float distance = headPos.Distance(localPlayer.m_vecCameraPos()->get());
		float bullet_speed = localPlayer.GetActiveWeapon()->m_flBulletSpeed()->get();
		float time_to_hit = distance / bullet_speed;
		if (time_to_hit > 1.f)
			continue;

		float* oVisTime = &visible_times[player.base];
		if (!player.IsVisible(oVisTime))
			continue;

		if (player.IsKnocked())
			continue;

		Math::CalcAngle(localPlayer.m_vecCameraPos()->fromCache(), headPos, &aimAngle); //Calc angle to player

		float fovToPlayer = Math::FovToAngle(localPlayer.m_vecViewAngles()->get(), aimAngle);

		/*Save for later*/
		player.fromLocalFov = fovToPlayer; 
		player.distanceFromLocal = distance;

		if (fovToPlayer < settings.fov && distance != 0) {
			playersInsideFov.push_back(player); //reference the player inside of the vector 
		}
	}

	if (playersInsideFov.size() > 0){

		sort(playersInsideFov.begin(), playersInsideFov.end(), [](BasePlayer& lhs, BasePlayer& rhs) {
			return lhs.fromLocalFov < rhs.fromLocalFov;
			});

		*bestplayer = playersInsideFov[0];
		return true;
	}


	//find closest to fov
	return false;

}


//static unsigned int counter = 0;
Vector3 extrapolatePosition(Vector position, Vector velocity, float seconds) {

		return position + velocity * seconds;
	//if (lastPlayer.IsValid())
	//{
	//	if (counter > 0 && counter % millisecondsToReach) { //save a record of the player every millisecond to reach

	//		if (positions.size() > 10) {
	//			positions.pop_back(); //20 records is too much, pop them
	//		}

	//		positions.push_back(lastPlayer.m_vecAbsOrigin()->get());
	//	}
	//	counter++;
	//}
	//else
	//{
	//	positions.clear(); //reset for the new lastPlayer
	//	counter = 1; //reset counter
	//}

	//Vector sumOfDelta(0, 0, 0);
	//int deltaAmount = positions.size() - 1;
	//for (int i = 0; i < deltaAmount /*so that we get all records*/; i++) {

	//	Vector3 delta = positions[i] - positions[i + 1];
	//	sumOfDelta += delta;
	//}
	//sumOfDelta /= deltaAmount;
	//
	//return currentPos - sumOfDelta; //apply the delta
};

//Vector3 extrapolatePosition(BasePlayer & player, float seconds)
//{
//	auto player_origin = player.m_vecAbsOrigin()->get();
//	auto player_velocity = player.m_vecAbsVelocity()->get();
//	return player_origin + player_velocity * seconds;
//}


void Cheat::AimAtBone(Vector localPlayerCameraPos, BasePlayer& target, int bone)
{
	Vector aimPos(0, 0, 0);
	Vector3 aimAngle(0, 0, 0);

	float projectileSpeed = localPlayer.GetActiveWeapon()->m_flBulletSpeed()->get(); // units/s
	float bulletGravity = localPlayer.GetActiveWeapon()->m_flBulletGravity()->get();
	Vector3 playerAngles = localPlayer.m_vecViewAngles()->get();
	float bulletVelocity = projectileSpeed / 10000.f; // units/ms

	float distance = localPlayer.m_vecAbsOrigin()->get().Distance(target.m_vecAbsOrigin()->get());
	float bulletTravelTime = distance / projectileSpeed;
	//printf("bullet travel time: %.2f\n", bulletTravelTime);
	float pitchToHit = .5 * std::asinf((bulletGravity * distance) / powf(projectileSpeed, 2));

	if (target.GetEntityBonePosition(bone, &aimPos)) {

		Vector3 aimPosExtrapolated = extrapolatePosition(aimPos, target.m_vecAbsVelocity()->get(), bulletTravelTime); //extrapolate the position
		float extrapolation_distance = aimPos.Distance(aimPosExtrapolated);


		//if (extrapolation_distance > 0)
			//printf("extrapolated by: %.2f units\n", extrapolation_distance);
																													  //printf("predicted: (%.2f, %.2f, %.2f) from %d records\n", aimPos.x - aimPosExtrapolated.x, aimPos.y - aimPosExtrapolated.y, aimPos.z - aimPosExtrapolated.z, positions[target.m_iName()->get()].size());
		Math::CalcAngle(localPlayerCameraPos, aimPosExtrapolated, &aimAngle);
		float fovToAngle = Math::FovToAngle(playerAngles, aimAngle);
		//printf("original pitch: %.2f\nnet pitch: %.2f\n\n", aimAngle.x, pitchToHit);


		aimAngle.x -= RAD2DEG(pitchToHit);//adjust pitch
		aimAngle -= localPlayer.m_vecAimPunch()->get(); //negate recoil

		Vector3 smoothedAngle(0, 0, 0);
		//printf("Viewangles before: %.2f, %.2f, %.2f\n", playerAngles.x, playerAngles.y, playerAngles.z);
		//printf("AimAngles: %.2f, %.2f, %.2f\n\n", aimAngle.x, aimAngle.y, aimAngle.z);

		switch (settings.aimbotMode)
		{
			case AimbotModes::TYPE_LINEAR:
			{
				Math::LinearSmooth(playerAngles, aimAngle, &smoothedAngle, settings.smoothing / 100.f);

				if ((int)playerAngles.y == (int)smoothedAngle.y 
					&& (int)playerAngles.x == (int)smoothedAngle.x)
					return;
				break;
			}
			case AimbotModes::TYPE_QUADTRATIC:
			{
				Math::QudraticSmoothAssist(playerAngles, aimAngle, &smoothedAngle, settings.smoothing);


				if ((int)playerAngles.y == (int)smoothedAngle.y 
					&& (int)playerAngles.x == (int)smoothedAngle.x)
					return;
				break;
			}
			case AimbotModes::TYPE_OFF:
			{
				Math::NormalizeAngles(&aimAngle);
				smoothedAngle = aimAngle;
				//do silent aim
				break;
			}
		}
		smoothedAngle.z = 0.f;
		if (smoothedAngle == Vector(0, 0, 0))
			return;

		localPlayer.m_vecViewAngles()->set(smoothedAngle);
	}
}

void Cheat::Aimbot()
{
	static bool aimKeyWasDown = false;

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	while (true) {
		if (clientState.signOnState() != SignonState::Full || !localPlayer.IsValid() || settings.aimbotMode == AimbotModes::TYPE_OFF) {
			playerListCopy.clear();
			std::this_thread::sleep_for(5s);
			continue;
		}

		playerListCopy = playerList; //copy list

		bool aimKeyDown = keydown(VK_LBUTTON);
		auto weapon = localPlayer.GetActiveWeapon();

		if (aimKeyDown && weapon->m_ammoInClip()->get() > 0 && 
			!weapon->m_bInReload()->get())
		{ 

			Vector localPlayerCameraPos = localPlayer.m_vecCameraPos()->get();
			Vector localPlayerAngles = localPlayer.m_vecViewAngles()->get();

			float* oVisTime = &visible_times[lastPlayer.base];
			bool attackLast = false;
			if (lastPlayer.IsValid() && lastPlayer.IsVisible(oVisTime))
			{
				Vector angleToEnemy(0, 0, 0);
				Vector3 position(0, 0, 0);
				if (lastPlayer.GetEntityBonePosition(BoneId::head, &position))
				{
					Math::CalcAngle(localPlayerCameraPos, position, &angleToEnemy);
					float fovToAngle = Math::FovToAngle(localPlayerAngles, angleToEnemy);

					if (fovToAngle < settings.fov)
						attackLast = true;
				}
			}

			if (attackLast)
			{ //no need to look for someone new

				int targetBone = 0;
				if (!bestBone(&targetBone, lastPlayer))
				{
					printf("failed to find best Bone\n");
					return;
				}

				AimAtBone(localPlayerCameraPos, lastPlayer, targetBone);

			}
			else 
			{
				
				BasePlayer closestPlayer;
				if (bestPlayer(&closestPlayer)) 
				{
					int targetBone = 0;
					if (!bestBone(&targetBone, closestPlayer))
					{
						printf("failed to find best Bone\n");
						return;
					}

					AimAtBone(localPlayerCameraPos, closestPlayer, targetBone);
					lastPlayer = closestPlayer;
				}
				else
				{
					printf("Failed to find Best Player\n");
				}
				aimKeyWasDown = true;
			}

			if (settings.aimbotMode != AimbotModes::TYPE_NONE)
				Utl::DelayExeuction(rand() % 100 + 5e+3);
			else
				Utl::DelayExeuction(1);
			continue;
		}
		else 
		{
			lastPlayer = BasePlayer(0); //null our class so we arent valid
			aimKeyDown = false;
		}
		std::this_thread::sleep_for(4ms);
	}
}
