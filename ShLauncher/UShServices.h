#ifndef _USHSERVICES_H_
#define _USHSERVICES_H_

#define SERVICE_NAME "Shh0yaDriver"
#define DRIVER_NAME  "ShExample1.sys"
#define LINK_NAME	 "\\\\.\\ShDrv"
#define MAX_NAME 128

#define IO_SYSTEM_BUFFER 0x800
#define IO_MDL			 0x900
#define IO_USER_BUFFER   0x1000

#define IOCTL_SHD_SYSTEM_BUFFER CTL_CODE(FILE_DEVICE_UNKNOWN, IO_SYSTEM_BUFFER, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_SHD_MDL			CTL_CODE(FILE_DEVICE_UNKNOWN, IO_MDL, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#define IOCTL_SHD_USER_BUFFER	CTL_CODE(FILE_DEVICE_UNKNOWN, IO_USER_BUFFER, METHOD_NEITHER, FILE_ANY_ACCESS)

typedef struct _OBJECT_REF {
	PVOID ObjectHeader;
	PVOID Object;
	ULONG TypeIndex;
	ULONG RefCount;
}OBJECT_REF, * POBJECT_REF;

typedef struct _DEBUGGER_INFO {
	ULONG DebuggerId;
	ULONG DebuggeeId;
}DEBUGGER_INFO,*PDEBUGGER_INFO;

class ShService {
private:
	SC_HANDLE		SCManager;
	SC_HANDLE		ServiceHandle;
	std::string		ServiceName;
	std::string		LinkName;
	std::string		DriverPath;
public:
	bool InitializeService();
	bool IsExist();

	bool RunService();
	void StopService();
	bool CheckService();
	void SendControl(int mode);
};

#endif
