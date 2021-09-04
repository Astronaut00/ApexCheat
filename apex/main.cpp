#include "cheat.h"
#include "utils.h"

Cheat* pCheat = nullptr;
Memory* pMem = nullptr;

void Init()
{
	int processId = NULL;
	HWND window = NULL;
	{

		auto window_process = Utl::GetWindowProcessID("Apex Legends", "Respawn001");
		processId = window_process.first;                       
		window = window_process.second;
		while (!processId)
			std::this_thread::sleep_for(5s);
	}


	pMem = new Memory(processId);
	pCheat = new Cheat();
	{
		std::thread(&Cheat::UpdatePlayers, pCheat).detach();
		std::thread(&Cheat::Render, pCheat).detach();
		std::thread(&Cheat::Aimbot, pCheat).detach();
		std::thread(&Cheat::Overlay, pCheat, window).detach();
	}

	while (!keydown(VK_END))
	{
		std::this_thread::sleep_for(2s);
		continue;
	}
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) 
{
	try
	{
		Init();
	}
	catch (const std::exception& e)
	{
		MessageBox(NULL, std::string(e.what()).c_str(), "Error", MB_ICONERROR);
	}
}

int main()
{
	Init();
}