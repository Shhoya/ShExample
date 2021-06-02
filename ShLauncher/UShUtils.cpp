#include "UShInc.h"

void ShUtils::PrintMenu()
{
	printf("[1] Driver Load\n");
	printf("[2] Driver Unload\n");
	printf("[3] DeviceIoControl(SystemBuffer)\n");
	printf("[4] DeviceIoControl(MDL)\n");
	printf("[5] DeviceIoControl(UserBuffer)\n");
	printf("[6] Test Object reference count(SystemBuffer)\n");
	printf("[0] Exit\n");
}

bool ShUtils::SelParser(int num, ShService* service)
{
	switch (num)
	{
	case DriverLoad:
	{
		Log("Driver Load\n\n");
		service->RunService();
		break;
	}

	case DriverUnlod:
	{
		Log("Driver Unload\n\n");
		service->StopService();
		break;
	}

	case DeviceIoControl_SystemBuffer:
	{
		Log("DeviceIoControl_SystemBuffer\n");
		service->SendControl(num);
		break;
	}

	case DeviceIoControl_MDL:
	{
		Log("DeviceIoControl_MDL\n");
		service->SendControl(num);
		break;
	}

	case DeviceIoControl_UserBuffer:
	{
		Log("DeviceIoControl_UserBuffer\n");
		service->SendControl(num);
		break;
	}

	case ObjectRefCountTest:
	{
		Log("Object reference count test\n");
		service->SendControl(num);
		break;
	}

	default: return false;
		
	}

	return true;
}
