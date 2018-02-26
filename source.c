#include "ntdll.h"
#include <corecrt_wstdio.h>

UNICODE_STRING U_Str;
OBJECT_ATTRIBUTES ObjAttr;
IO_STATUS_BLOCK FileStatus;
SECTION_IMAGE_INFORMATION SectionImageInfo;
WIN32_FIND_DATAW FindData;
PROCESS_BASIC_INFORMATION ProcessBasicInfo;

HANDLE hFile, hSection, hProcess;
WCHAR Path[MAX_PATH];
DWORD ProcessId;

int wmain(int argc, wchar_t *argv[], wchar_t *envp[]) {
	RtlInitUnicodeString(&U_Str, L"\\??\\F:\\MinPack\\Assassin's Creed Origins\\ACOrigins.exe"); // ?? or DosDevices
	InitializeObjectAttributes(&ObjAttr, &U_Str, OBJ_CASE_INSENSITIVE, 0, NULL);

	NtOpenFile(&hFile, FILE_OPEN, &ObjAttr, &FileStatus, FILE_SHARE_READ, FILE_SEQUENTIAL_ONLY);
	NtCreateSection(&hSection, MAXIMUM_ALLOWED, NULL, 0, PAGE_READONLY, SEC_IMAGE, hFile);
	NtClose(hFile);
	NtQuerySection(hSection, SectionImageInformation, &SectionImageInfo, sizeof(SECTION_IMAGE_INFORMATION), 0);
	NtCreateProcess(&hProcess, MAXIMUM_ALLOWED, NULL, NtCurrentProcess(), FALSE, hSection, 0, 0);
	NtClose(hSection);

	K32GetModuleFileNameExW(hProcess, 0, Path, MAX_PATH);
	FindFirstFileW(Path, &FindData);
	NtQueryInformationProcess(hProcess, ProcessBasicInformation, &ProcessBasicInfo, sizeof(PROCESS_BASIC_INFORMATION), 0);
	ProcessId = ProcessBasicInfo.UniqueProcessId;

	wprintf(L"Porcess name: %s\nProcess handle: %x\nProcess ID: %d\nEntry Point: 0x%016llX\nMachine: %x\nFile size: %d\n",
		FindData.cFileName, hProcess, ProcessId, SectionImageInfo.TransferAddress, SectionImageInfo.Machine, SectionImageInfo.ImageFileSize);

	NtTerminateProcess(hProcess, 0);

	getwchar();
	return FALSE;
}
