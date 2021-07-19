#ifndef _SHUTILS_H_
#define _SHUTILS_H_

#define Log(...) DbgPrintEx( DPFLTR_SYSTEM_ID,DPFLTR_ERROR_LEVEL, "[Shh0ya] " __VA_ARGS__ )
#define ErrLog(...) DbgPrintEx( DPFLTR_SYSTEM_ID, DPFLTR_ERROR_LEVEL, "[Error] " __VA_ARGS__ )


/*
* Wrapping functions
*/

// MmGetSystemRoutineAddress Wrapping
PVOID GetRoutineAddress(IN PCWSTR RoutineName);									

// MmIsAddressValid Wrapping
BOOLEAN IsValid(IN PVOID Address);													

// Nt,ZwQuerySystemInformation Wrapping
extern "C" NTSTATUS GetModuleInformation(
	IN PSTR ModuleName,
	OUT PSYSTEM_MODULE_ENTRY ModuleEntry
);

// NTSTATUS Error Handler
VOID NtErrorHandler(IN PSTR Caller, IN NTSTATUS Status);

// I/O Buffer valid check
BOOLEAN IoValidCheck(
	IN PVOID	InBuffer, 
	IN PVOID	OutBuffer, 
	IN SIZE_T	InSize,
	IN SIZE_T	OutSize
);

// GetHandleByPid
VOID GetProcessHandleById(
	IN HANDLE Pid,
	OUT PHANDLE ProcessHandle
);



VOID ScanDriver();


/*
* Scan bytes functions
*/

extern "C" SIZE_T  NTAPI TrimBytes(
	IN PSTR Sig,
	IN PSTR Coll OPTIONAL,
	__in_bcount(Coll) SIZE_T CollSize,
	OUT PBOOLEAN Selector
);

extern "C" SIZE_T  NTAPI CompareBytes(
	IN PSTR Destination,
	IN PSTR Source,
	IN SIZE_T Length,
	IN BOOLEAN Selector
);

extern "C" PVOID   NTAPI ScanBytes(
	IN PSTR Begin,
	IN PSTR End,
	IN PSTR Sig
);

//--------------------- Scan.asm -----------------------------------------//
extern "C" BOOLEAN	NTAPI _CmpByte(IN CHAR b1, IN CHAR b2);
extern "C" BOOLEAN	NTAPI _CmpShort(IN SHORT s1, IN SHORT s2);
extern "C" BOOLEAN	NTAPI _CmpLong(IN LONG l1, IN LONG l2);
extern "C" BOOLEAN	NTAPI _CmpLongLong(IN LONGLONG ll1, IN LONGLONG ll2);
//--------------------- Scan.asm -----------------------------------------//



#endif