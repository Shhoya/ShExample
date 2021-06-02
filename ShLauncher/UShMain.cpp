#include "UShInc.h"

using namespace std;

int main()
{
	ShService service;
	
	unsigned int num = 0;
	while (true)
	{
		if (service.InitializeService() == false)
		{
			ErrLog("Initialize failed\n");
			return -1;
		}

		ShUtils::PrintMenu();
		cout << endl << "[*] Select num : ";
		cin >> num;
		if (ShUtils::SelParser(num, &service) == false)
		{
			break;
		}
		system("pause");
		system("cls");

	}
	return 0;
}