#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#ifndef _NTDEF_H_
#define _NTDEF_H_

#include <Windows.h>
#include <winternl.h>

#pragma comment (lib, "ntdll.lib")

#define NtCurrentProcess() ((HANDLE)(LONG_PTR)-1)
#define NtCurrentThread() ((HANDLE)(LONG_PTR)-2)

#define CSR_CREATE_API_NUMBER (ServerId, ApiId) (CSR_API_NUMBER)(((ServerId) << 16) | (ApiId))
#define CSRSRV_SERVERDLL_INDEX 0

typedef CLIENT_ID *PCLIENT_ID;

typedef enum _SECTION_INFORMATION_CLASS {
	SectionBasicInformation = 0,
	SectionImageInformation = 1,
	SectionRelocationInformation = 2,
	SectionOriginalBaseInformation = 3,
	SectionInternalImageInformation = 4,
	MaxSectionInfoClass = 5
} SECTION_INFORMATION_CLASS;

typedef enum _MEMORY_INFORMATION_CLASS {
	MemoryBasicInformation = 0,
	MemoryWorkingSetInformation = 1,
	MemoryMappedFilenameInformation = 2,
	MemoryRegionInformation = 3,
	MemoryWorkingSetExInformation = 4,
	MemorySharedCommitInformation = 5,
	MemoryImageInformation = 6,
	MemoryRegionInformationEx = 7,
	MemoryPrivilegedBasicInformation = 8,
	MemoryEnclaveImageInformation = 9,
	MemoryBasicInformationCapped = 10
} MEMORY_INFORMATION_CLASS;

typedef struct _SECTION_IMAGE_INFORMATION {
	PVOID TransferAddress;
	ULONG ZeroBits;
	SIZE_T MaximumStackSize;
	SIZE_T CommittedStackSize;
	ULONG SubSystemType;
	union {
		struct {
			USHORT SubSystemMinorVersion;
			USHORT SubSystemMajorVersion;
		};
		ULONG SubSystemVersion;
	};
	union {
		struct {
			USHORT MajorOperatingSystemVersion;
			USHORT MinorOperatingSystemVersion;
		};
		ULONG OperatingSystemVersion;
	};
	USHORT ImageCharacteristics;
	USHORT DllCharacteristics;
	USHORT Machine;
	BOOLEAN ImageContainsCode;
	union {
		UCHAR ImageFlags;
		struct {
			UCHAR ComPlusNativeReady : 1;
			UCHAR ComPlusILOnly : 1;
			UCHAR ImageDynamicallyRelocated : 1;
			UCHAR ImageMappedFlat : 1;
			UCHAR BaseBelow4gb : 1;
			UCHAR ComPlusPrefer32bit : 1;
			UCHAR Reserved : 2;
		};
	};
	ULONG LoaderFlags;
	ULONG ImageFileSize;
	ULONG CheckSum;
} SECTION_IMAGE_INFORMATION, *PSECTION_IMAGE_INFORMATION;

typedef struct _INITIAL_TEB {
	struct {
		PVOID OldStackBase;
		PVOID OldStackLimit;
	} OldInitialTeb;
	PVOID StackBase;
	PVOID StackLimit;
	PVOID StackAllocationBase;
} INITIAL_TEB, *PINITIAL_TEB;

typedef ULONG CSR_API_NUMBER;

typedef struct _PORT_MESSAGE_HEADER {
	USHORT DataSize;
	USHORT MessageSize;
	USHORT MessageType;
	USHORT VirtualRangesOffset;
	CLIENT_ID ClientId;
	ULONG MessageId;
	ULONG SectionSize;
} PORT_MESSAGE_HEADER, *PPORT_MESSAGE_HEADER, PORT_MESSAGE, *PPORT_MESSAGE;

typedef struct _CSR_API_CONNECTINFO {
	HANDLE ObjectDirectory;
	PVOID  SharedSectionBase;
	PVOID  SharedStaticServerData;
	PVOID  SharedSectionHeap;
	ULONG  DebugFlags;
	ULONG  SizeOfPebData;
	ULONG  SizeOfTebData;
	ULONG  NumberOfServerDllNames;
	HANDLE ServerProcessId;
} CSR_API_CONNECTINFO, *PCSR_API_CONNECTINFO;

typedef struct _CSR_CAPTURE_BUFFER {
	ULONG Size;
	struct _CSR_CAPTURE_BUFFER *PreviousCaptureBuffer;
	ULONG PointerCount;
	PVOID BufferEnd;
	ULONG_PTR PointerOffsetsArray[ANYSIZE_ARRAY];
} CSR_CAPTURE_BUFFER, *PCSR_CAPTURE_BUFFER;

typedef struct _CSR_CLIENT_CONNECT {
	ULONG ServerId;
	PVOID ConnectionInfo;
	ULONG ConnectionInfoSize;
} CSR_CLIENT_CONNECT, *PCSR_CLIENT_CONNECT;

typedef struct _CSR_SET_PRIORITY_CLASS {
	HANDLE hProcess;
	ULONG PriorityClass;
} CSR_SET_PRIORITY_CLASS, *PCSR_SET_PRIORITY_CLASS;

typedef struct _CSR_IDENTIFY_ALTERTABLE_THREAD {
	CLIENT_ID Cid;
} CSR_IDENTIFY_ALTERTABLE_THREAD, *PCSR_IDENTIFY_ALTERTABLE_THREAD;

typedef struct _CSR_API_MESSAGE {
	PORT_MESSAGE Header;
	union {
		CSR_API_CONNECTINFO ConnectionInfo;
		struct {
			PCSR_CAPTURE_BUFFER CsrCaptureData;
			CSR_API_NUMBER ApiNumber;
			NTSTATUS Status;
			ULONG Reserved;
			union {
				CSR_CLIENT_CONNECT CsrClientConnect;
				CSR_SET_PRIORITY_CLASS SetPriorityClass;
				CSR_IDENTIFY_ALTERTABLE_THREAD IdentifyAlertableThread;
				ULONG_PTR ApiMessageData[39];
			} Data;
		};
	};
} CSR_API_MESSAGE, *PCSR_API_MESSAGE;

#endif // _NTDEF_H_
