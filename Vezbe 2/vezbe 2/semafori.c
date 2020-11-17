#include <stdio.h>
#include <conio.h>
#include <windows.h>

HANDLE hSemaphore;
int counter;

DWORD WINAPI ThreadProc(LPVOID param);

int main() {
	DWORD threadID;
	HANDLE hThread;

	hSemaphore = CreateSemaphore(0, 0, 1, NULL);
	hThread = CreateThread(NULL, 0, &ThreadProc, NULL, 0, &threadID);

	while (1) {
		if (_kbhit()) {
			if (_getch() == 'q') break;
			ReleaseSemaphore(hSemaphore, 1, NULL);
		}
		Sleep(100);
	}
	CloseHandle(hThread);
	CloseHandle(hSemaphore);

	_getch();
	return 0;
}

DWORD WINAPI ThreadProc(LPVOID param) {
	while (1) {
		WaitForSingleObject(hSemaphore, INFINITE);
		printf("\r counter = %d\n", ++counter);
	}
}