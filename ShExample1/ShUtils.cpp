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

VOID GetProcessHandleById(IN HANDLE Pid, OUT PHANDLE ProcessHandle)
{
	CLIENT_ID Cid = { 0, };
	Cid.UniqueProcess = Pid;

	OBJECT_ATTRIBUTES ObjAttribute = { 0, };
	ObjAttribute.Length = sizeof(OBJECT_ATTRIBUTES);
	NTSTATUS Status = ZwOpenProcess(
		ProcessHandle,
		NTOPEN_ACCESS,
		&ObjAttribute,
		&Cid
	);
}

VOID ScanDriver()
{
	SYSTEM_MODULE_ENTRY SystemModule = { 0, };
	if (GetModuleInformation("\\SystemRoot\\System32\\ntoskrnl.exe", &SystemModule) != 0)
	{
		Log("Not Found\n");
		return;
	}

	PVOID PsLoadedModuleListPtr = ScanBytes(
		(PSTR)SystemModule.ImageBase,
		(PSTR)((DWORD64)SystemModule.ImageBase + SystemModule.ImageSize),
		"48 8D 05 ?? ?? ?? ?? 33 C9 44 8B E1"
	);

	if (PsLoadedModuleListPtr == nullptr) { Log("Not found pattern\n"); return; }
	ULONG CalcBytes;
	RtlCopyMemory(&CalcBytes, (PVOID)((DWORD64)PsLoadedModuleListPtr + 3), 4);
	PLDR_DATA_TABLE_ENTRY PsLoadedModuleList = (PLDR_DATA_TABLE_ENTRY)((DWORD64)PsLoadedModuleListPtr + 7 + CalcBytes);
	PLIST_ENTRY Head = (PLIST_ENTRY)PsLoadedModuleList;
	PLDR_DATA_TABLE_ENTRY TempEntry = (PLDR_DATA_TABLE_ENTRY)PsLoadedModuleList->InLoadOrderLinks.Flink;
	
	while ((PLIST_ENTRY)TempEntry != Head)
	{

		Log("%wZ : 0x%p\n", TempEntry->BaseDllName, TempEntry->DllBase);
		TempEntry = (PLDR_DATA_TABLE_ENTRY)TempEntry->InLoadOrderLinks.Flink;
	}
	

	// Handle Info
	/*ULONG BufferSize = 0;
	PVOID Buffer = NULL;
	
	NTSTATUS Status = ShGlobal.NtQuerySystemInformation(SystemExtendedHandleInformation, Buffer, BufferSize, &BufferSize);
	while (Status == STATUS_INFO_LENGTH_MISMATCH)
	{
		if (Buffer) { ExFreePool(Buffer); }
		Buffer = ExAllocatePool(NonPagedPool, BufferSize);
		Status = ShGlobal.NtQuerySystemInformation(SystemExtendedHandleInformation, Buffer, BufferSize, &BufferSize);
	}
	if (Status || !Buffer)
	{
		if (Buffer != NULL) { ExFreePool(Buffer); }
		return;
	}
	
	PSYSTEM_HANDLE_INFORMATION_EX HandleInformation = (PSYSTEM_HANDLE_INFORMATION_EX)Buffer;
	for (int i = 0; i < HandleInformation->NumberOfHandles; i++)
	{
		SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX Entry = HandleInformation->Handles[i];
		if (i<10)
		{
			Log("[%d] 0x%p\n", Entry.UniqueProcessId, Entry.Object);
		}
	}
	ExFreePool(Buffer);*/

	/*SYSTEM_MODULE_ENTRY SystemModule = { 0, };
	if (GetModuleInformation("\\SystemRoot\\System32\\ntoskrnl.exe", &SystemModule) != 0)
	{
		Log("Not Found\n");
		return;
	}
	PVOID PiDDbCacheTablePtr = ScanBytes(
		(PSTR)SystemModule.ImageBase,
		(PSTR)((DWORD64)SystemModule.ImageBase + SystemModule.ImageSize),
		"48 8D 0D ?? ?? ?? ?? 45 33 F6 48 89"
	);
	if (PiDDbCacheTablePtr == nullptr)
	{
		Log("Not found pattern\n");
		return;
	}
	ULONG CalcBytes = 0;
	RtlCopyMemory(&CalcBytes, (PVOID)((DWORD64)PiDDbCacheTablePtr + 3), 4);
	PRTL_AVL_TABLE PiDDBCacheTable = (PRTL_AVL_TABLE)((DWORD64)PiDDbCacheTablePtr + 7 + CalcBytes);
	PVOID FirstNode = PiDDBCacheTable->BalancedRoot.RightChild;

	_PiDDBCacheEntry* FirstEntry = (PiDDBCacheEntry*)((DWORD64)FirstNode + sizeof(RTL_BALANCED_LINKS));
	PLIST_ENTRY Head = FirstEntry->List.Flink;
	PLIST_ENTRY TempList = (PLIST_ENTRY)FirstEntry;
	int i = 1;
	while (true)
	{
		TempList = TempList->Flink;
		if (TempList->Flink == Head) { break; }
		PiDDBCacheEntry* Entry = (PiDDBCacheEntry*)TempList;
		Log("[0x%X] Name : %wZ Status : 0x%X\n", i, Entry->DriverName, Entry->LoadStatus);
		i++;
	}*/
	 

	/*PVOID PiDDBCacheListPtr = ScanBytes(
		(PSTR)SystemModule.ImageBase, 
		(PSTR)((DWORD64)SystemModule.ImageBase + SystemModule.ImageSize), 
		"48 8D 15 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 48 39 11 75 24"
	);
	if (PiDDBCacheListPtr == nullptr)
	{
		Log("Not found pattern\n");
		return;
	}
	ULONG CalcBytes = 0;
	RtlCopyMemory(&CalcBytes, (PVOID)((DWORD64)PiDDBCacheListPtr + 3), 4);
	PLIST_ENTRY PiDDBCacheList = (LIST_ENTRY*)((DWORD64)PiDDBCacheListPtr + 7 + CalcBytes);
	PLIST_ENTRY Head = PiDDBCacheList->Flink;
	PLIST_ENTRY TempList = PiDDBCacheList;
	int i = 0;
	while (true)
	{
		TempList = TempList->Flink;
		if (TempList->Flink == Head)
		{
			break;
		}
		PiDDBCacheEntry* Entry = (PiDDBCacheEntry *)TempList;
		Log("%p : %wZ : %X\n",Entry, Entry->DriverName, Entry->LoadStatus);
		i++;
	}
	Log("Drivers : %d\n", i);*/

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
