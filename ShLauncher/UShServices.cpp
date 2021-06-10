#include "UShInc.h"

bool ShService::InitializeService()
{
    char TempBuffer[MAX_PATH] = { 0, };
    DWORD ret = GetCurrentDirectory(MAX_PATH, TempBuffer);
    if (ret == 0) { return false; }
    DriverPath = std::string(TempBuffer);
    DriverPath = DriverPath + "\\" + DRIVER_NAME;
    ServiceName = SERVICE_NAME;
    LinkName    = LINK_NAME;

    SCManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
    if (SCManager == nullptr) { ErrLog("Can't open service control manager..%d\n", GetLastError()); return false; }
    
    if (IsExist() == false)
    {
        Log("Create Service...\n");
        ServiceHandle = CreateService(
            SCManager, ServiceName.c_str(), ServiceName.c_str(),
            SC_MANAGER_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START,
            SERVICE_ERROR_IGNORE, DriverPath.c_str(),
            nullptr, nullptr, nullptr, nullptr, nullptr
        );
        if (ServiceHandle == nullptr) 
        { 
            ErrLog("Can't create(open) service..%d\n", GetLastError());
            CloseHandle(SCManager);
            
            return false;
        }
    }

    return true;
}

bool ShService::IsExist()
{
    ServiceHandle = OpenService(
        SCManager,
        ServiceName.c_str(),
        SC_MANAGER_ALL_ACCESS
    );
    if (ServiceHandle == nullptr) { return false; }
    return true;
}

bool ShService::RunService()
{
    if (StartService(ServiceHandle, 0, nullptr) == false)
    {
        ErrLog("Can't start service..%d\n", GetLastError());
		CloseHandle(SCManager);
		CloseHandle(ServiceHandle);
        return false;
    }
    
    return true;
}

void ShService::StopService()
{
    SERVICE_STATUS Status;
    ControlService(
        ServiceHandle,
        SERVICE_CONTROL_STOP,
        &Status
    );

    CloseHandle(SCManager);
    CloseHandle(ServiceHandle);
}

bool ShService::CheckService()
{
	if (ServiceHandle == nullptr)
	{
		if (InitializeService() == false) { ErrLog("Can't get service handle\n"); return false; }
	}
    SERVICE_STATUS_PROCESS Status = { 0, };
    DWORD ReturnLength = 0;
    if (QueryServiceStatusEx(
        ServiceHandle,
        SC_STATUS_PROCESS_INFO,
        (LPBYTE)&Status,
        sizeof(SERVICE_STATUS_PROCESS),
        &ReturnLength
    ) == false)
    {
        ErrLog("Can't get service information..%d\n", GetLastError()); return false;
    }

    if (Status.dwCurrentState == SERVICE_RUNNING) { return true; }
    return false;
}

void ShService::SendControl(int mode)
{
    if (CheckService() == false) { return; }
    HANDLE DeviceHandle = nullptr;
    DWORD ret = 0;

    DeviceHandle = CreateFile(
        LinkName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );
    if (DeviceHandle == nullptr) { ErrLog("Can't get device handle..%d\n", GetLastError()); return; }

    switch (mode)
    {
    case DeviceIoControl_SystemBuffer:
    {
        unsigned int inBuffer = 0x1337;
        unsigned int outBuffer = 0;
	    DeviceIoControl(
			DeviceHandle,
			IOCTL_SHD_SYSTEM_BUFFER,
			&inBuffer,
			sizeof(unsigned int),
			&outBuffer,
			sizeof(unsigned int),
			&ret,
			nullptr
		);
        break;
    }

    case DeviceIoControl_MDL:
    {
        unsigned int inBuffer = 0x1337;
        unsigned int outBuffer = 0;
        Log("%p\n", &outBuffer);
        system("pause");
		DeviceIoControl(
			DeviceHandle,
			IOCTL_SHD_MDL,
			&inBuffer,
			sizeof(unsigned int),
			&outBuffer,
			sizeof(unsigned int),
			&ret,
			nullptr
		);
        Log("0x%X\n", outBuffer);
        break;
    }

    case DeviceIoControl_UserBuffer:
    {
		unsigned int inBuffer = 0x1337;
        unsigned int outBuffer = 0;
		Log("InBuffer : 0x%X    OutBuffer : 0x%X\n", &inBuffer, &outBuffer);

		DeviceIoControl(
			DeviceHandle,
			IOCTL_SHD_USER_BUFFER,
			&inBuffer,
			sizeof(unsigned int),
			&outBuffer,
			sizeof(unsigned int),
			&ret,
			nullptr
		);
		Log("0x%X\n", outBuffer);
        break;
    }

    case DebugObject:
    {
        ULONG pid = 0;
        OBJECT_REF obTest = { 0, };
        std::cout << "[*] Input PID : ";
        std::cin >> pid;
		DeviceIoControl(
			DeviceHandle,
			IOCTL_SHD_SYSTEM_BUFFER,
			&pid,
			sizeof(ULONG),
			&obTest,
			sizeof(OBJECT_REF),
			&ret,
			nullptr
		);
        Log("Object Header : 0x%p Object : 0x%p Type Index : 0x%X\n", obTest.ObjectHeader, obTest.Object, obTest.TypeIndex);
        break;
    }

    }

   
    CloseHandle(DeviceHandle);
}
