#include <stdio.h>
#include <conio.h>
#include <windows.h>

CRITICAL_SECTION cs;

DWORD WINAPI print(LPVOID param);
DWORD WINAPI print1(LPVOID param);
DWORD WINAPI print2(LPVOID param);
DWORD WINAPI print3(LPVOID param);

HANDLE hSemaphores[3];
#define SAFE_DELETE_HANDLE(a) if(a){CloseHandle(a);}

int main()
{
	DWORD print1ID, print2ID, print3ID;
	HANDLE hPrint1, hPrint2, hPrint3;

	//hPrint1 = CreateThread(NULL, 0, &print1, NULL, 0, &print1ID);
	//hPrint2 = CreateThread(NULL, 0, &print2, NULL, 0, &print2ID);
	//hPrint3 = CreateThread(NULL, 0, &print3, NULL, 0, &print3ID);

	char c1 = '1';
	char c2 = '2';
	char c3 = '3';
	hPrint1 = CreateThread(NULL, 0, &print, &c1, 0, &print1ID);
	hPrint2 = CreateThread(NULL, 0, &print, &c2, 0, &print2ID);
	hPrint3 = CreateThread(NULL, 0, &print, &c3, 0, &print3ID);

	int liT = getchar();

	CloseHandle(hPrint1);
	CloseHandle(hPrint2);
	CloseHandle(hPrint3);
}

DWORD WINAPI print(LPVOID param) {
	//char c = *(char*)param;
	//for (int i = 0; i < 1000; i++) printf("%c", c);

	int n = (int)param;
	for (int i = 0; i < 1000; i++) {
		WaitForSingleObject(hSemaphores[n], INFINITE);
		print("%d", n + 1);
		ReleaseSemaphore(hSemaphores[(n + 1) % 3], 1, NULL);
	}
}
DWORD WINAPI print1(LPVOID param) {
	for (int i = 0; i < 1000; i++) {
		//printf("1");
		EnterCriticalSection(&cs);
		printf("Zvezda");
		printf(" je");
		printf(" prvak");
		printf(" sveta\n");
		LeaveCriticalSection(&cs);
	}
}
DWORD WINAPI print2(LPVOID param) {
	for (int i = 0; i < 1000; i++) {
		//printf("2");
		EnterCriticalSection(&cs);
		printf("Priprema");
		printf(" vezbe");
		printf(" iz ");
		printf(" Sistemske");
		printf(" Programske");
		printf(" Podrske\n");
		LeaveCriticalSection(&cs);
	}
}
DWORD WINAPI print3(LPVOID param) {
	for (int i = 0; i < 1000; i++) {
		//printf("3");
		EnterCriticalSection(&cs);
		printf("Danas");
		printf(" je");
		printf(" lep");
		printf(" i");
		printf(" suncan");
		printf(" dan \n");
		LeaveCriticalSection(&cs);
	}
}