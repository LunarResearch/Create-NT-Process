#include "ntdll.h"
#include <corecrt_wstdio.h>

UNICODE_STRING U_Str;
OBJECT_ATTRIBUTES ObjAttr;
IO_STATUS_BLOCK FileStatus;
SECTION_IMAGE_INFORMATION SectionImageInfo;
MEMORY_BASIC_INFORMATION MemoryBasicInfo;
CONTEXT ThreadContext;
CLIENT_ID ClientId;
WIN32_FIND_DATAW FindData;

HANDLE hFile, hSection, hProcess, hThread;
WCHAR Path[MAX_PATH];
DWORD dwProcessId, dwThreadId;

void wmain(void) {
	RtlInitUnicodeString(&U_Str, L"\\??\\E:\\Games\\MinPack\\Far Cry 5\\bin\\FarCry5.exe"); // ?? or DosDevices
	InitializeObjectAttributes(&ObjAttr, &U_Str, OBJ_CASE_INSENSITIVE, 0, NULL);

	NtOpenFile(&hFile, FILE_GENERIC_READ | FILE_GENERIC_WRITE | FILE_GENERIC_EXECUTE, &ObjAttr, &FileStatus,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, FILE_SEQUENTIAL_ONLY);
	
	NtCreateSection(&hSection, SECTION_ALL_ACCESS, NULL, 0, PAGE_EXECUTE_READWRITE, SEC_IMAGE, hFile);
	NtClose(hFile);

	NtQuerySection(hSection, SectionImageInformation, &SectionImageInfo, sizeof(SECTION_IMAGE_INFORMATION), 0);

	NtCreateProcess(&hProcess, PROCESS_ALL_ACCESS, NULL, NtCurrentProcess(), FALSE, hSection, 0, 0);
	NtClose(hSection);
	
	ThreadContext.ContextFlags =  CONTEXT_ALL;
	NtGetContextThread(NtCurrentThread(), &ThreadContext);
	NtQueryVirtualMemory(hProcess, (PVOID)ThreadContext.Rip, MemoryBasicInformation, &MemoryBasicInfo, sizeof(MEMORY_BASIC_INFORMATION), 0);
	INITIAL_TEB InitialTeb = { 0, 0, (PCHAR)MemoryBasicInfo.BaseAddress + MemoryBasicInfo.RegionSize, MemoryBasicInfo.BaseAddress, MemoryBasicInfo.AllocationBase };
	
	NtCreateThread(&hThread, THREAD_ALL_ACCESS, NULL, hProcess, &ClientId, &ThreadContext, &InitialTeb, TRUE);

	//CsrClientCallServer(&ApiMessage, CaptureBuffer, CSR_CREATE_API_NUMBER(CSRSRV_SERVERDLL_INDEX, CsrpClientConnect), sizeof(CSR_CLIENT_CONNECT));
	//CsrAllocateCaptureBuffer
	// CsrAllocateMessagePointer(CaptureBuffer, MessageLength, CapturedData);
	//CsrCaptureMessageBuffer

	//NtResumeThread(hThread, 0);

	dwProcessId = (DWORD)ClientId.UniqueProcess;
	dwThreadId = (DWORD)ClientId.UniqueThread;

	K32GetModuleFileNameExW(hProcess, 0, Path, MAX_PATH);
	FindFirstFileW(Path, &FindData);
	
	wprintf(L"Process name: %s\nProcess handle: %x\nProcess ID: %d\nThread handle: %x\nThread ID: %d\nEntry point: 0x%016llX\nMachine: %x\nFile size: %d\n",
		FindData.cFileName, (UINT)hProcess, dwProcessId, (UINT)hThread, dwThreadId, (UINT64)SectionImageInfo.TransferAddress, SectionImageInfo.Machine, SectionImageInfo.ImageFileSize);

	getwchar();
	NtTerminateProcess(hProcess, 0);
}
