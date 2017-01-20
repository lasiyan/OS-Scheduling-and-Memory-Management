#pragma once

/* Thread and List */
typedef struct _Thread_Information
{
	DWORD Threadnum;		// �ܺο��� ����� ����ü ��ȣ�� ���� ����
	HANDLE hThread;			// ������ �ڵ��� ���� ����
	DWORD hThreadId = NULL;	// ������ ID�� ���� ����
	LONG hThread_priority;	// ������ �켱������ ���� ����
	char Que_Info[10];
	float StartTime = 0;		// �����尡 ������ �ð�
	float OperationTime = 0;	// �����尡 ť���� �۵��� �ð�
	float FinishTime = 0;		// �����尡 �۵��� ���� �ð�
}T_Info;
extern T_Info ti[MAX_Thread];

void create_Scheduling_thread();
int Thread_Priority_Set(void);			// ������ �켱���� <����> �ο�
DWORD WINAPI Worker_Function(LPVOID);	// ������ �۵��� ������ �Լ