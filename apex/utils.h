#pragma once
#include "stdafx.h"
namespace Utl {
	/*Process Id using snapshot handle, this is bad for EAC*/
	inline DWORD GetProcessID(const char* processName) {
		HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		DWORD procID = NULL;

		if (handle == INVALID_HANDLE_VALUE)
			return procID;

		PROCESSENTRY32 entry = { 0 };
		entry.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(handle, &entry)) {
			if (!_stricmp(processName, entry.szExeFile)) {
				procID = entry.th32ProcessID;
			}
			else while (Process32Next(handle, &entry)) {
				if (!_stricmp(processName, entry.szExeFile)) {
					procID = entry.th32ProcessID;
				}
			}
		}

		CloseHandle(handle);
		return procID;
	}

	//DWORD GetModuleBase(DWORD processId, const char* szModuleName)
	//{
	//	DWORD moduleBase = 0;

	//	//HANDLE apes = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
	//	//std::wcout << (GetLastError()) << std::endl;
	//	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processId);
	//	

	//	std::wcout << (GetLastError()) << std::endl;
	//	if (hSnapshot != INVALID_HANDLE_VALUE) {
	//		MODULEENTRY32 moduleEntry;
	//		moduleEntry.dwSize = sizeof(MODULEENTRY32);
	//		if (Module32First(hSnapshot, &moduleEntry)) {
	//			do {
	//				if (_stricmp(moduleEntry.szModule, szModuleName) == 0) {
	//					moduleBase = (DWORD)moduleEntry.modBaseAddr;
	//					break;
	//				}
	//			} while (Module32Next(hSnapshot, &moduleEntry));
	//		}
	//		CloseHandle(hSnapshot);
	//	}
	//	return moduleBase;
	//}

	static LONG(__stdcall* NtDelayExecution)(BOOL Alertable, PLARGE_INTEGER DelayInterval) = (LONG(__stdcall*)(BOOL, PLARGE_INTEGER)) GetProcAddress(GetModuleHandle("ntdll.dll"), "NtDelayExecution");

	/**
	 * A wrapper around NtDelayExecution.
	 */
	inline bool DelayExeuction(int64_t ns)
	{
		// Will store our relative interval for NtDelayExecution
		LARGE_INTEGER interval;

		// Store our relative delay time (multiply by -1 for relative)
		interval.QuadPart = -1 * ns;

		// Call NtDelayExecution and return success or failure
		return (NtDelayExecution(false, &interval) == 0); // 0 = STATUS_SUCCESS
	}

	inline void SetForegroundUnsupervised(HWND window, bool attatched = false)
	{
		auto currentForeground = GetForegroundWindow();
		if (!attatched)
		{
			AttachThreadInput(
				GetWindowThreadProcessId(currentForeground, NULL),
				GetCurrentThreadId(), TRUE
			);
		}
		SetForegroundWindow(window);

		if (!attatched)
		{
			AttachThreadInput(
				GetWindowThreadProcessId(currentForeground, NULL),
				GetCurrentThreadId(), FALSE
			);
		}
	}


	inline bool WorldToScreen(Vector3 worldPos, Vector2* screenPos)
	{
		Vector2 to = Vector2();

		uint64_t viewRenderer = pMem->Rpm<uint64_t>(pMem->base + OFFSET_RENDER);
		VMatrix matrix = pMem->Rpm<VMatrix>(viewRenderer + OFFSET_MATRIX);

		//VMatrix matrix = (pMem->Rpm<VMatrix>(viewMatrix));
		matrix.Transpose();

		Vector3 translationVector = Vector3(matrix._41, matrix._42, matrix._43);
		Vector3 up = Vector3(matrix._21, matrix._22, matrix._23);
		Vector3 right = Vector3(matrix._11, matrix._12, matrix._13);

		float w = translationVector.Dot(worldPos) + matrix._44;

		if (w < 0.098f)
			return false;

		float y = up.Dot(worldPos) + matrix._24;
		float x = right.Dot(worldPos) + matrix._14;

		static RECT screenSize{ 0,0,0,0 };
		static BOOL rectResult = GetWindowRect(GetDesktopWindow(), &screenSize);

		screenPos->x = (screenSize.bottom /2) * (1.f + x / w);
		screenPos->y = (screenSize.right / 2) * (1.f - y / w);
		return true;
	}
	/*Process Id without touching the process*/
	inline std::pair<DWORD, HWND> GetWindowProcessID(const char* windowName, const char* className) {

		DWORD procID = NULL;
		HWND window = NULL;

		window = FindWindowA(className, windowName);
		if (window)
			GetWindowThreadProcessId(window, &procID);

		return std::make_pair(procID, window);
	}

}