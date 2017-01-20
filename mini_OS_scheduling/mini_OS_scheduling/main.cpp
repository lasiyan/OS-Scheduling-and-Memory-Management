// main.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "main.h"
#include "thread.h"
#include "scheduling.h"
#include "memoryManage.h"

void main(int argc, char **argv)
{
	HANDLE *hThread = new HANDLE[MAX_Thread];	// MAX_Thread만큼 핸들값 얻음

	srand((unsigned)time(NULL));				// Thread_Priority_Set 함수 사용하기 위해
	timeBeginPeriod((unsigned int)1);			// 멀티미디어 타이머 정밀도 설정

	// 프로세스 정보 출력
	Process_Info();

	printf("## 타이머 해상도 설정 ##\n");
	if (timeGetDevCaps(&resolution, sizeof(TIMECAPS)) == TIMERR_NOERROR)	// 타이머 해상도 측정
	{
		printf("최소 지원 해상도 : %d\n", resolution.wPeriodMin);
		printf("최대 지원 해상도 : %d\n", resolution.wPeriodMax);
	}
	timer = time(NULL);
	t = localtime(&timer);		// year 부터 second까지 timer구조체에 시스템 현재 시간 저장
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
	HANDLE ph = OpenProcess(MAXIMUM_ALLOWED, TRUE, pid);	// 현재 프로세서의 핸들값 저장
	PROCESS_MEMORY_COUNTERS pmc;
	if (GetProcessMemoryInfo(ph, &pmc, sizeof(pmc))) {

		printf("\n	|------------------------------------|\n");
		printf("	| 현재 프로세스 정보		     |\n");
		printf("	|------------------------------------|\n");
		printf("	| HANDLE : 0x%08X		     |\n", ph);
		printf("	| ID : %-8d			     |\n", pid);
		printf("	| Name : cmd.exe		     |\n");
		printf("	| Path : C:\\WINDOWS\\system32\\cmd.exe |\n");
		printf("	| 총 스레드 개수 : %d		     |\n", MAX_Thread + MEMORY_ALLOC_SET);
		printf("	| 가상 메모리 : 0x%08X	     |\n", pmc.PagefileUsage);
		printf("	| 메모리 사용량 : 0x%08X         |\n", pmc.WorkingSetSize);
		printf("	|------------------------------------|\n\n");
		printf("        스레드를 실행시키려면 아무키나 눌러주세요\n");

		_getch();
		system("cls");
	}
	CloseHandle(ph);
}