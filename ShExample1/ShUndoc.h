#ifndef _SHUNDOC_H_
#define _SHUNDOC_H_

#define PROCESS_TERMINATE                  (0x0001)  
#define PROCESS_CREATE_THREAD              (0x0002)  
#define PROCESS_SET_SESSIONID              (0x0004)  
#define PROCESS_VM_OPERATION               (0x0008)  
#define PROCESS_VM_READ                    (0x0010)  
#define PROCESS_VM_WRITE                   (0x0020)  
#define PROCESS_DUP_HANDLE                 (0x0040)  
#define PROCESS_CREATE_PROCESS             (0x0080)  
#define PROCESS_SET_QUOTA                  (0x0100)  
#define PROCESS_SET_INFORMATION            (0x0200)  
#define PROCESS_QUERY_INFORMATION          (0x0400)  
#define PROCESS_SUSPEND_RESUME             (0x0800)  
#define PROCESS_QUERY_LIMITED_INFORMATION  (0x1000)  
#define PROCESS_SET_LIMITED_INFORMATION    (0x2000)  

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation = 0,
	SystemProcessorInformation = 1,
	SystemPerformanceInformation = 2,
	SystemTimeOfDayInformation = 3,
	SystemPathInformation = 4,
	SystemProcessInformation = 5,
	SystemCallCountInformation = 6,
	SystemDeviceInformation = 7,
	SystemProcessorPerformanceInformation = 8,
	SystemFlagsInformation = 9,
	SystemCallTimeInformation = 10,
	SystemModuleInformation = 11,
	SystemLocksInformation = 12,
	SystemStackTraceInformation = 13,
	SystemPagedPoolInformation = 14,
	SystemNonPagedPoolInformation = 15,
	SystemHandleInformation = 16,
	SystemObjectInformation = 17,
	SystemPageFileInformation = 18,
	SystemVdmInstemulInformation = 19,
	SystemVdmBopInformation = 20,
	SystemFileCacheInformation = 21,
	SystemPoolTagInformation = 22,
	SystemInterruptInformation = 23,
	SystemDpcBehaviorInformation = 24,
	SystemFullMemoryInformation = 25,
	SystemLoadGdiDriverInformation = 26,
	SystemUnloadGdiDriverInformation = 27,
	SystemTimeAdjustmentInformation = 28,
	SystemSummaryMemoryInformation = 29,
	SystemMirrorMemoryInformation = 30,
	SystemPerformanceTraceInformation = 31,
	SystemObsolete0 = 32,
	SystemExceptionInformation = 33,
	SystemCrashDumpStateInformation = 34,
	SystemKernelDebuggerInformation = 35,
	SystemContextSwitchInformation = 36,
	SystemRegistryQuotaInformation = 37,
	SystemExtendServiceTableInformation = 38,
	SystemPrioritySeperation = 39,
	SystemVerifierAddDriverInformation = 40,
	SystemVerifierRemoveDriverInformation = 41,
	SystemProcessorIdleInformation = 42,
	SystemLegacyDriverInformation = 43,
	SystemCurrentTimeZoneInformation = 44,
	SystemLookasideInformation = 45,
	SystemTimeSlipNotification = 46,
	SystemSessionCreate = 47,
	SystemSessionDetach = 48,
	SystemSessionInformation = 49,
	SystemRangeStartInformation = 50,
	SystemVerifierInformation = 51,
	SystemVerifierThunkExtend = 52,
	SystemSessionProcessInformation = 53,
	SystemLoadGdiDriverInSystemSpace = 54,
	SystemNumaProcessorMap = 55,
	SystemPrefetcherInformation = 56,
	SystemExtendedProcessInformation = 57,
	SystemRecommendedSharedDataAlignment = 58,
	SystemComPlusPackage = 59,
	SystemNumaAvailableMemory = 60,
	SystemProcessorPowerInformation = 61,
	SystemEmulationBasicInformation = 62,
	SystemEmulationProcessorInformation = 63,
	SystemExtendedHandleInformation = 64,
	SystemLostDelayedWriteInformation = 65,
	SystemBigPoolInformation = 66,
	SystemSessionPoolTagInformation = 67,
	SystemSessionMappedViewInformation = 68,
	SystemHotpatchInformation = 69,
	SystemObjectSecurityMode = 70,
	SystemWatchdogTimerHandler = 71,
	SystemWatchdogTimerInformation = 72,
	SystemLogicalProcessorInformation = 73,
	SystemWow64SharedInformationObsolete = 74,
	SystemRegisterFirmwareTableInformationHandler = 75,
	SystemFirmwareTableInformation = 76,
	SystemModuleInformationEx = 77,
	SystemVerifierTriageInformation = 78,
	SystemSuperfetchInformation = 79,
	SystemMemoryListInformation = 80,
	SystemFileCacheInformationEx = 81,
	SystemThreadPriorityClientIdInformation = 82,
	SystemProcessorIdleCycleTimeInformation = 83,
	SystemVerifierCancellationInformation = 84,
	SystemProcessorPowerInformationEx = 85,
	SystemRefTraceInformation = 86,
	SystemSpecialPoolInformation = 87,
	SystemProcessIdInformation = 88,
	SystemErrorPortInformation = 89,
	SystemBootEnvironmentInformation = 90,
	SystemHypervisorInformation = 91,
	SystemVerifierInformationEx = 92,
	SystemTimeZoneInformation = 93,
	SystemImageFileExecutionOptionsInformation = 94,
	SystemCoverageInformation = 95,
	SystemPrefetchPatchInformation = 96,
	SystemVerifierFaultsInformation = 97,
	SystemSystemPartitionInformation = 98,
	SystemSystemDiskInformation = 99,
	SystemProcessorPerformanceDistribution = 100,
	SystemNumaProximityNodeInformation = 101,
	SystemDynamicTimeZoneInformation = 102,
	SystemCodeIntegrityInformation = 103,
	SystemProcessorMicrocodeUpdateInformation = 104,
	SystemProcessorBrandString = 105,
	SystemVirtualAddressInformation = 106,
	SystemLogicalProcessorAndGroupInformation = 107,
	SystemProcessorCycleTimeInformation = 108,
	SystemStoreInformation = 109,
	SystemRegistryAppendString = 110,
	SystemAitSamplingValue = 111,
	SystemVhdBootInformation = 112,
	SystemCpuQuotaInformation = 113,
	SystemNativeBasicInformation = 114,
	SystemErrorPortTimeouts = 115,
	SystemLowPriorityIoInformation = 116,
	SystemBootEntropyInformation = 117,
	SystemVerifierCountersInformation = 118,
	SystemPagedPoolInformationEx = 119,
	SystemSystemPtesInformationEx = 120,
	SystemNodeDistanceInformation = 121,
	SystemAcpiAuditInformation = 122,
	SystemBasicPerformanceInformation = 123,
	SystemQueryPerformanceCounterInformation = 124,
	SystemSessionBigPoolInformation = 125,
	SystemBootGraphicsInformation = 126,
	SystemScrubPhysicalMemoryInformation = 127,
	SystemBadPageInformation = 128,
	SystemProcessorProfileControlArea = 129,
	SystemCombinePhysicalMemoryInformation = 130,
	SystemEntropyInterruptTimingInformation = 131,
	SystemConsoleInformation = 132,
	SystemPlatformBinaryInformation = 133,
	SystemPolicyInformation = 134,
	SystemHypervisorProcessorCountInformation = 135,
	SystemDeviceDataInformation = 136,
	SystemDeviceDataEnumerationInformation = 137,
	SystemMemoryTopologyInformation = 138,
	SystemMemoryChannelInformation = 139,
	SystemBootLogoInformation = 140,
	SystemProcessorPerformanceInformationEx = 141,
	SystemCriticalProcessErrorLogInformation = 142,
	SystemSecureBootPolicyInformation = 143,
	SystemPageFileInformationEx = 144,
	SystemSecureBootInformation = 145,
	SystemEntropyInterruptTimingRawInformation = 146,
	SystemPortableWorkspaceEfiLauncherInformation = 147,
	SystemFullProcessInformation = 148,
	SystemKernelDebuggerInformationEx = 149,
	SystemBootMetadataInformation = 150,
	SystemSoftRebootInformation = 151,
	SystemElamCertificateInformation = 152,
	SystemOfflineDumpConfigInformation = 153,
	SystemProcessorFeaturesInformation = 154,
	SystemRegistryReconciliationInformation = 155,
	SystemEdidInformation = 156,
	SystemManufacturingInformation = 157,
	SystemEnergyEstimationConfigInformation = 158,
	SystemHypervisorDetailInformation = 159,
	SystemProcessorCycleStatsInformation = 160,
	SystemVmGenerationCountInformation = 161,
	SystemTrustedPlatformModuleInformation = 162,
	SystemKernelDebuggerFlags = 163,
	SystemCodeIntegrityPolicyInformation = 164,
	SystemIsolatedUserModeInformation = 165,
	SystemHardwareSecurityTestInterfaceResultsInformation = 166,
	SystemSingleModuleInformation = 167,
	SystemAllowedCpuSetsInformation = 168,
	SystemVsmProtectionInformation = 169,
	SystemInterruptCpuSetsInformation = 170,
	SystemSecureBootPolicyFullInformation = 171,
	SystemCodeIntegrityPolicyFullInformation = 172,
	SystemAffinitizedInterruptProcessorInformation = 173,
	SystemRootSiloInformation = 174,
	SystemCpuSetInformation = 175,
	SystemCpuSetTagInformation = 176,
	SystemWin32WerStartCallout = 177,
	SystemSecureKernelProfileInformation = 178,
	SystemCodeIntegrityPlatformManifestInformation = 179,
	SystemInterruptSteeringInformation = 180,
	SystemSupportedProcessorArchitectures = 181,
	SystemMemoryUsageInformation = 182,
	SystemCodeIntegrityCertificateInformation = 183,
	SystemPhysicalMemoryInformation = 184,
	SystemControlFlowTransition = 185,
	SystemKernelDebuggingAllowed = 186,
	SystemActivityModerationExeState = 187,
	SystemActivityModerationUserSettings = 188,
	SystemCodeIntegrityPoliciesFullInformation = 189,
	SystemCodeIntegrityUnlockInformation = 190,
	SystemIntegrityQuotaInformation = 191,
	SystemFlushInformation = 192,
	SystemProcessorIdleMaskInformation = 193,
	SystemSecureDumpEncryptionInformation = 194,
	SystemWriteConstraintInformation = 195,
	SystemKernelVaShadowInformation = 196,
	SystemHypervisorSharedPageInformation = 197,
	SystemFirmwareBootPerformanceInformation = 198,
	SystemCodeIntegrityVerificationInformation = 199,
	SystemFirmwarePartitionInformation = 200,
	SystemSpeculationControlInformation = 201,
	SystemDmaGuardPolicyInformation = 202,
	SystemEnclaveLaunchControlInformation = 203,
	SystemWorkloadAllowedCpuSetsInformation = 204,
	SystemCodeIntegrityUnlockModeInformation = 205,
	SystemLeapSecondInformation = 206,
	SystemFlags2Information = 207,
	SystemSecurityModelInformation = 208,
	SystemCodeIntegritySyntheticCacheInformation = 209,
	SystemFeatureConfigurationInformation = 210,
	SystemFeatureConfigurationSectionInformation = 211,
	SystemFeatureUsageSubscriptionInformation = 212,
	SystemSecureSpeculationControlInformation = 213,
	SystemSpacesBootInformation = 214,
	SystemFwRamdiskInformation = 215,
	SystemWheaIpmiHardwareInformation = 216,
	SystemDifSetRuleClassInformation = 217,
	SystemDifClearRuleClassInformation = 218,
	SystemDifApplyPluginVerificationOnDriver = 219,
	SystemDifRemovePluginVerificationOnDriver = 220,
	SystemShadowStackInformation = 221,
	SystemBuildVersionInformation = 222,
	SystemPoolLimitInformation = 223,
	SystemCodeIntegrityAddDynamicStore = 224,
	SystemCodeIntegrityClearDynamicStores = 225,
	SystemPoolZeroingInformation = 227,
	MaxSystemInfoClass = 228
}SYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_MODULE_ENTRY
{
	HANDLE Section;				//0x0000(0x0008)
	PVOID MappedBase;			//0x0008(0x0008)
	PVOID ImageBase;			//0x0010(0x0008)
	ULONG ImageSize;			//0x0018(0x0004)
	ULONG Flags;				//0x001C(0x0004)
	USHORT LoadOrderIndex;		//0x0020(0x0002)
	USHORT InitOrderIndex;		//0x0022(0x0002)
	USHORT LoadCount;			//0x0024(0x0002)
	USHORT OffsetToFileName;	//0x0026(0x0002)
	UCHAR FullPathName[256];	//0x0028(0x0100)
} SYSTEM_MODULE_ENTRY, * PSYSTEM_MODULE_ENTRY;

