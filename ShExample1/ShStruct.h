#ifndef _SHSTRUCT_H_
#define _SHSTRUCT_H_

typedef struct _SH_GLOBAL {
	PDRIVER_OBJECT				DriverObject;
	PDEVICE_OBJECT				DeviceObject;
	PVOID						RegistrationHandle;
	PsGetProcessImageFileName_t PsGetProcessImageFileName;
	NtQuerySystemInformation_t  NtQuerySystemInformation;
	PEPROCESS					TargetProcess;
	HANDLE						TargetProcessId;
	BOOLEAN						IsNotifyRoutine;
	ULONG						Index;
}SH_GLOBAL, * PSH_GLOBAL;

typedef struct _OBJECT_REF {
	PVOID ObjectHeader;
	PVOID Object;
	ULONG TypeIndex;
	ULONG RefCount;
}OBJECT_REF, * POBJECT_REF;

typedef struct _DEBUGGER_INFO {
	ULONG DebuggerId;
	ULONG DebuggeeId;
}DEBUGGER_INFO, * PDEBUGGER_INFO;

#endif
