#include "stdafx.h"
#include "thread.h"

/* time */
TIMECAPS resolution;			// 타이머 정밀도 정보를 담을 구조체
extern float Program_Begin_Time;		// 프로그램이 시작된 시간을 담을 변수(기준) // 경과시간 = 특정시간 - (기준)
struct tm *t;					// tm형 구조체포인터 t
time_t timer;					// 시간 측정(시스템 현재 시간 구조체)

list <T_Info> h_que;			// High_Que List
list <T_Info> m_que;			// Middle_Que List
list <T_Info> l_que;			// Low_Que List

void create_Scheduling_thread() {
	printf("\n## 스레드 생성 시작 ##\n");
	for (int i = 0; i < MAX_Thread; i++)
	{

		ti[i].hThread = CreateThread(NULL, 0,
			Worker_Function, (LPVOID)i,
			CREATE_SUSPENDED, &ti[i].hThreadId); // 대기상태로 생성

		SetThreadPriority(ti[i].hThread, Thread_Priority_Set());
		// Thread_Priority_Set을 통한 우선순위 랜덤 부여
		ti[i].hThread_priority = GetThreadPriority(ti[i].hThread);
		// hThread_priority에 저장
		Program_Begin_Time = (float)timeGetTime() * 0.001f;
		ti[i].StartTime = timeGetTime() * 0.001f - Program_Begin_Time;

		/* 생성된 스레드를 3개의 큐에 각각 우선순위에 따라 저장 */
		switch (ti[i].hThread_priority) {
		case THREAD_PRIORITY_HIGHEST:			// 15
		case THREAD_PRIORITY_TIME_CRITICAL:		// 2
			ti[i].Threadnum = (DWORD)i;	// 스레드 구조체에 스레드 번호 저장
			h_que.push_front(ti[i]);	// 현재 구조체를 리스트의 맨 앞에 저장
			strcpy(ti[i].Que_Info, "HighQue");	// 구조체 Que_Info에 리스트 이름 저장
			printf("%2d번 스레드 => HighQue\n", i + 1);
			break;
		case THREAD_PRIORITY_BELOW_NORMAL:		// 1
		case THREAD_PRIORITY_NORMAL:			// 0
		case THREAD_PRIORITY_ABOVE_NORMAL:		// -1
			ti[i].Threadnum = (DWORD)i;
			m_que.push_front(ti[i]);
			strcpy(ti[i].Que_Info, "MiddleQue");
			printf("%2d번 스레드 => MiddleQue\n", i + 1);
			break;
		case THREAD_PRIORITY_IDLE:				// -2
		case THREAD_PRIORITY_LOWEST:			// -15
			ti[i].Threadnum = (DWORD)i;
			l_que.push_front(ti[i]);
			strcpy(ti[i].Que_Info, "LowQue");
			printf("%2d번 스레드 => LowQue\n", i + 1);

			break;
		}
	}
	printf("## 스레드 생성 완료 ##\n");
}

/* Thread struct and Process Function*/
DWORD WINAPI Worker_Function(LPVOID arg)
{
	static int min_temp = t->tm_min; int i = (int)arg;
	static float sec_temp = (float)t->tm_sec + ti[i].StartTime;
	// 스레드가 시작된 시간 저장(static)
	// 스레드 생성은 for문에서 일괄처리되기때문에 생성시간의 차이가 거의 없기 때문
	if ((float)t->tm_sec + ti[i].OperationTime >= 60.00 ||
		(float)t->tm_sec + timeGetTime() * 0.001f - Program_Begin_Time >= 60.00)	// "초"가 60.00이상이면
	{
		if (t->tm_min + 1 != t->tm_min)	// 현재시간의 "분"에 1 추가 
			t->tm_min += 1;				//(단 1추가가 한번 이루어질 경우 추가로 더하지 않음)
		t->tm_sec -= 60;				// 현재시간의 "초"에서 -60.00
	}
	printf("  %2d	%4d	%5d	   %3d	     %s	%2d %2d' %05.2f\"	%2d %2d' %05.2f\"	%2d %2d' %05.2f\"\n",
		i + 1, OpenThread(THREAD_ALL_ACCESS, FALSE, ti[i].hThreadId),// 스레드 번호, 핸들값
		ti[i].hThreadId, ti[i].hThread_priority, ti[i].Que_Info,// 아이디, 우선순위, 큐 정보
		t->tm_hour, min_temp, sec_temp,		// 스레드 생성시간
		t->tm_hour, t->tm_min, (float)t->tm_sec + ti[i].OperationTime,	// 큐에서 작동한 시간
		t->tm_hour, t->tm_min, (float)t->tm_sec + timeGetTime() * 0.001f - Program_Begin_Time);	// 작동 후 종료된 시간

	return 0;
}
int Thread_Priority_Set(void)			// 랜덤으로 스레드 우선순위 부여 ( -15 ~ 15 까지 )
{
	double r = ((double)rand() / (double)(RAND_MAX + (double(1))));
	int M = 7;
	double x = (r*(double)M);
	int y = (int)x;
	int z = y + 1;

	switch (z) {
	case 1:
		return THREAD_PRIORITY_IDLE; break;
	case 2:
		return THREAD_PRIORITY_LOWEST; break;
	case 3:
		return THREAD_PRIORITY_BELOW_NORMAL; break;
	case 4:
		return THREAD_PRIORITY_NORMAL; break;
	case 5:
		return THREAD_PRIORITY_ABOVE_NORMAL; break;
	case 6:
		return THREAD_PRIORITY_HIGHEST; break;
	case 7:
		return THREAD_PRIORITY_TIME_CRITICAL; break;
	}

	return 0;
}