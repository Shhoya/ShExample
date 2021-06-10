#include "ShInc.h"

SH_GLOBAL ShGlobal;

#pragma alloc_text("INIT",DriverEntry)
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	UNICODE_STRING DeviceName;
	UNICODE_STRING LinkName;
	NTSTATUS Status = STATUS_SUCCESS;
	DriverObject->DriverUnload = DriverUnload;

	Log("Shh0ya Driver Load\n");
	
	ShGlobal.DriverObject = DriverObject;
	RtlInitUnicodeString(&DeviceName, DEVICE_NAME);
	RtlInitUnicodeString(&LinkName, SYMBOLIC_NAME);

	Status = IoCreateDevice(
		DriverObject,
		0,
		&DeviceName,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&ShGlobal.DeviceObject
	);
	if (NT_SUCCESS(Status) == FALSE) { NtErrorHandler("IoCreateDevice", Status); return Status; }

	ShGlobal.DeviceObject->Flags |= DO_BUFFERED_IO;
	ShGlobal.DeviceObject->Flags |= DO_DIRECT_IO;

	Status = IoCreateSymbolicLink(&LinkName, &DeviceName);
	if (NT_SUCCESS(Status) == FALSE) { NtErrorHandler("IoCreateSymbolicLink", Status); return Status; }

	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		DriverObject->MajorFunction[i] = DriverUnsupported;
	}

	DriverObject->MajorFunction[IRP_MJ_CREATE]				= DriverCreate;
	DriverObject->MajorFunction[IRP_MJ_CLOSE]				= DriverClose;
	DriverObject->MajorFunction[IRP_MJ_READ]				= DriverRead;
	DriverObject->MajorFunction[IRP_MJ_WRITE]				= DriverWrite;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]		= DriverDeviceControl;
	DriverObject->MajorFunction[IRP_MJ_CLEANUP]				= DriverCleanUp;

	ShGlobal.PsGetProcessImageFileName = (PsGetProcessImageFileName_t)GetRoutineAddress(L"PsGetProcessImageFileName");
	if (ShGlobal.PsGetProcessImageFileName == NULL)
	{
		ErrLog("Not found routine address\n");
		return STATUS_NOT_FOUND;
	}


	return Status;
}

VOID DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
	if (IsValid(ShGlobal.DeviceObject))
	{
		UNICODE_STRING LinkName;
		RtlInitUnicodeString(&LinkName, SYMBOLIC_NAME);

		IoDeleteDevice(ShGlobal.DeviceObject);
		IoDeleteSymbolicLink(&LinkName);
	}

	if (ShGlobal.RegistrationHandle)
	{
		ObUnRegisterCallbacks(ShGlobal.RegistrationHandle);
	}
	if (ShGlobal.IsNotifyRoutine)
	{
		PsRemoveLoadImageNotifyRoutine(&ShObject::LoadImageNotifyRoutine);
	}
	PsSetCreateProcessNotifyRoutine(&ShObject::CreateImageNotifyRoutine, TRUE);
	Log("Shh0ya Driver Unload\n");
	return;
}

VOID IoCompleteRoutine(IN OUT PIRP Irp, IN NTSTATUS Status, IN int Size)
{
	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = Size;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
}

NTSTATUS DriverCreate(IN OUT PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp)
{
	IoCompleteRoutine(Irp, STATUS_SUCCESS, 0);
	return STATUS_SUCCESS;
}

NTSTATUS DriverClose(IN OUT PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp)
{
	IoCompleteRoutine(Irp, STATUS_SUCCESS, 0);
	return STATUS_SUCCESS;
}

NTSTATUS DriverRead(IN OUT PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp)
{
	IoCompleteRoutine(Irp, STATUS_SUCCESS, 0);
	return STATUS_SUCCESS;
}

NTSTATUS DriverWrite(IN OUT PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp)
{
	IoCompleteRoutine(Irp, STATUS_SUCCESS, 0);
	return STATUS_SUCCESS;
}

