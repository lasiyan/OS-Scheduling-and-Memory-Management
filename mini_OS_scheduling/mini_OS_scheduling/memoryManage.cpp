#include "stdafx.h"
#include "memoryManage.h"

/* MemoryManagement and Mutex - Virtual Alloc and Mutex */
SYSTEM_INFO sysInfo;	//SYSTEM_INFO 구조체
HANDLE ghMutex;			// Mutex핸들 생성
char* MyMemory[MEMORY_ALLOC_SET] = { 0, };	// My Memroy 
int PageTable[MEMORY_ALLOC_SET][2] = { 0 };	// Virtual PageTable Creating
NODE Mthread[MEMORY_ALLOC_SET];				// Creating threads using in Memory Management
NODE* node_head = 0;
char* MyBuffer;			// My virtual Memory

int memory_Management_start() {
	printf("\n## 메모리 할당할 스레드 생성 ##\n");
	GetSystemInfo(&sysInfo);

	ghMutex = CreateMutex(NULL, FALSE, NULL);
	if (ghMutex == NULL)
		printf("뮤텍스 생성 오류\n");

	for (int j = 0; j < MEMORY_ALLOC_SET; j++) {
		Mthread[j].hthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MemoryManagement,
			(LPVOID)j, CREATE_SUSPENDED, NULL);
		Sleep(700);
		if (Mthread[j].hthread == NULL)	{
			printf("메모리 할당용 스레드 생성 오류!!\n");
			return 0;
		}
	}
	printf("## 메모리 할당할 스레드 생성 완료 ##\n");
	Print_Memory_Status("초기 메모리 상태 : "); Sleep(1000);

	printf("\n## 스레드 메모리 할당 시작 ##\n");
	printf("MyMemory : 256Mbyte, PageSize : 4kbyte, offset : 0x040\n");
	printf("\n   이름		크기		My논리주소		My물리주소\n");
	printf("------------------------------------------------------------------\n");
	for (int j = 0; j < MEMORY_ALLOC_SET; j++) {
		ResumeThread(Mthread[j].hthread);
		Sleep(700);
	}
	printf("## 스레드 메모리 할당 완료 ##\n");
	Print_Memory_Status("할당 후 메모리 상태 : ");

	printf("## 스레드 메모리 반환 시작 ##\n");
	for (int j = 0; j < MEMORY_ALLOC_SET; j++) {
		VirtualFree(Mthread[j].hthread, 0, MEM_RELEASE);
		myfree(MyMemory[j]);
		Sleep(700);
	}
	free(MyBuffer);
	Print_Memory_Status("반환 후 메모리 상태 : ");
	printf("## 메모리 반환 종료 ##\n");

	WaitForMultipleObjects(MEMORY_ALLOC_SET, (HANDLE*)Mthread, TRUE, INFINITE);
	
	for (int j = 0; j < MEMORY_ALLOC_SET; j++)
		CloseHandle(Mthread[j].hthread);	// 메모리 할당용 스레드 핸들 종료
	CloseHandle(ghMutex);	// Mutex 사용 핸들 종료
	Sleep(1000);
	printf("\n## 프로그램 종료 ##\n");
}

DWORD WINAPI MemoryManagement(LPVOID arg)
{
	DWORD dwWaitResult;	//뮤텍스 상태 저장할 변수
	int j = (int)arg;
	while (1) {
		dwWaitResult = WaitForSingleObject(ghMutex, INFINITE);

		switch (dwWaitResult) {	// WaitForSingleObject에서 리턴된 값으로 판단
		case WAIT_OBJECT_0:
			__try {
				Mthread[j].Size = (20 + j) * MB;		// 사이즈 임의 설정 / 20, 21, 22 ...MB
				
				/* 가상메모리 할당 - 미구현 */
				/*
				Mthread[j].hthread = VirtualAlloc(NULL, Mthread[j].Size, MEM_RESERVE, PAGE_READWRITE);
					// Mthread[j]에 가상메모리 할당 (예약)
				Mthread[j].hthread = VirtualAlloc(Mthread[j].hthread, Mthread[j].Size,
				MEM_COMMIT, PAGE_READWRITE);	// 가상메모리에 할당 확정
				memset(Mthread[j].hthread, '\0', Mthread[j].Size); // RAM에 생성된 size만큼 할당
				printf("Mthread[%d]	%-3dMB	0X%08X", (int)arg, (Mthread[j].Size / (1024 * 1024)), (Mthread + j));
					//VirtualFree(Mthread[j].hthread, 0, MEM_RELEASE);
				*/

				MyMemory[j] = (char*)Mymalloc(sizeof(char) * Mthread[j].Size);
				printf("Mthread[%d]	%-3dMB		0x%08p", j, Mthread[j].Size / (1024 * 1024), MyMemory[j]);

				/* Paging */
				PageTable[j][1] = (10 + 3 * j) * 1234;	// 가상주소의 페이지넘버 임의 설정
				printf("		0x%08p\n",
					Transfer_LA_to_PA((int)MyMemory[j], j));
			}
			__finally {
				if (!ReleaseMutex(ghMutex)) //ghMutex 핸들 사용
					printf("Wait for Create Thread..\n");
			}
			break;
		case WAIT_ABANDONED:	// 이미 뮤텍스 사용중이면
			return FALSE;
		}
		return TRUE;
	}

}
void Print_Memory_Status(const char *pszMsg)
{
	HANDLE hMe = NULL;
	PROCESS_MEMORY_COUNTERS mem;

	hMe = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());

	if (hMe && GetProcessMemoryInfo(hMe, &mem, sizeof(PROCESS_MEMORY_COUNTERS)))
	{
		printf("%s", pszMsg);
		printf("Working : %7ld bytes, PageFileUsage : %10ld bytes\n", mem.WorkingSetSize, mem.PagefileUsage);
	}
}

