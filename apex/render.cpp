#include "cheat.h"
#include "render.hpp"
#include "utils.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <d3d11.h>

static std::vector<BasePlayer> playerListCopy; //Thread safe playerlist

void Cheat::ESP()
{
	playerListCopy = playerList; //copy list

	for (BasePlayer player : playerListCopy) {

		Vector3 CenterHitbox;
		Vector2 CenterHitboxScreen; 
		if (!player.GetEntityBonePosition(2, &CenterHitbox))
			continue;

		if (!Utl::WorldToScreen(CenterHitbox, &CenterHitboxScreen))
			continue;

		Vector3 HeadPosition;
		Vector2 HeadPositionScreen;
		if (!player.GetEntityBonePosition(BoneId::head, &HeadPosition))
			continue;

		if (!Utl::WorldToScreen(HeadPosition, &HeadPositionScreen))
			continue;

		Vector3 Origin = player.m_vecAbsOrigin()->get();
		Vector2 OriginScreen;
		if (!Utl::WorldToScreen(Origin, &OriginScreen))
			continue;

		float box_height = HeadPositionScreen.y - OriginScreen.y;
		float box_width = box_height / 1.2f;


		if (settings.playerBox)
		{
			Render::Text(OriginScreen.x, OriginScreen.y, "Nigger", false, ImColor(255,255,255));
		}
		if (settings.playerHealth)
		{
			//Render::Text(OriginScreen.x, OriginScreen.y, "Nigger", false, ImColor(255, 255, 255));
		}

		Utl::DelayExeuction(10);
	}
}

void Cheat::Render()
{
	while (true) { 
		if (clientState.signOnState() != SignonState::Full) {
			playerListCopy.clear();
			std::this_thread::sleep_for(5s);
			continue;
		}

		playerListCopy = playerList; //copy list

		for (BasePlayer player : playerListCopy) {


			

			//Glow Esp
			if (settings.playerGlow) {
				if (player.IsValid()) //this is sort of redundant, but I kept writing to invalid entities
					player.DoGlow(Color{ 132,3,252 });
			}

		} 
		std::this_thread::sleep_for(1s); //only once a second because the glow only needs to be written once
	}
}