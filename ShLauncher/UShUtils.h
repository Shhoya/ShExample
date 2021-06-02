#ifndef _USHUTILS_H_
#define _USHUTILS_H_

#define Log(...) printf( "[LOG] " __VA_ARGS__ )
#define WarnLog(...) printf( "[WARN] " __VA_ARGS__ )
#define ErrLog(...) printf( "\n[ERROR] " __VA_ARGS__ )
#define Blue SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#define Green SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#define Gray SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#define Red  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
#define Yellow SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
#define White SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#define Purple SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

enum MenuSel {
	Exit,
	DriverLoad,
	DriverUnlod,
	DeviceIoControl_SystemBuffer,
	DeviceIoControl_MDL,
	DeviceIoControl_UserBuffer,
	ObjectRefCountTest
};

namespace ShUtils {
	void PrintMenu();
	bool SelParser(int num, ShService* service);
}

#endif