char* Transfer_LA_to_PA(UINT LAddr, int j)		// 물리주소에서 논리주수로 변환
{
	/* Transfer LAddr to PAddr logic
	offset = LAddr % PageSize
	pg num = LAddr / PageSize
	Frame  = [j] == PageTable[j]
	PAddr  = Frame * PageSize + offset;
	*/

	int PageSize = 4096;	// 4K
	int offset = LAddr % PageSize;
	int PageNum = LAddr / PageSize;
	int Frame = PageTable[j][1];

	LAddr = Frame * PageSize + offset;

	return Mthread[j].Address = (char*)LAddr;
}

/* Double-Linked List using in MyMemory */
node* search_free_node(size_t size)
{
	node* n = node_head;

	while (1)
	{
		/* 비어있는 노드중 사이즈보다 큰 노드를 탐색 */
		if (n->Free_status == TRUE && n->Size >= size)
			return n;
		else
		{
			n = n->Nextnode;

			/* 끝까지 가도 노드를 찾지 못하면 프로그램 종료 */
			if (n == node_head)
			{
				printf("조건을 만족하는 빈 노드가 없습니다.\n");
				return 0;
			}
		}
	}
}

node* dll_create()	// 처음 배열이 빈 상태일 경우
{
	int FullSize = 256 * MB;
	node* n = (node*)malloc(sizeof(node));
	MyBuffer = (char*)malloc(sizeof(char) * FullSize);
	// MyMemory = 256MB 할당

	n->Address = MyBuffer;
	n->Size = FullSize;
	n->Nextnode = n;
	n->Prenode = n;
	n->Free_status = TRUE;

	return n;
}
void* Mymalloc(size_t size)
{
	node* n = 0;
	node* temp = 0;

	if (node_head == 0) { // For first use
		node_head = dll_create();
	}
	temp = search_free_node(size);	// temp = 빈 노드
	if (temp == 0)
		printf("Malloc Fail\n");
	else
	{
		n = (node*)malloc(sizeof(node));	// node형 포인터 동적할당
		n->Address = temp->Address;			// temp에 주소 저장			
		n->Size = temp->Size;	// temp에 할당할 크기 저장
		n->Nextnode = temp;		// 노드의 다음노드는 temp
		n->Prenode = temp->Prenode;	// 노드의 이전 노드는 temp가 가리키던 노드의 이전노드
		n->Free_status = FALSE;		// 노드가 사용중임을 표시

		temp->Address = temp->Address + size;	// 빈 노드의 주소를 size만큼 증가
		temp->Size = temp->Size - size;			// 빈 노드의 크기는 size만큼 감소
		temp->Prenode->Nextnode = n;
		temp->Prenode = n;
		// 빈 공간에 size만큼의 노드 추가 완료
	}
	return n->Address;
}

node* search_node(void* addr)
{
	node* n = node_head;

	while (1)
	{
		if (n->Address == (char*)addr)
			return n;
		else
		{
			n = n->Nextnode;
			if (n == node_head) {
				printf("노드가 없습니다.\n");
				return 0;
			}
		}
	}
}

BOOL check_sequence(node* n1, node* n2)	// 노드가 연결되있는지 확인
{
	if (n1->Address + n1->Size == n2->Address)
		return TRUE;
	else
		return FALSE;
}
void mysize_free(void* addr)
{
	node* n = 0;
	if (!(n = search_node(addr))) {
		printf("[Free] free is failed: no node which has the address\n");
		return;
	}
	n = search_node(addr);
	n->Free_status = TRUE;
}
void myfree(void* addr) {	// Free() 함수 구현
	node* n = 0;
	node* temp = 0;

	if (!(n = search_node(addr))) { // There is no node which has the address
		printf("[Free] free is failed: no node which has the address\n");
		return;
	}
	if ((n->Prenode->Free_status == TRUE) && (n->Nextnode->Free_status == TRUE)) {
		if ((n->Prenode->Free_status == TRUE) && (check_sequence(n->Prenode, n) == TRUE)) {
			n->Prenode->Size += n->Size;
			n->Prenode->Nextnode = n->Nextnode;
			n->Prenode->Nextnode = n->Prenode;
			if ((n->Nextnode->Free_status == TRUE) && (n->Nextnode != n->Prenode && (check_sequence(n, n->Nextnode) == TRUE)))
			{
				n->Prenode->Size += n->Nextnode->Size;
				n->Prenode->Nextnode = n->Nextnode->Nextnode;
				n->Nextnode->Nextnode->Prenode = n->Prenode;
				free(n->Nextnode);
			}
			free(n);

		}
		else if (check_sequence(n, n->Nextnode) == TRUE) {
			temp = n->Nextnode;

			n->Nextnode->Address -= n->Size;
			n->Nextnode->Size += n->Size;
			n->Prenode->Nextnode = n->Nextnode;
			n->Nextnode->Prenode = n->Prenode;

			free(temp);
		}
	}
	else {
		n->Free_status = TRUE;
	}
}