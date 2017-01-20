// main.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//
#include "stdafx.h"
#include "main.h"
#include "thread.h"
#include "scheduling.h"
#include "memoryManage.h"

void main(int argc, char **argv)
{
	HANDLE *hThread = new HANDLE[MAX_Thread];	// MAX_Thread��ŭ �ڵ鰪 ����

	srand((unsigned)time(NULL));				// Thread_Priority_Set �Լ� ����ϱ� ����
	timeBeginPeriod((unsigned int)1);			// ��Ƽ�̵�� Ÿ�̸� ���е� ����

	// ���μ��� ���� ���
	Process_Info();

	printf("## Ÿ�̸� �ػ� ���� ##\n");
	if (timeGetDevCaps(&resolution, sizeof(TIMECAPS)) == TIMERR_NOERROR)	// Ÿ�̸� �ػ� ����
	{
		printf("�ּ� ���� �ػ� : %d\n", resolution.wPeriodMin);
		printf("�ִ� ���� �ػ� : %d\n", resolution.wPeriodMax);
	}
	timer = time(NULL);
	t = localtime(&timer);		// year ���� second���� timer����ü�� �ý��� ���� �ð� ����
	Sleep(1000);

	// Create threads using in scheduling
	create_Scheduling_thread();
	Sleep(1000);

	// scheduling Start
	scheduling_start();

	// Memory Management Start
	memory_Management_start();
}

void Process_Info(void)
{
	DWORD pid = GetCurrentProcessId();
	HANDLE ph = OpenProcess(MAXIMUM_ALLOWED, TRUE, pid);	// ���� ���μ����� �ڵ鰪 ����
	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(ph, &pmc, sizeof(pmc))) {

		printf("\n	|------------------------------------|\n");
		printf("	| ���� ���μ��� ����		     |\n");
		printf("	|------------------------------------|\n");
		printf("	| HANDLE : 0x%08X		     |\n", ph);
		printf("	| ID : %-8d			     |\n", pid);
		printf("	| Name : cmd.exe		     |\n");
		printf("	| Path : C:\\WINDOWS\\system32\\cmd.exe |\n");
		printf("	| �� ������ ���� : %d		     |\n", MAX_Thread + MEMORY_ALLOC_SET);
		printf("	| ���� �޸� : 0x%08X	     |\n", pmc.PagefileUsage);
		printf("	| �޸� ��뷮 : 0x%08X         |\n", pmc.WorkingSetSize);
		printf("	|------------------------------------|\n\n");
		printf("        �����带 �����Ű���� �ƹ�Ű�� �����ּ���\n");

		_getch();
		system("cls");
	}
	CloseHandle(ph);
}