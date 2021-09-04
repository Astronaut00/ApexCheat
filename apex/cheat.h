#pragma once
#include "stdafx.h"
#include "sdk.h"
#include <d3d11.h>

enum class AimbotModes : int
{
	TYPE_OFF,
	TYPE_LINEAR,
	TYPE_QUADTRATIC,
	TYPE_NONE
};

//TODO: Add Feater GUI
struct Settings
{
	float fov = 10;
	float smoothing = 50;

	bool rage = false;
	bool marker = false;
	bool aimbot = true;
	AimbotModes aimbotMode = AimbotModes::TYPE_LINEAR;
	bool fovCirvle = false;
	
	bool playerGlow = true;
	bool playerBox = false;
	bool playerHealth = false;
	bool radar = false;

	bool itemGlow = false;
	bool itemName = false;
	bool itemDistance = false;

};

//TODO: Use superclasses/subclasses for the calls for each Aimbot etc.
class Cheat
{
public:
	/*Public Shit*/
	Settings settings;

	/*Construct most game specific objects*/
	Cheat() {
		localPlayer = BasePlayer(pMem->Rpm<uint64_t>(pMem->base + OFFSET_LOCAL_ENT));
		render = pMem->Rpm<uint64_t>(pMem->base + OFFSET_RENDER);
		viewMatrix = pMem->Rpm<VMatrix>(render + OFFSET_MATRIX);
		clientState = ClientState(); 
	};

	/*Our Threads*/
	void UpdatePlayers();
	void UpdateItems();
	void Render();
	void Aimbot();
	bool LoadTextureFromMemory(unsigned char* bytes, size_t size, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
	void Overlay(HWND target);

private: //Why have I dont it like this? In case in the future we need to call certain functions from other threads...

	/*update*/
	//Nothing lol

	/*Aimbot Calls*/
	//std::vector<BasePlayer> ClosestPlayer(const Vector& localPlayerCameraPos, const Vector& localPlayerAngles); //TODO: Stop passing stuff here and do it from cache
	bool bestPlayer(BasePlayer* bestplayer);
	bool bestBone(int* bestBone, BasePlayer& player);
	void AimAtBone(Vector localPlayerCameraPos, BasePlayer& target, int bone); //TODO: Target and pone should be the only arguments

	/*Render*/
	void ESP();
	void Menu();

	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	ID3D11Device* g_pd3dDevice = NULL;
	ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
	IDXGISwapChain* g_pSwapChain = NULL;
	ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();
private:

	/*The Non-Thread safe player list*/
	std::vector<BasePlayer> playerList;
	std::vector<BasePlayer> itemList;

	/*game specific objects and classes*/
	BasePlayer localPlayer;
	ClientState clientState;
	
	uint64_t render;
	VMatrix viewMatrix;

}; extern Cheat *pCheat;

extern ID3D11ShaderResourceView* logo_texture;
extern unsigned char prevKeys[256];
extern unsigned char keys[256];

#define keydown(vk_key) ((keys[vk_key] & 0xF0) && !(prevKeys[vk_key] & 0xF0))