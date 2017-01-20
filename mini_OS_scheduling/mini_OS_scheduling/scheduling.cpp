#include "stdafx.h"
#include "scheduling.h"

float Program_Begin_Time;

T_Info ti[MAX_Thread];

extern list <T_Info> h_que;			// High_Que List
extern list <T_Info> m_que;			// Middle_Que List
extern list <T_Info> l_que;			// Low_Que List

void scheduling_start() {
	printf("\n## 큐 운영용 스레드 생성 ##\n");
	Sleep(1000);
	HANDLE HighQue_Thread = (HANDLE)CreateThread(NULL, 0, HighQue_func, NULL, 0, NULL); // 생성 시 바로 시작 - no suspend
	HANDLE MiddleQue_Thread = (HANDLE)CreateThread(NULL, 0, MiddleQue_func, NULL, 0, NULL);
	HANDLE LowQue_Thread = (HANDLE)CreateThread(NULL, 0, LowQue_func, NULL, 0, NULL);
	printf(" 번호	핸들	아이디	 우선순위    큐 정보	  생성시간	  작동시간	  종료시간\n");
	printf("---------------------------------------------------------------------------------------------\n");
	//Sleep(1000);

	WaitForSingleObject(HighQue_Thread, INFINITE); //HighQue 담당 쓰레드 기다림
	WaitForSingleObject(MiddleQue_Thread, INFINITE); //MiddleQue 담당 쓰레드 기다림
	WaitForSingleObject(LowQue_Thread, INFINITE); //LowQue 담당 쓰레드 기다림

	WaitForMultipleObjects(MAX_Thread, (HANDLE*)ti, TRUE, INFINITE);
	Sleep(1000);	// 구조체 ti가 다 끝날때까지 기다린다.

	for (int i = 0; i < MAX_Thread; i++)
		CloseHandle(ti[i].hThread);		// 구조체 속 스레드 핸들 종료
	CloseHandle(HighQue_Thread);
	CloseHandle(MiddleQue_Thread);
	CloseHandle(LowQue_Thread);
	printf("## 스레드 동작 완료 ##\n"); Sleep(1000);
}

/* List Function */
DWORD WINAPI HighQue_func(LPVOID arg)
{
	UNREFERENCED_PARAMETER(arg);
	int htemp;
	while (!h_que.empty())
	{
		Sleep(500);				// while문에서 반복으로 인한 중복출력 방지
		htemp = h_que.back().Threadnum;	// 실행할 스레드구조체의 번호를 임시로 저장
		ResumeThread(h_que.back().hThread);	// 리스트의 구조체(스레드) 실행
		ti[htemp].OperationTime = timeGetTime() * 0.001f - Program_Begin_Time;
		// 스레드가 작동된 시간 저장
		h_que.pop_back();		// 리스트에서 마지막 노드 제거
		Sleep(500);
	}
	return 0;
}
DWORD WINAPI MiddleQue_func(LPVOID arg)
{
	UNREFERENCED_PARAMETER(arg);
	int mtemp;
	while (!m_que.empty())
	{
		Sleep(1500);			// while문에서 반복으로 인한 중복출력 방지
		mtemp = m_que.back().Threadnum;
		ResumeThread(m_que.back().hThread);
		ti[mtemp].OperationTime = timeGetTime() * 0.001f - Program_Begin_Time;
		m_que.pop_back();
		Sleep(1500);
	}
	return 0;
}
DWORD WINAPI LowQue_func(LPVOID arg)
{
	UNREFERENCED_PARAMETER(arg);
	int ltemp;
	while (!l_que.empty())
	{
		Sleep(2500);			// while문에서 반복으로 인한 중복출력 방지
		ltemp = l_que.back().Threadnum;
		ResumeThread(l_que.back().hThread);
		ti[ltemp].OperationTime = timeGetTime() * 0.001f - Program_Begin_Time;
		l_que.pop_back();
		Sleep(2500);
	}
	return 0;
}