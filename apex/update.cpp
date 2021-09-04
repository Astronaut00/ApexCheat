#include "cheat.h"
#include "utils.h"

void Cheat::UpdateItems()
{
	while (true)
	{
		//printf("sigOnState: %d\n", clientState.signOnState());

		//float bullet_speed = localPlayer.GetActiveWeapon()->m_flBulletSpeed()->get();
		//std::cout << "m_flBulletSpeed: " << bullet_speed << std::endl;

		//float bullet_gravity = localPlayer.GetActiveWeapon()->m_flBulletGravity()->get();
		//std::cout << "m_flBulletGravity: " << bullet_gravity << std::endl;

		//--For updating the playerList--//
		if (clientState.signOnState() != SignonState::Full && settings.itemName || settings.itemGlow) {
			playerList.clear();
			std::this_thread::sleep_for(5s);
			continue;
		}

		std::vector<BasePlayer> newPlayerList;
		for (int i = 0; i < 65553; i++) {

			uint64_t entityPointer;

			if (!pMem->Rpm(pMem->base + OFFSET_ENTITYLIST + (i * 0x20),
				&entityPointer,
				sizeof(uint64_t)) ||
				!entityPointer)
				continue;


			BasePlayer player(entityPointer);
			char apexName[255];
			__int64 apexNamePoint = player.m_iSignifierName()->get();//
			if (apexNamePoint < 1000000) continue;
			pMem->Rpm(apexNamePoint, apexName, 32);//

			std::cout << std::string(apexName) << std::endl;

			if (player == localPlayer)
				continue;

	/*		if (player.m_iTeamNum()->fromCache() ==
				localPlayer.m_iTeamNum()->fromCache())
				continue;*/

			if (player.IsValid())
				newPlayerList.push_back(player);

			//Utl::DelayExeuction(10);
		}


		//for (int i = 0; i < 65553; i++)
		//{
		//	uint64_t entityPointer;

		//	if (!pMem->Rpm(pMem->base + OFFSET_ENTITYLIST + (i * 0x20),
		//		&entityPointer,
		//		sizeof(uint64_t)) ||
		//		!entityPointer)
		//		continue;


		//	BasePlayer player(entityPointer);

		//	
		//}

		//for (auto& player : newPlayerList)
		//{
		//	printf("player: %d ", player.m_iName()->fromCache());
		//	printf("health: %d ", player.m_iHealth()->fromCache());
		//	printf("team: %d\n", player.m_iTeamNum()->fromCache());
		//}

		//printf("\n\n");

		playerList = newPlayerList;
		std::this_thread::sleep_for(7s);
		//---------------------------------//
	}
}


void Cheat::UpdatePlayers()
{
	while (true)
	{
		//printf("sigOnState: %d\n", clientState.signOnState());

		//float bullet_speed = localPlayer.GetActiveWeapon()->m_flBulletSpeed()->get();
		//std::cout << "m_flBulletSpeed: " << bullet_speed << std::endl;

		//float bullet_gravity = localPlayer.GetActiveWeapon()->m_flBulletGravity()->get();
		//std::cout << "m_flBulletGravity: " << bullet_gravity << std::endl;

		//--For updating the playerList--//
		if (clientState.signOnState() != SignonState::Full) {
			playerList.clear();
			std::this_thread::sleep_for(5s);
			continue;
		}

		localPlayer = BasePlayer(pMem->Rpm<uint64_t>(pMem->base + OFFSET_LOCAL_ENT));

		std::vector<BasePlayer> newPlayerList;
		for (int i = 0; i < 128; i++) {

			uint64_t entityPointer;

			if (!pMem->Rpm(pMem->base + OFFSET_ENTITYLIST + (i * 0x20),
				&entityPointer,
				sizeof(uint64_t)) ||
				!entityPointer)
				continue;


			BasePlayer player(entityPointer);
			//char apexName[255];
			//__int64 apexNamePoint = player.m_iSignifierName()->get();//
			//if (apexNamePoint < 1000000) continue;
			//pMem->Rpm(apexNamePoint, apexName, 32);//

			//std::cout << std::string(apexName) << std::endl;

			if (player == localPlayer)
				continue;

			//if (player.m_iTeamNum()->fromCache() == 
			//	localPlayer.m_iTeamNum()->fromCache())
			//	continue;

			if (player.IsValid())
				newPlayerList.push_back(player);

			//DelayExeuction(1000000);
		}


		//for (int i = 0; i < 65553; i++)
		//{
		//	uint64_t entityPointer;

		//	if (!pMem->Rpm(pMem->base + OFFSET_ENTITYLIST + (i * 0x20),
		//		&entityPointer,
		//		sizeof(uint64_t)) ||
		//		!entityPointer)
		//		continue;


		//	BasePlayer player(entityPointer);

		//	
		//}

		//for (auto& player : newPlayerList)
		//{
		//	printf("player: %d ", player.m_iName()->fromCache());
		//	printf("health: %d ", player.m_iHealth()->fromCache());
		//	printf("team: %d\n", player.m_iTeamNum()->fromCache());
		//}

		//printf("\n\n");

		playerList = newPlayerList;
		std::this_thread::sleep_for(7s);
		//---------------------------------//
	}
}