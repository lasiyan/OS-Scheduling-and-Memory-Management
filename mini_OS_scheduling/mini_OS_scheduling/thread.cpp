#include "stdafx.h"
#include "thread.h"

/* time */
TIMECAPS resolution;			// Ÿ�̸� ���е� ������ ���� ����ü
extern float Program_Begin_Time;		// ���α׷��� ���۵� �ð��� ���� ����(����) // ����ð� = Ư���ð� - (����)
struct tm *t;					// tm�� ����ü������ t
time_t timer;					// �ð� ����(�ý��� ���� �ð� ����ü)

list <T_Info> h_que;			// High_Que List
list <T_Info> m_que;			// Middle_Que List
list <T_Info> l_que;			// Low_Que List

void create_Scheduling_thread() {
	printf("\n## ������ ���� ���� ##\n");
	for (int i = 0; i < MAX_Thread; i++)
	{

		ti[i].hThread = CreateThread(NULL, 0,
			Worker_Function, (LPVOID)i,
			CREATE_SUSPENDED, &ti[i].hThreadId); // �����·� ����

		SetThreadPriority(ti[i].hThread, Thread_Priority_Set());
		// Thread_Priority_Set�� ���� �켱���� ���� �ο�
		ti[i].hThread_priority = GetThreadPriority(ti[i].hThread);
		// hThread_priority�� ����
		Program_Begin_Time = (float)timeGetTime() * 0.001f;
		ti[i].StartTime = timeGetTime() * 0.001f - Program_Begin_Time;

		/* ������ �����带 3���� ť�� ���� �켱������ ���� ���� */
		switch (ti[i].hThread_priority) {
		case THREAD_PRIORITY_HIGHEST:			// 15
		case THREAD_PRIORITY_TIME_CRITICAL:		// 2
			ti[i].Threadnum = (DWORD)i;	// ������ ����ü�� ������ ��ȣ ����
			h_que.push_front(ti[i]);	// ���� ����ü�� ����Ʈ�� �� �տ� ����
			strcpy(ti[i].Que_Info, "HighQue");	// ����ü Que_Info�� ����Ʈ �̸� ����
			printf("%2d�� ������ => HighQue\n", i + 1);
			break;
		case THREAD_PRIORITY_BELOW_NORMAL:		// 1
		case THREAD_PRIORITY_NORMAL:			// 0
		case THREAD_PRIORITY_ABOVE_NORMAL:		// -1
			ti[i].Threadnum = (DWORD)i;
			m_que.push_front(ti[i]);
			strcpy(ti[i].Que_Info, "MiddleQue");
			printf("%2d�� ������ => MiddleQue\n", i + 1);
			break;
		case THREAD_PRIORITY_IDLE:				// -2
		case THREAD_PRIORITY_LOWEST:			// -15
			ti[i].Threadnum = (DWORD)i;
			l_que.push_front(ti[i]);
			strcpy(ti[i].Que_Info, "LowQue");
			printf("%2d�� ������ => LowQue\n", i + 1);

			break;
		}
	}
	printf("## ������ ���� �Ϸ� ##\n");
}

/* Thread struct and Process Function*/
DWORD WINAPI Worker_Function(LPVOID arg)
{
	static int min_temp = t->tm_min; int i = (int)arg;
	static float sec_temp = (float)t->tm_sec + ti[i].StartTime;
	// �����尡 ���۵� �ð� ����(static)
	// ������ ������ for������ �ϰ�ó���Ǳ⶧���� �����ð��� ���̰� ���� ���� ����
	if ((float)t->tm_sec + ti[i].OperationTime >= 60.00 ||
		(float)t->tm_sec + timeGetTime() * 0.001f - Program_Begin_Time >= 60.00)	// "��"�� 60.00�̻��̸�
	{
		if (t->tm_min + 1 != t->tm_min)	// ����ð��� "��"�� 1 �߰� 
			t->tm_min += 1;				//(�� 1�߰��� �ѹ� �̷���� ��� �߰��� ������ ����)
		t->tm_sec -= 60;				// ����ð��� "��"���� -60.00
	}
	printf("  %2d	%4d	%5d	   %3d	     %s	%2d %2d' %05.2f\"	%2d %2d' %05.2f\"	%2d %2d' %05.2f\"\n",
		i + 1, OpenThread(THREAD_ALL_ACCESS, FALSE, ti[i].hThreadId),// ������ ��ȣ, �ڵ鰪
		ti[i].hThreadId, ti[i].hThread_priority, ti[i].Que_Info,// ���̵�, �켱����, ť ����
		t->tm_hour, min_temp, sec_temp,		// ������ �����ð�
		t->tm_hour, t->tm_min, (float)t->tm_sec + ti[i].OperationTime,	// ť���� �۵��� �ð�
		t->tm_hour, t->tm_min, (float)t->tm_sec + timeGetTime() * 0.001f - Program_Begin_Time);	// �۵� �� ����� �ð�

	return 0;
}
int Thread_Priority_Set(void)			// �������� ������ �켱���� �ο� ( -15 ~ 15 ���� )
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