typedef struct _SYSTEM_MODULE_INFORMATION
{
	ULONG               Count;
	SYSTEM_MODULE_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;

typedef struct _DEBUG_OBJECT {
	KEVENT		EventPresent;
	FAST_MUTEX	Mutex;
	LIST_ENTRY	EventList;
}DEBUG_OBJECT,*PDEBUG_OBJECT;

typedef struct _PiDDBCacheEntry{
	LIST_ENTRY		List;
	UNICODE_STRING	DriverName;
	LARGE_INTEGER	TimeStamp;
	NTSTATUS		LoadStatus;
}PiDDBCacheEntry,*PPiDDBCacheEntry;

typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX
{
	PVOID Object;
	DWORD64 UniqueProcessId;
	DWORD64 HandleValue;
	ULONG	GrantedAccess;
	USHORT	CreatorBackTraceIndex;
	USHORT	ObjectTypeIndex;
	ULONG	HandleAttributes;
	ULONG	Reserved;
}SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX,*PSYSTEM_HANDLE_TABLE_ENTRY_INFO_EX;

typedef struct _SYSTEM_HANDLE_INFORMATION_EX
{
	DWORD64 NumberOfHandles;
	PVOID   Reserved;
	SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX Handles[1];
}SYSTEM_HANDLE_INFORMATION_EX, * PSYSTEM_HANDLE_INFORMATION_EX;

typedef struct _LDR_DATA_TABLE_ENTRY
{
	struct _LIST_ENTRY InLoadOrderLinks;                                    //0x0
	struct _LIST_ENTRY InMemoryOrderLinks;                                  //0x10
	struct _LIST_ENTRY InInitializationOrderLinks;                          //0x20
	VOID* DllBase;                                                          //0x30
	VOID* EntryPoint;                                                       //0x38
	ULONG SizeOfImage;                                                      //0x40
	struct _UNICODE_STRING FullDllName;                                     //0x48
	struct _UNICODE_STRING BaseDllName;                                     //0x58
	union
	{
		UCHAR FlagGroup[4];                                                 //0x68
		ULONG Flags;                                                        //0x68
		struct
		{
			ULONG PackagedBinary : 1;                                         //0x68
			ULONG MarkedForRemoval : 1;                                       //0x68
			ULONG ImageDll : 1;                                               //0x68
			ULONG LoadNotificationsSent : 1;                                  //0x68
			ULONG TelemetryEntryProcessed : 1;                                //0x68
			ULONG ProcessStaticImport : 1;                                    //0x68
			ULONG InLegacyLists : 1;                                          //0x68
			ULONG InIndexes : 1;                                              //0x68
			ULONG ShimDll : 1;                                                //0x68
			ULONG InExceptionTable : 1;                                       //0x68
			ULONG ReservedFlags1 : 2;                                         //0x68
			ULONG LoadInProgress : 1;                                         //0x68
			ULONG LoadConfigProcessed : 1;                                    //0x68
			ULONG EntryProcessed : 1;                                         //0x68
			ULONG ProtectDelayLoad : 1;                                       //0x68
			ULONG ReservedFlags3 : 2;                                         //0x68
			ULONG DontCallForThreads : 1;                                     //0x68
			ULONG ProcessAttachCalled : 1;                                    //0x68
			ULONG ProcessAttachFailed : 1;                                    //0x68
			ULONG CorDeferredValidate : 1;                                    //0x68
			ULONG CorImage : 1;                                               //0x68
			ULONG DontRelocate : 1;                                           //0x68
			ULONG CorILOnly : 1;                                              //0x68
			ULONG ChpeImage : 1;                                              //0x68
			ULONG ReservedFlags5 : 2;                                         //0x68
			ULONG Redirected : 1;                                             //0x68
			ULONG ReservedFlags6 : 2;                                         //0x68
			ULONG CompatDatabaseProcessed : 1;                                //0x68
		};
	};
	USHORT ObsoleteLoadCount;                                               //0x6c
	USHORT TlsIndex;                                                        //0x6e
	struct _LIST_ENTRY HashLinks;                                           //0x70
	ULONG TimeDateStamp;                                                    //0x80
	struct _ACTIVATION_CONTEXT* EntryPointActivationContext;                //0x88
	VOID* Lock;                                                             //0x90
	struct _LDR_DDAG_NODE* DdagNode;                                        //0x98
	struct _LIST_ENTRY NodeModuleLink;                                      //0xa0
	struct _LDRP_LOAD_CONTEXT* LoadContext;                                 //0xb0
	VOID* ParentDllBase;                                                    //0xb8
	VOID* SwitchBackContext;                                                //0xc0
	struct _RTL_BALANCED_NODE BaseAddressIndexNode;                         //0xc8
	struct _RTL_BALANCED_NODE MappingInfoIndexNode;                         //0xe0
	ULONGLONG OriginalBase;                                                 //0xf8
	union _LARGE_INTEGER LoadTime;                                          //0x100
	ULONG BaseNameHashValue;                                                //0x108
	enum _LDR_DLL_LOAD_REASON LoadReason;                                   //0x10c
	ULONG ImplicitPathOptions;                                              //0x110
	ULONG ReferenceCount;                                                   //0x114
	ULONG DependentLoadFlags;                                               //0x118
	UCHAR SigningLevel;                                                     //0x11c
}LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

typedef NTSTATUS(NTAPI* NtQuerySystemInformation_t)(
	IN SYSTEM_INFORMATION_CLASS		SystemInformationClass,
	IN OUT PVOID					SystemInformaiton,
	IN ULONG						SystemInformationLenght,
	OUT PULONG						ReturnLength OPTIONAL
	);

typedef NTSTATUS(NTAPI* NtQueryInformationProcess_t)(
	IN	HANDLE				ProcessHandle,
	IN	PROCESSINFOCLASS	ProcessInformationClass,
	OUT PVOID				ProcessInformation,
	IN	ULONG				ProcessInformationLength,
	OUT PULONG				ReturnLength
	);

typedef PCHAR(NTAPI* PsGetProcessImageFileName_t)(
	IN PEPROCESS Process
	);

typedef PVOID(NTAPI* ObGetObjectType_t)(
	IN PEPROCESS Process
	);


#endif