NTSTATUS DriverDeviceControl(IN OUT PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp)
{
	PIO_STACK_LOCATION IoStackLocation = NULL;
	IoStackLocation = IoGetCurrentIrpStackLocation(Irp);

	// Filter I/O Method
	ULONG ControlCode = IoStackLocation->Parameters.DeviceIoControl.IoControlCode;
	ULONG MethodType = ControlCode & 0xFF;
	
	switch (MethodType)
	{
	case METHOD_BUFFERED:
	{
		if ((ShGlobal.DeviceObject->Flags & DO_BUFFERED_IO) == FALSE)
		{
			Log("Not allowed Buffered I/O Method\n");
			IoCompleteRoutine(Irp, STATUS_ACCESS_DENIED, 0);
			return STATUS_ACCESS_DENIED;
		}

		NTSTATUS Status = DispatchParser(IoStackLocation->Parameters.DeviceIoControl.OutputBufferLength, Irp);
		if (NT_SUCCESS(Status) == FALSE)
		{
			IoCompleteRoutine(Irp, Status, 0);
			return Status;
		}

		IoCompleteRoutine(Irp, STATUS_SUCCESS, IoStackLocation->Parameters.DeviceIoControl.OutputBufferLength);	// Ãë¾àÇÔ
		return STATUS_SUCCESS;
	}
	case METHOD_IN_DIRECT:
	{
		if ((ShGlobal.DeviceObject->Flags & DO_DIRECT_IO) == FALSE)
		{
			Log("Not allowed Direct I/O Method\n");
			IoCompleteRoutine(Irp, STATUS_ACCESS_DENIED, 0);
			return STATUS_ACCESS_DENIED;
		}
		if (IsValid(Irp->AssociatedIrp.SystemBuffer) == FALSE) 
		{ 
			ErrLog("Invalid System buffer\n"); 
			IoCompleteRoutine(Irp, STATUS_INVALID_ADDRESS, 0); 
			return STATUS_INVALID_ADDRESS;
		}
		
		ULONG* RecvBuffer = (ULONG*)Irp->AssociatedIrp.SystemBuffer;
		Log("Recv : 0x%X\n", *RecvBuffer);
		
		ULONG Buffer = 0xdeadbeef;
		ULONG* SendBuffer = (ULONG*)MmGetMdlVirtualAddress(Irp->MdlAddress);

		if (IoValidCheck(NULL, SendBuffer, 0, sizeof(ULONG)) == FALSE)
		{
			Log("Invalid User buffer\n");
			IoCompleteRoutine(Irp, STATUS_ACCESS_DENIED, 0);
			return STATUS_ACCESS_DENIED;
		}

		RtlCopyMemory(SendBuffer, &Buffer, sizeof(ULONG));
		IoCompleteRoutine(Irp, STATUS_SUCCESS, sizeof(ULONG));
		return STATUS_SUCCESS;
	}
	case METHOD_NEITHER:
	{
		if (IoValidCheck(
			IoStackLocation->Parameters.DeviceIoControl.Type3InputBuffer,
			Irp->UserBuffer,
			sizeof(ULONG),
			sizeof(ULONG)) == FALSE)
		{
			Log("Invalid User buffer\n");
			IoCompleteRoutine(Irp, STATUS_ACCESS_DENIED, 0);
			return STATUS_ACCESS_DENIED;
		}

		ULONG* RecvBuffer = (ULONG*)IoStackLocation->Parameters.DeviceIoControl.Type3InputBuffer;
		Log("Recv : 0x%X\n", *RecvBuffer);
		
		ULONG Buffer = 0xdeadbeef;
		ULONG* SendBuffer = (ULONG*)Irp->UserBuffer;
		
		RtlCopyMemory(SendBuffer, &Buffer, sizeof(ULONG));
		IoCompleteRoutine(Irp, STATUS_SUCCESS, sizeof(ULONG));
		return STATUS_SUCCESS;
	}

	default:
	{
		Log("Not Supported IO Method\n");
		break;
	}
	}

	IoCompleteRoutine(Irp, STATUS_SUCCESS, 0);
	return STATUS_SUCCESS;
}

NTSTATUS DriverCleanUp(IN OUT PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp)
{
	IoCompleteRoutine(Irp, STATUS_SUCCESS, 0);
	return STATUS_SUCCESS;
}

NTSTATUS DispatchParser(IN SIZE_T Size, IN PIRP Irp)
{
	if (Size > sizeof(ULONG))
	{
		PHANDLE Pid = (PHANDLE)Irp->AssociatedIrp.SystemBuffer;
		ShGlobal.TargetProcessId = *Pid;
		PEPROCESS Process = NULL;
		NTSTATUS Status = PsLookupProcessByProcessId(*Pid, &Process);
		if (NT_SUCCESS(Status) == FALSE)
		{
			NtErrorHandler("PsLookupProcessByProcessId", Status);
			return Status;
		}
		ObGetObjectType_t ObGetObjectType = (ObGetObjectType_t)GetRoutineAddress(L"ObGetObjectType");

		OBJECT_REF ObjRef = { 0, };
		ObjRef.ObjectHeader = (PVOID)((DWORD64)Process - 0x30);
		PULONG RefCount = (PULONG)ObjRef.ObjectHeader;

		PCHAR ProcessName = ShGlobal.PsGetProcessImageFileName(Process);

		Log("Process Name : %s\n", ProcessName);
		Log("Object : 0x%p\n", Process);
		Log("ObjectHeader : 0x%p\n", ObjRef.ObjectHeader);
		Log("Pre-Reference Count : %d\n", *RefCount);
		ObReferenceObject(Process);
		Log("Post-Reference Count : %d\n", *RefCount);
		ObDereferenceObject(Process);
		CLIENT_ID Cid = { 0, };
		Cid.UniqueProcess = *Pid;

		HANDLE ProcessHandle = NULL;
		OBJECT_ATTRIBUTES ObjAttribute = { 0, };
		ObjAttribute.Length = sizeof(OBJECT_ATTRIBUTES);
		Log("PID %d\n", *Pid);
		Status = ZwOpenProcess(
			&ProcessHandle,
			NTOPEN_ACCESS,
			&ObjAttribute,
			&Cid
		);
		if (NT_SUCCESS(Status))
		{
			Log("Open Success %X\n", ProcessHandle);
			NtClose(ProcessHandle);
		}
		else {
			NtErrorHandler("NtOpenProcess", Status);
		}

		return STATUS_SUCCESS;
	}

	else
	{
		ULONG* RecvBuffer = (ULONG*)Irp->AssociatedIrp.SystemBuffer;
		Log("Recv : 0x%X\n", *RecvBuffer);
		ULONG Buffer = 0xdeadbeef;
		RtlCopyMemory(Irp->AssociatedIrp.SystemBuffer, &Buffer, sizeof(ULONG));
	}
	return STATUS_SUCCESS;
}

NTSTATUS DriverUnsupported(IN OUT PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp)
{
	ErrLog("Not Supported\n");
	IoCompleteRoutine(Irp, STATUS_SUCCESS, 0);
	return STATUS_SUCCESS;
}
