#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#ifndef _NTDLL_H_
#define _NTDLL_H_

#include "ntdef.h"

NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateSection(
	OUT PHANDLE SectionHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
	IN PLARGE_INTEGER MaximumSize OPTIONAL,
	IN ULONG SectionPageProtection,
	IN ULONG AllocationAttributes,
	IN HANDLE FileHandle OPTIONAL
);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtQuerySection(
	IN HANDLE SectionHandle,
	IN SECTION_INFORMATION_CLASS SectionInformationClass,
	OUT PVOID SectionInformation,
	IN SIZE_T SectionInformationLength,
	OUT PSIZE_T ReturnLength OPTIONAL
);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateProcess(
	OUT PHANDLE ProcessHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
	IN HANDLE ParentProcess,
	IN BOOLEAN InheritObjectTable,
	IN HANDLE SectionHandle OPTIONAL,
	IN HANDLE DebugPort OPTIONAL,
	IN HANDLE ExceptionPort OPTIONAL
);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtTerminateProcess(
	IN HANDLE ProcessHandle OPTIONAL,
	IN NTSTATUS ExitStatus
);

#endif // _NTDLL_H_
