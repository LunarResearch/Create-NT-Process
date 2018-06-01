#ifdef _MSC_VER
#pragma once
#endif // _MSC_VER

#ifndef _NTDLL_H_
#define _NTDLL_H_

#include "ntdef.h"

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateSection(
	_Out_ PHANDLE SectionHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_opt_ PLARGE_INTEGER MaximumSize,
	_In_ ULONG SectionPageProtection,
	_In_ ULONG AllocationAttributes,
	_In_opt_ HANDLE FileHandle
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtQuerySection(
	_In_ HANDLE SectionHandle,
	_In_ SECTION_INFORMATION_CLASS SectionInformationClass,
	_Out_writes_bytes_(SectionInformationLength) PVOID SectionInformation,
	_In_ SIZE_T SectionInformationLength,
	_Out_opt_ PSIZE_T ReturnLength
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateProcess(
	_Out_ PHANDLE ProcessHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_ HANDLE ParentProcess,
	_In_ BOOLEAN InheritObjectTable,
	_In_opt_ HANDLE SectionHandle,
	_In_opt_ HANDLE DebugPort,
	_In_opt_ HANDLE ExceptionPort
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtGetContextThread(
	_In_ HANDLE ThreadHandle,
	_Inout_ PCONTEXT ThreadContext
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryVirtualMemory(
	_In_ HANDLE ProcessHandle,
	_In_ PVOID BaseAddress,
	_In_ MEMORY_INFORMATION_CLASS MemoryInformationClass,
	_Out_writes_bytes_(MemoryInformationLength) PVOID MemoryInformation,
	_In_ SIZE_T MemoryInformationLength,
	_Out_opt_ PSIZE_T ReturnLength
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtCreateThread(
	_Out_ PHANDLE ThreadHandle,
	_In_ ACCESS_MASK DesiredAccess,
	_In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_ HANDLE ProcessHandle,
	_Out_ PCLIENT_ID ClientId,
	_In_ PCONTEXT ThreadContext,
	_In_ PINITIAL_TEB InitialTeb,
	_In_ BOOLEAN CreateSuspended
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtTerminateProcess(
	_In_opt_ HANDLE ProcessHandle,
	_In_ NTSTATUS ExitStatus
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
NtResumeThread(
	_In_ HANDLE ThreadHandle,
	_Out_opt_ PULONG PreviousSuspendCount
);

__kernel_entry NTSYSCALLAPI
NTSTATUS
NTAPI
CsrClientCallServer(
	_Inout_ PCSR_API_MESSAGE ApiMessage,
	_Inout_opt_ PCSR_CAPTURE_BUFFER CaptureBuffer,
	_In_ CSR_API_NUMBER ApiNumber,
	_In_ ULONG DataLength
);

PCSR_CAPTURE_BUFFER
NTAPI
CsrAllocateCaptureBuffer(
	_In_ ULONG ArgumentCount,
	_In_ ULONG BufferSize
);

ULONG
NTAPI
CsrAllocateMessagePointer(
	_Inout_ PCSR_CAPTURE_BUFFER CaptureBuffer,
	_In_ ULONG MessageLength,
	_Out_ PVOID *CapturedData
);

VOID
NTAPI
CsrCaptureMessageBuffer(
	_Inout_ PCSR_CAPTURE_BUFFER CaptureBuffer,
	_In_opt_ PVOID MessageBuffer,
	_In_ ULONG MessageLength,
	_Out_ PVOID *CapturedData
);

#endif // _NTDLL_H_
