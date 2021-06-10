#include "ShInc.h"

OB_PREOP_CALLBACK_STATUS ShObject::PreCallback(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION PreOperationInfo)
{
	OB_PREOP_CALLBACK_STATUS Status = OB_PREOP_SUCCESS;
	if (PreOperationInfo->ObjectType == *PsProcessType)
	{
		if (PreOperationInfo->Operation == OB_OPERATION_HANDLE_CREATE)
		{
			PCHAR ProcessName = ShGlobal.PsGetProcessImageFileName((PEPROCESS)PreOperationInfo->Object);
		}
	}
	return Status;
}

void ShObject::PostCallback(PVOID RegistrationContext, POB_POST_OPERATION_INFORMATION PostOperationInfo)
{
	
}

void ShObject::LoadImageNotifyRoutine(IN PUNICODE_STRING FullImageName, IN HANDLE ProcessId, IN PIMAGE_INFO ImageInfo)
{
	if (IsValid(&ShGlobal))
	{
		if (ShGlobal.TargetProcessId == ProcessId)
		{
			Log("Found\n");
		}
	}
}

void ShObject::CreateImageNotifyRoutine(HANDLE ParentId, HANDLE ProcessId, BOOLEAN Create)
{
	if (IsValid(&ShGlobal))
	{
		if (ShGlobal.TargetProcessId == ProcessId)
		{
			PEPROCESS Process = NULL;
			PsLookupProcessByProcessId(ProcessId, &Process);
			PVOID ObjectHeader = (PVOID)((DWORD64)Process - 0x30);
			PULONG RefCount = (PULONG)ObjectHeader;
			PCHAR ProcessName = ShGlobal.PsGetProcessImageFileName(Process);
			Log("%s : Create %d\n", ProcessName, Create);
			
		}
	}
}

bool ShObject::RegObjectCallback()
{
	OB_CALLBACK_REGISTRATION	CallbackRegistration = { 0, };
	OB_OPERATION_REGISTRATION	OperationRegistration = { 0, };

	CallbackRegistration.Version = ObGetFilterVersion();
	CallbackRegistration.OperationRegistrationCount = 1;
	CallbackRegistration.RegistrationContext = NULL;
	CallbackRegistration.OperationRegistration = &OperationRegistration;
	RtlInitUnicodeString(&CallbackRegistration.Altitude, L"300000");
	
	OperationRegistration.ObjectType = PsProcessType;
	OperationRegistration.Operations = OB_OPERATION_HANDLE_CREATE;
	OperationRegistration.PreOperation = PreCallback;
	OperationRegistration.PostOperation = PostCallback;

	NTSTATUS Status = ObRegisterCallbacks(&CallbackRegistration, &ShGlobal.RegistrationHandle);
	if (NT_SUCCESS(Status) == FALSE)
	{
		NtErrorHandler("ObRegisterCallbacks", Status);
		return false;
	}
	return true;
}
