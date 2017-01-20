#pragma once

/* Thread and List */
typedef struct _Thread_Information
{
	DWORD Threadnum;		// 외부에서 사용할 구조체 번호를 담을 변수
	HANDLE hThread;			// 스레드 핸들을 담을 변수
	DWORD hThreadId = NULL;	// 스레드 ID를 담을 변수
	LONG hThread_priority;	// 스레드 우선순위를 담을 변수
	char Que_Info[10];
	float StartTime = 0;		// 스레드가 생성된 시간
	float OperationTime = 0;	// 스레드가 큐에서 작동된 시간
	float FinishTime = 0;		// 스레드가 작동을 끝낸 시간
}T_Info;
extern T_Info ti[MAX_Thread];

void create_Scheduling_thread();
int Thread_Priority_Set(void);			// 스레드 우선순위 <랜덤> 부여
DWORD WINAPI Worker_Function(LPVOID);	// 스레드 작동시 수행할 함수