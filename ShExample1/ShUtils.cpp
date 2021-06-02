#include "ShInc.h"


PVOID GetRoutineAddress(IN PCWSTR RoutineName)
{
	UNICODE_STRING RoutineString;
	RtlInitUnicodeString(&RoutineString, RoutineName);

	return MmGetSystemRoutineAddress(&RoutineString);
}

BOOLEAN IsValid(IN PVOID Address)
{
	if (MmIsAddressValid(Address) == FALSE || Address == NULL) { return FALSE; }
	return TRUE;
}

VOID InitUnicodeString(IN PCWSTR String, OUT PUNICODE_STRING UnicodeString)
{
	RtlZeroMemory(UnicodeString, sizeof(UNICODE_STRING));
	RtlInitUnicodeString(UnicodeString, String);
	return;
}

NTSTATUS GetModuleInformation(IN PSTR ModuleName, OUT PSYSTEM_MODULE_ENTRY ModuleEntry)
{
	BOOLEAN FindFlag = FALSE;
	ULONG infoLen = 0;
	UNICODE_STRING ZwQueryString = { 0, };
	PSYSTEM_MODULE_INFORMATION ModuleInformation = { 0, };
	RtlInitUnicodeString(&ZwQueryString, L"ZwQuerySystemInformation");
	NtQuerySystemInformation_t ZwQuerySystemInformation = (NtQuerySystemInformation_t)MmGetSystemRoutineAddress(&ZwQueryString);

	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, &infoLen, 0, &infoLen);
	if (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		ModuleInformation = (PSYSTEM_MODULE_INFORMATION)ExAllocatePoolWithTag(NonPagedPool, infoLen, 'H0YA');
		if (IsValid(ModuleInformation) == FALSE) { return STATUS_MEMORY_NOT_ALLOCATED; }
	}
	else { return status; }

	RtlZeroMemory(ModuleInformation, infoLen);
	status = ZwQuerySystemInformation(SystemModuleInformation, ModuleInformation, infoLen, &infoLen);

	PSYSTEM_MODULE_ENTRY pModEntry = ModuleInformation->Module;
	for (int i = 0; i < ModuleInformation->Count; i++)
	{
		if (!_stricmp((const char*)pModEntry[i].FullPathName, ModuleName))
		{
			Log("Find Module %s\n", pModEntry[i].FullPathName);
			*ModuleEntry = pModEntry[i];
			FindFlag = TRUE;
			break;
		}
	}
	ExFreePoolWithTag(ModuleInformation, 'H0YA');
	if (!FindFlag)
	{
		return STATUS_NOT_FOUND;
	}
	return status;
}

BOOLEAN IoValidCheck(IN PVOID InBuffer, IN PVOID OutBuffer, IN SIZE_T InSize, IN SIZE_T OutSize)
{
	__try {
		if (InBuffer != NULL) { ProbeForRead(InBuffer, InSize, 1); }
		if (OutBuffer != NULL) { ProbeForWrite(OutBuffer, OutSize, 1); }
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
		return FALSE;
	}
	return TRUE;
}

VOID NtErrorHandler(PSTR Caller, NTSTATUS Status)
{
	ErrLog("%s : 0x%X\n", Caller, Status);
}

SIZE_T TrimBytes(IN PSTR Sig, IN PSTR Coll OPTIONAL, __in_bcount(Coll) SIZE_T CollSize, OUT PBOOLEAN Selector)
{
	NTSTATUS Status = STATUS_SUCCESS;
	SIZE_T Result = 0;
	PSTR Buffer = NULL;
	SIZE_T BufferSize = 0;
	CHAR Single[3] = { 0 };
	ULONG Digit = 0;
	SIZE_T Index = 0;
	ULONG Length = 0;

	Length = strlen(Sig);

	for (Index = 0;
		Index < Length;
		Index++) {
		if (0 != isxdigit(*(Sig + Index)) ||
			0 == _CmpByte(*(Sig + Index), '?')) {
			BufferSize++;
		}
	}
	if (0 != BufferSize) {
		Buffer = (PSTR)ExAllocatePool(
			NonPagedPool,
			BufferSize);

		if (NULL != Buffer) {
			RtlZeroMemory(
				Buffer,
				BufferSize);

			for (Index = 0;
				Index < Length;
				Index++) {
				if (0 != isxdigit(*(Sig + Index)) ||
					0 == _CmpByte(*(Sig + Index), '?')) {
					RtlCopyMemory(
						Buffer + strlen(Buffer),
						Sig + Index,
						2);

					Index++;
				}
			}

			if (0 != (BufferSize & 1)) {
				Result = -1;
			}
			else {
				if (NULL == Coll) {
					Result = BufferSize / 2;
				}
				else {
					Result = BufferSize / 2;

					if (CollSize >= BufferSize / 2) {
						for (Index = 0;
							Index < BufferSize;
							Index += 2) {
							if (0 == _CmpByte(*(Buffer + Index), '?') &&
								0 == _CmpByte(*(Buffer + Index + 1), '?')) {
								*(Coll + Index / 2) = '?';

								*Selector = TRUE;
							}
							else if (0 != isxdigit(*(Buffer + Index)) &&
								0 != isxdigit(*(Buffer + Index + 1))) {
								RtlCopyMemory(
									Single,
									Buffer + Index,
									sizeof(CHAR) * 2);

								Status = RtlCharToInteger(
									Single,
									16,
									&Digit);

								if (NT_SUCCESS(Status)) {
									*(Coll + Index / 2) = (CHAR)Digit;

									*Selector =
										*Selector ? TRUE : FALSE;
								}
								else {
									Result = -1;

									break;
								}
							}
							else {
								Result = -1;

								break;
							}
						}
					}
					else {
						Result = -1;
					}
				}
			}
			ExFreePool(Buffer);
		}
	}

	return Result;
}

SIZE_T CompareBytes(IN PSTR Destination, IN PSTR Source, IN SIZE_T Length, IN BOOLEAN Selector)
{
	SIZE_T Count = 0;

	if (FALSE == Selector) {
		Count = RtlCompareMemory(
			Destination,
			Source,
			Length);
	}
	else {
		for (Count = 0;
			Count < Length;
			Count++) {
			if (0 != _CmpByte(*(Destination + Count), *(Source + Count)) &&
				0 != _CmpByte(*(Source + Count), '?')) {
				break;
			}
		}
	}

	return Count;
}

PVOID ScanBytes(IN PSTR Begin, IN PSTR End, IN PSTR Sig)
{
	BOOLEAN Selector = FALSE;
	PSTR Coll = NULL;
	SIZE_T CollSize = 0;
	PVOID Result = NULL;
	SIZE_T Index = 0;

	CollSize = TrimBytes(
		Sig,
		NULL,
		CollSize,
		&Selector);

	if (-1 != CollSize) {
		if ((LONG_PTR)(End - Begin - CollSize) >= 0) {
			Coll = (PSTR)ExAllocatePool(
				NonPagedPool,
				CollSize);

			if (NULL != Coll) {
				CollSize = TrimBytes(
					Sig,
					Coll,
					CollSize,
					&Selector);

				if (-1 != CollSize) {
					for (Index = 0;
						Index < End - Begin - CollSize;
						Index++) {
						if (CollSize == CompareBytes(
							Begin + Index,
							Coll,
							CollSize,
							Selector)) {
							Result = Begin + Index;
							break;
						}
					}
				}

				ExFreePool(Coll);
			}
		}
	}

	return Result;
}
