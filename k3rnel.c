#include "ntdll.h"
#include <corecrt_wstdio.h>

UNICODE_STRING U_Str;
OBJECT_ATTRIBUTES ObjAttr;
IO_STATUS_BLOCK FileStatus;
SECTION_IMAGE_INFORMATION SectionImageInfo;
WIN32_FIND_DATAW FindData;
PROCESS_BASIC_INFORMATION ProcessBasicInfo;
SYSTEM_THREAD_INFORMATION ThreadSystemInfo;

HANDLE hFile, hSection, hProcess, hThread;
WCHAR Path[MAX_PATH];
DWORD dwProcessId, dwThreadId;
PVOID BaseAddress;
SIZE_T ViewSize;

void wmain(void) {
	RtlInitUnicodeString(&U_Str, L"\\??\\E:\\MinPack\\Assassin's Creed Origins\\uplay_r1_loader64.dll"); // ?? or DosDevices ACOrigins.exe uplay_r1_loader64.dll
	InitializeObjectAttributes(&ObjAttr, &U_Str, OBJ_CASE_INSENSITIVE, 0, NULL);

	NtOpenFile(&hFile, FILE_GENERIC_READ | FILE_GENERIC_WRITE | FILE_GENERIC_EXECUTE, &ObjAttr, &FileStatus,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, FILE_SEQUENTIAL_ONLY);
	NtCreateSection(&hSection, SECTION_ALL_ACCESS, NULL, 0, PAGE_EXECUTE_READWRITE, SEC_IMAGE, hFile);
	NtClose(hFile);
	NtQuerySection(hSection, SectionImageInformation, &SectionImageInfo, sizeof(SECTION_IMAGE_INFORMATION), 0);
	NtCreateProcess(&hProcess, PROCESS_ALL_ACCESS, NULL, NtCurrentProcess(), FALSE, hSection, 0, 0);
	NtMapViewOfSection(hSection, hProcess, &BaseAddress, 0, 0, NULL, &ViewSize, ViewShare, 0, PAGE_EXECUTE_READWRITE);
	NtClose(hSection);
	NtCreateThreadEx(&hThread, THREAD_ALL_ACCESS, NULL, hProcess, NULL, NULL, THREAD_CREATE_FLAGS_CREATE_SUSPENDED, 0, 0, 0, NULL);

	NtQueryInformationProcess(hProcess, ProcessBasicInformation, &ProcessBasicInfo, sizeof(PROCESS_BASIC_INFORMATION), 0);
	NtQueryInformationThread(hThread, ThreadSystemThreadInformation, &ThreadSystemInfo, sizeof(SYSTEM_THREAD_INFORMATION), 0);
	if ((DWORD)ProcessBasicInfo.UniqueProcessId != (DWORD)ThreadSystemInfo.ClientId.UniqueProcess) {
		NtTerminateProcess(hProcess, 0);
		exit(-1);
	}
	else {
		dwProcessId = (DWORD)ThreadSystemInfo.ClientId.UniqueProcess;
		dwThreadId = (DWORD)ThreadSystemInfo.ClientId.UniqueThread;
	}

	K32GetModuleFileNameExW(hProcess, 0, Path, MAX_PATH);
	FindFirstFileW(Path, &FindData);
	
	wprintf(L"Process name: %s\nProcess handle: %x\nProcess ID: %d\nThread handle: %x\nThread ID: %d\nEntry point: 0x%016llX\nStart address: 0x%016llX {ntdll.dll!RtlUserThreadStart}\nImage base address: 0x%016llX\nMachine: %x\nFile size: %d\nImage size: %d\n",
		FindData.cFileName, (UINT)hProcess, dwProcessId, (UINT)hThread, dwThreadId,
		(UINT64)SectionImageInfo.TransferAddress, ThreadSystemInfo.StartAddress, BaseAddress,
		SectionImageInfo.Machine, SectionImageInfo.ImageFileSize, ViewSize);

	getwchar();
	NtTerminateProcess(hProcess, 0);
}
