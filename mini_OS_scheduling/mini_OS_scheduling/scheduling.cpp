#include "stdafx.h"
#include "scheduling.h"

float Program_Begin_Time;

T_Info ti[MAX_Thread];

extern list <T_Info> h_que;			// High_Que List
extern list <T_Info> m_que;			// Middle_Que List
extern list <T_Info> l_que;			// Low_Que List

void scheduling_start() {
	printf("\n## ť ��� ������ ���� ##\n");
	Sleep(1000);
	HANDLE HighQue_Thread = (HANDLE)CreateThread(NULL, 0, HighQue_func, NULL, 0, NULL); // ���� �� �ٷ� ���� - no suspend
	HANDLE MiddleQue_Thread = (HANDLE)CreateThread(NULL, 0, MiddleQue_func, NULL, 0, NULL);
	HANDLE LowQue_Thread = (HANDLE)CreateThread(NULL, 0, LowQue_func, NULL, 0, NULL);
	printf(" ��ȣ	�ڵ�	���̵�	 �켱����    ť ����	  �����ð�	  �۵��ð�	  ����ð�\n");
	printf("---------------------------------------------------------------------------------------------\n");
	//Sleep(1000);

	WaitForSingleObject(HighQue_Thread, INFINITE); //HighQue ��� ������ ��ٸ�
	WaitForSingleObject(MiddleQue_Thread, INFINITE); //MiddleQue ��� ������ ��ٸ�
	WaitForSingleObject(LowQue_Thread, INFINITE); //LowQue ��� ������ ��ٸ�

	WaitForMultipleObjects(MAX_Thread, (HANDLE*)ti, TRUE, INFINITE);
	Sleep(1000);	// ����ü ti�� �� ���������� ��ٸ���.

	for (int i = 0; i < MAX_Thread; i++)
		CloseHandle(ti[i].hThread);		// ����ü �� ������ �ڵ� ����
	CloseHandle(HighQue_Thread);
	CloseHandle(MiddleQue_Thread);
	CloseHandle(LowQue_Thread);
	printf("## ������ ���� �Ϸ� ##\n"); Sleep(1000);
}

/* List Function */
DWORD WINAPI HighQue_func(LPVOID arg)
{
	UNREFERENCED_PARAMETER(arg);
	int htemp;
	while (!h_que.empty())
	{
		Sleep(500);				// while������ �ݺ����� ���� �ߺ���� ����
		htemp = h_que.back().Threadnum;	// ������ �����屸��ü�� ��ȣ�� �ӽ÷� ����
		ResumeThread(h_que.back().hThread);	// ����Ʈ�� ����ü(������) ����
		ti[htemp].OperationTime = timeGetTime() * 0.001f - Program_Begin_Time;
		// �����尡 �۵��� �ð� ����
		h_que.pop_back();		// ����Ʈ���� ������ ��� ����
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
		Sleep(1500);			// while������ �ݺ����� ���� �ߺ���� ����
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
		Sleep(2500);			// while������ �ݺ����� ���� �ߺ���� ����
		ltemp = l_que.back().Threadnum;
		ResumeThread(l_que.back().hThread);
		ti[ltemp].OperationTime = timeGetTime() * 0.001f - Program_Begin_Time;
		l_que.pop_back();
		Sleep(2500);
	}
	return 0;
}