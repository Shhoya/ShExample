#ifndef _SHOBJECT_H_
#define _SHOBJECT_H_

namespace ShObject {

	OB_PREOP_CALLBACK_STATUS PreCallback(
		IN PVOID RegistrationContext, 
		IN POB_PRE_OPERATION_INFORMATION PreOperationInfo
	);

	void PostCallback(
		IN PVOID RegistrationContext,
		IN POB_POST_OPERATION_INFORMATION PostOperationInfo
	);

	void LoadImageNotifyRoutine(
		IN PUNICODE_STRING	FullImageName,
		IN HANDLE			ProcessId,
		IN PIMAGE_INFO		ImageInfo
	);

	void CreateImageNotifyRoutine(
		IN HANDLE	ParentId,
		IN HANDLE	ProcessId,
		IN BOOLEAN Create
	);

	bool RegObjectCallback();

}

#endif
