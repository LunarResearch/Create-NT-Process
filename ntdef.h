#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#ifndef _NTDEF_H_
#define _NTDEF_H_

#include <Windows.h>

#include <winternl.h>
#include <Psapi.h>

#pragma comment (lib, "ntdll.lib")

#define InitializeObjectAttributes( p, n, a, r, s ) { \
		(p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
		(p)->RootDirectory = r;                             \
		(p)->Attributes = a;                                \
		(p)->ObjectName = n;                                \
		(p)->SecurityDescriptor = s;                        \
		(p)->SecurityQualityOfService = NULL;               \
}

#define NtCurrentProcess() ((HANDLE)(LONG_PTR)-1)

typedef enum _SECTION_INFORMATION_CLASS {
	SectionBasicInformation,
	SectionImageInformation,
	SectionRelocationInformation,
	SectionOriginalBaseInformation,
	SectionInternalImageInformation,
	MaxSectionInfoClass
} SECTION_INFORMATION_CLASS;

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

#endif // _NTDEF_H_
