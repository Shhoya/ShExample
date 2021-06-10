#ifndef _SHENTRY_H_
#define _SHENTRY_H_

#define DEVICE_NAME		L"\\Device\\ShhoyaDriver"
#define SYMBOLIC_NAME	L"\\DosDevices\\ShDrv"
#define NTOPEN_ACCESS   (PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION|PROCESS_VM_READ|PROCESS_VM_WRITE|PROCESS_SUSPEND_RESUME|PROCESS_QUERY_INFORMATION)

/*
* Extern Variable
*/
extern SH_GLOBAL ShGlobal;

/*
* Dispatch parser enumeration
*/
enum DispatchFunction {
	SystemBuffer = 3,
	DebugObject = 6
};

/*
* Driver Entry & Driver Unload Routine
*/
extern "C" NTSTATUS DriverEntry(
	IN PDRIVER_OBJECT DriverObject,
	IN PUNICODE_STRING RegistryPath
);

VOID DriverUnload(
	IN PDRIVER_OBJECT DriverObject
);


/*
* Driver Dispatch Routines
*/

VOID IoCompleteRoutine(
	IN OUT PIRP Irp, 
	IN NTSTATUS Status, 
	IN int Size
);

NTSTATUS DriverCreate(
	IN OUT PDEVICE_OBJECT DeviceObject,
	IN OUT PIRP Irp
);

NTSTATUS DriverClose(
	IN OUT PDEVICE_OBJECT DeviceObject,
	IN OUT PIRP Irp
);

NTSTATUS DriverRead(
	IN OUT PDEVICE_OBJECT DeviceObject,
	IN OUT PIRP Irp
);

NTSTATUS DriverWrite(
	IN OUT PDEVICE_OBJECT DeviceObject,
	IN OUT PIRP Irp
);

NTSTATUS DriverDeviceControl(
	IN OUT PDEVICE_OBJECT DeviceObject,
	IN OUT PIRP Irp
);

NTSTATUS DriverCleanUp(
	IN OUT PDEVICE_OBJECT DeviceObject,
	IN OUT PIRP Irp
);

NTSTATUS DispatchParser(
	IN SIZE_T Size,
	IN PIRP Irp
);

/*
* Unsupported routine
*/
NTSTATUS DriverUnsupported(
	IN OUT PDEVICE_OBJECT DeviceObject,
	IN OUT PIRP Irp
);

#endif
