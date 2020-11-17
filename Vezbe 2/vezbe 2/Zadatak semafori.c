#include <stdio.h>
#include <conio.h>
#include <windows.h>

#define RING_SIZE  (16)
#define SLEEPING_TIME 400
#define SEMAPHORE_NUM 2

#define SAFE_DELETE_HANDLE(a)  if(a){CloseHandle(a);}

typedef struct buffer {
	unsigned int tail;
	unsigned int head;
	unsigned char data[RING_SIZE];
} RingBuffer;

RingBuffer ring1;
RingBuffer ring2;

HANDLE Empty;
HANDLE Full;
HANDLE FinishSignal;
HANDLE Process;

CRITICAL_SECTION BufferAccess;

DWORD WINAPI producer(LPVOID param);

DWORD WINAPI consumer(LPVOID param);

DWORD WINAPI exitFunc(LPVOID param);

char ringBufGetChar(RingBuffer* apBuffer);

void ringBufPutChar(RingBuffer* apBuffer, const char c);

int main() {
	HANDLE hEntry = NULL, hProgram = NULL, hExit = NULL;
	DWORD EntryID, ProgramID, ExitID;

	Empty = CreateSemaphore(0, RING_SIZE, RING_SIZE, NULL);
	Full = CreateSemaphore(0, 0, RING_SIZE, NULL);
	Process = CreateSemaphore(0, 0, RING_SIZE, NULL);
	FinishSignal = CreateSemaphore(0, 0, 3, NULL);

	if (Empty && Full && FinishSignal && Process) {
		InitializeCriticalSection(&BufferAccess);

		hEntry = CreateThread(NULL, 0, &producer, (LPVOID)0, 0, &EntryID);
		hProgram = CreateThread(NULL, 0, &consumer, (LPVOID)0, 0, &ProgramID);
		hExit = CreateThread(NULL, 0, &exitFunc, (LPVOID)0, 0, &ExitID);

		if (!hEntry || !hProgram || !hExit) {
			ReleaseSemaphore(FinishSignal, 3, NULL);
		}

		if (hProgram) WaitForSingleObject(hProgram, INFINITE);
		if (hEntry) WaitForSingleObject(hEntry, INFINITE);
		if (hExit) WaitForSingleObject(hExit, INFINITE);
	}

	SAFE_DELETE_HANDLE(hEntry);
	SAFE_DELETE_HANDLE(hProgram);
	SAFE_DELETE_HANDLE(hExit);
	SAFE_DELETE_HANDLE(Empty);
	SAFE_DELETE_HANDLE(Full);
	SAFE_DELETE_HANDLE(FinishSignal);

	DeleteCriticalSection(&BufferAccess);
	return 0;
}

DWORD WINAPI producer(LPVOID param) {
	char c;
	HANDLE semaphores[SEMAPHORE_NUM] = { FinishSignal, Empty };

	while (WaitForMultipleObjects(SEMAPHORE_NUM, semaphores, FALSE, INFINITE) == WAIT_OBJECT_0 + 1) {
		c = _getch();

		if (c == 'q' || c == 'Q') ReleaseSemaphore(FinishSignal, 2, NULL);

		EnterCriticalSection(&BufferAccess);
		ringBufPutChar(&ring1, c);
		LeaveCriticalSection(&BufferAccess);

		ReleaseSemaphore(Full, 1, NULL);
	}

	return 0;
}

DWORD WINAPI consumer(LPVOID param) {
	char c;

	HANDLE semaphores[SEMAPHORE_NUM] = { FinishSignal, Full };

	while (WaitForMultipleObjects(SEMAPHORE_NUM, semaphores, FALSE, INFINITE) == WAIT_OBJECT_0 + 1) {
		EnterCriticalSection(&BufferAccess);

		c = ringBufGetChar(&ring1);
		c -= 0x20;
		ringBufPutChar(&ring2, c);
		LeaveCriticalSection(&BufferAccess);

		ReleaseSemaphore(Process, 1, NULL);

		Sleep(SLEEPING_TIME);
	}

	return 0;
}

DWORD WINAPI exitFunc(LPVOID param) {
	char c;
	printf("Characters got from the circular buffer: \n");

	HANDLE semaphores[SEMAPHORE_NUM] = { FinishSignal, Process };

	while (WaitForMultipleObjects(SEMAPHORE_NUM, semaphores, FALSE, INFINITE)) {
		EnterCriticalSection(&BufferAccess);

		c = ringBufGetChar(&ring2);
		LeaveCriticalSection(&BufferAccess);
		printf("%c\n", c);

		ReleaseSemaphore(Empty, 1, NULL);
		Sleep(SLEEPING_TIME);
	}

	return 0;
}

char ringBufGetChar(RingBuffer* apBuffer) {
	int index;
	index = apBuffer->head;
	apBuffer->head = (apBuffer->head + 1) % RING_SIZE;
	return apBuffer->data[index];
}

void ringBufPutChar(RingBuffer* apBuffer, const char c) {
	apBuffer->data[apBuffer->tail] = c;
	apBuffer->tail = (apBuffer->tail + 1) % RING_SIZE;
}